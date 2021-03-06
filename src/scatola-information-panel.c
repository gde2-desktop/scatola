/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/*
 * Scatola
 *
 * Copyright (C) 1999, 2000, 2001 Eazel, Inc.
 *
 * Scatola is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Scatola is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * Author: Andy Hertzfeld <andy@eazel.com>
 *
 */

#include <config.h>
#include "scatola-information-panel.h"

#include "scatola-sidebar-title.h"

#include <eel/eel-background.h>
#include <eel/eel-glib-extensions.h>
#include <eel/eel-gtk-extensions.h>
#include <eel/eel-stock-dialogs.h>
#include <eel/eel-string.h>
#include <eel/eel-vfs-extensions.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <gtk/gtk.h>
#include <glib/gi18n.h>
#include <libscatola-private/scatola-dnd.h>
#include <libscatola-private/scatola-directory.h>
#include <libscatola-private/scatola-file-dnd.h>
#include <libscatola-private/scatola-file.h>
#include <libscatola-private/scatola-global-preferences.h>
#include <libscatola-private/scatola-keep-last-vertical-box.h>
#include <libscatola-private/scatola-metadata.h>
#include <libscatola-private/scatola-mime-actions.h>
#include <libscatola-private/scatola-program-choosing.h>
#include <libscatola-private/scatola-sidebar-provider.h>
#include <libscatola-private/scatola-module.h>

struct ScatolaInformationPanelDetails
{
    GtkWidget *container;
    ScatolaWindowInfo *window;
    ScatolaSidebarTitle *title;
    GtkWidget *button_box_centerer;
    GtkWidget *button_box;
    gboolean has_buttons;
    ScatolaFile *file;
    guint file_changed_connection;
    gboolean background_connected;

    char *default_background_color;
    char *default_background_image;
    char *current_background_color;
    char *current_background_image;
};

/* button assignments */
#define CONTEXTUAL_MENU_BUTTON 3

static gboolean scatola_information_panel_press_event           (GtkWidget                    *widget,
        GdkEventButton               *event);
static void     scatola_information_panel_finalize              (GObject                      *object);
static void     scatola_information_panel_drag_data_received    (GtkWidget                    *widget,
        GdkDragContext               *context,
        int                           x,
        int                           y,
        GtkSelectionData             *selection_data,
        guint                         info,
        guint                         time);
static void     scatola_information_panel_read_defaults         (ScatolaInformationPanel     *information_panel);
#if GTK_CHECK_VERSION (3, 0, 0)
static void     scatola_information_panel_style_updated         (GtkWidget                    *widget);
#else
static void     scatola_information_panel_style_set             (GtkWidget                    *widget,
        GtkStyle                     *previous_style);
#endif
static void     scatola_information_panel_theme_changed         (GSettings   *settings,
                                                              const gchar *key,
                                                              gpointer     user_data);
static void     scatola_information_panel_update_appearance     (ScatolaInformationPanel     *information_panel);
static void     scatola_information_panel_update_buttons        (ScatolaInformationPanel     *information_panel);
static void     background_metadata_changed_callback             (ScatolaInformationPanel     *information_panel);
static void     scatola_information_panel_iface_init            (ScatolaSidebarIface         *iface);
static void     scatola_information_panel_iface_init            (ScatolaSidebarIface         *iface);
static void     sidebar_provider_iface_init                      (ScatolaSidebarProviderIface *iface);
static GType    scatola_information_panel_provider_get_type     (void);

enum
{
    LOCATION_CHANGED,
    LAST_SIGNAL
};

static guint signals[LAST_SIGNAL] = { 0 };

/* drag and drop definitions */

enum
{
    TARGET_URI_LIST,
    TARGET_COLOR,
    TARGET_BGIMAGE,
    TARGET_KEYWORD,
    TARGET_BACKGROUND_RESET,
    TARGET_GDE2_URI_LIST
};

static const GtkTargetEntry target_table[] =
{
    { "text/uri-list",  0, TARGET_URI_LIST },
    { "application/x-color", 0, TARGET_COLOR },
    { "property/bgimage", 0, TARGET_BGIMAGE },
    { "property/keyword", 0, TARGET_KEYWORD },
    { "x-special/gde2-reset-background", 0, TARGET_BACKGROUND_RESET },
    { "x-special/gde2-icon-list",  0, TARGET_GDE2_URI_LIST }
};

typedef enum
{
    NO_PART,
    BACKGROUND_PART,
    ICON_PART
} InformationPanelPart;

typedef struct
{
    GObject parent;
} ScatolaInformationPanelProvider;

typedef struct
{
    GObjectClass parent;
} ScatolaInformationPanelProviderClass;


G_DEFINE_TYPE_WITH_CODE (ScatolaInformationPanel, scatola_information_panel, EEL_TYPE_BACKGROUND_BOX,
                         G_IMPLEMENT_INTERFACE (SCATOLA_TYPE_SIDEBAR,
                                 scatola_information_panel_iface_init));

G_DEFINE_TYPE_WITH_CODE (ScatolaInformationPanelProvider, scatola_information_panel_provider, G_TYPE_OBJECT,
                         G_IMPLEMENT_INTERFACE (SCATOLA_TYPE_SIDEBAR_PROVIDER,
                                 sidebar_provider_iface_init));


static const char *
scatola_information_panel_get_sidebar_id (ScatolaSidebar *sidebar)
{
    return SCATOLA_INFORMATION_PANEL_ID;
}

static char *
scatola_information_panel_get_tab_label (ScatolaSidebar *sidebar)
{
    return g_strdup (_("Information"));
}

static char *
scatola_information_panel_get_tab_tooltip (ScatolaSidebar *sidebar)
{
    return g_strdup (_("Show Information"));
}

static GdkPixbuf *
scatola_information_panel_get_tab_icon (ScatolaSidebar *sidebar)
{
    return NULL;
}

static void
scatola_information_panel_is_visible_changed (ScatolaSidebar *sidebar,
        gboolean         is_visible)
{
    /* Do nothing */
}

static void
scatola_information_panel_iface_init (ScatolaSidebarIface *iface)
{
    iface->get_sidebar_id = scatola_information_panel_get_sidebar_id;
    iface->get_tab_label = scatola_information_panel_get_tab_label;
    iface->get_tab_tooltip = scatola_information_panel_get_tab_tooltip;
    iface->get_tab_icon = scatola_information_panel_get_tab_icon;
    iface->is_visible_changed = scatola_information_panel_is_visible_changed;
}

/* initializing the class object by installing the operations we override */
static void
scatola_information_panel_class_init (ScatolaInformationPanelClass *klass)
{
    GtkWidgetClass *widget_class;
    GObjectClass *gobject_class;

    gobject_class = G_OBJECT_CLASS (klass);
    widget_class = GTK_WIDGET_CLASS (klass);

    gobject_class->finalize = scatola_information_panel_finalize;

    widget_class->drag_data_received  = scatola_information_panel_drag_data_received;
    widget_class->button_press_event  = scatola_information_panel_press_event;
#if GTK_CHECK_VERSION (3, 0, 0)
    widget_class->style_updated = scatola_information_panel_style_updated;
#else
    widget_class->style_set = scatola_information_panel_style_set;
#endif

    /* add the "location changed" signal */
    signals[LOCATION_CHANGED] = g_signal_new
                                ("location_changed",
                                 G_TYPE_FROM_CLASS (klass),
                                 G_SIGNAL_RUN_LAST,
                                 G_STRUCT_OFFSET (ScatolaInformationPanelClass,
                                         location_changed),
                                 NULL, NULL,
                                 g_cclosure_marshal_VOID__STRING,
                                 G_TYPE_NONE, 1, G_TYPE_STRING);

    g_type_class_add_private (klass, sizeof (ScatolaInformationPanelDetails));
}

/* utility routine to allocate the box the holds the command buttons */
static void
make_button_box (ScatolaInformationPanel *information_panel)
{
#if GTK_CHECK_VERSION (3, 0, 0)
    information_panel->details->button_box_centerer = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
#else
    information_panel->details->button_box_centerer = gtk_hbox_new (FALSE, 0);
#endif

    gtk_box_pack_start (GTK_BOX (information_panel->details->container),
                        information_panel->details->button_box_centerer, TRUE, TRUE, 0);

    information_panel->details->button_box = scatola_keep_last_vertical_box_new (4);
    gtk_container_set_border_width (GTK_CONTAINER (information_panel->details->button_box), 8);
    gtk_widget_show (information_panel->details->button_box);
    gtk_box_pack_start (GTK_BOX (information_panel->details->button_box_centerer),
                        information_panel->details->button_box,
                        TRUE, TRUE, 0);
    information_panel->details->has_buttons = FALSE;
}

/* initialize the instance's fields, create the necessary subviews, etc. */

static void
scatola_information_panel_init (ScatolaInformationPanel *information_panel)
{
    information_panel->details = G_TYPE_INSTANCE_GET_PRIVATE (information_panel,
    							      SCATOLA_TYPE_INFORMATION_PANEL,
    							      ScatolaInformationPanelDetails);

    /* load the default background */
    scatola_information_panel_read_defaults (information_panel);

    /* enable mouse tracking */
    gtk_widget_add_events (GTK_WIDGET (information_panel), GDK_POINTER_MOTION_MASK);

    /* create the container box */
#if GTK_CHECK_VERSION (3, 0, 0)
    information_panel->details->container = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
#else
    information_panel->details->container = gtk_vbox_new (FALSE, 0);
#endif
    gtk_container_set_border_width (GTK_CONTAINER (information_panel->details->container), 0);
    gtk_widget_show (information_panel->details->container);
    gtk_container_add (GTK_CONTAINER (information_panel),
                       information_panel->details->container);

    /* allocate and install the index title widget */
    information_panel->details->title = SCATOLA_SIDEBAR_TITLE (scatola_sidebar_title_new ());
    gtk_widget_show (GTK_WIDGET (information_panel->details->title));
    gtk_box_pack_start (GTK_BOX (information_panel->details->container),
                        GTK_WIDGET (information_panel->details->title),
                        FALSE, FALSE, 8);

    /* allocate and install the command button container */
    make_button_box (information_panel);

    /* add a callback for when the theme changes */
    g_signal_connect (scatola_preferences,
              "changed::" SCATOLA_PREFERENCES_SIDE_PANE_BACKGROUND_SET,
              G_CALLBACK(scatola_information_panel_theme_changed),
              information_panel);
    g_signal_connect (scatola_preferences,
              "changed::" SCATOLA_PREFERENCES_SIDE_PANE_BACKGROUND_COLOR,
              G_CALLBACK(scatola_information_panel_theme_changed),
              information_panel);
    g_signal_connect (scatola_preferences,
              "changed::" SCATOLA_PREFERENCES_SIDE_PANE_BACKGROUND_URI,
              G_CALLBACK(scatola_information_panel_theme_changed),
              information_panel);

    /* prepare ourselves to receive dropped objects */
    gtk_drag_dest_set (GTK_WIDGET (information_panel),
                       GTK_DEST_DEFAULT_MOTION | GTK_DEST_DEFAULT_HIGHLIGHT | GTK_DEST_DEFAULT_DROP,
                       target_table, G_N_ELEMENTS (target_table),
                       GDK_ACTION_COPY | GDK_ACTION_MOVE | GDK_ACTION_ASK);
}

static void
scatola_information_panel_finalize (GObject *object)
{
    ScatolaInformationPanel *information_panel;

    information_panel = SCATOLA_INFORMATION_PANEL (object);

    if (information_panel->details->file != NULL)
    {
        scatola_file_monitor_remove (information_panel->details->file, information_panel);
        scatola_file_unref (information_panel->details->file);
    }

    g_free (information_panel->details->default_background_color);
    g_free (information_panel->details->default_background_image);
    g_free (information_panel->details->current_background_color);
    g_free (information_panel->details->current_background_image);

    g_signal_handlers_disconnect_by_func (scatola_preferences,
                                          scatola_information_panel_theme_changed,
                                          information_panel);

    G_OBJECT_CLASS (scatola_information_panel_parent_class)->finalize (object);
}

/* callback to handle resetting the background */
static void
reset_background_callback (GtkWidget *menu_item, GtkWidget *information_panel)
{
    EelBackground *background;

    background = eel_get_widget_background (information_panel);
    if (background != NULL)
    {
        eel_background_reset (background);
    }
}

static gboolean
information_panel_has_background (ScatolaInformationPanel *information_panel)
{
    EelBackground *background;
    gboolean has_background;
    char *color;
    char *image;

    background = eel_get_widget_background (GTK_WIDGET(information_panel));

    color = eel_background_get_color (background);
    image = eel_background_get_image_uri (background);

    has_background = (color || image);

    return has_background;
}

/* create the context menu */
static GtkWidget *
scatola_information_panel_create_context_menu (ScatolaInformationPanel *information_panel)
{
    GtkWidget *menu, *menu_item;

    menu = gtk_menu_new ();
    gtk_menu_set_screen (GTK_MENU (menu),
                         gtk_widget_get_screen (GTK_WIDGET (information_panel)));

    /* add the reset background item, possibly disabled */
    menu_item = gtk_menu_item_new_with_mnemonic (_("Use _Default Background"));
    gtk_widget_show (menu_item);
    gtk_menu_shell_append (GTK_MENU_SHELL (menu), menu_item);
    gtk_widget_set_sensitive (menu_item, information_panel_has_background (information_panel));
    g_signal_connect_object (menu_item, "activate",
                             G_CALLBACK (reset_background_callback), information_panel, 0);

    return menu;
}

/* set up the default backgrounds and images */
static void
scatola_information_panel_read_defaults (ScatolaInformationPanel *information_panel)
{
    gboolean background_set;
    char *background_color, *background_image;

    background_set = g_settings_get_boolean (scatola_preferences, SCATOLA_PREFERENCES_SIDE_PANE_BACKGROUND_SET);

    background_color = NULL;
    background_image = NULL;
    if (background_set)
    {
        background_color = g_settings_get_string (scatola_preferences, SCATOLA_PREFERENCES_SIDE_PANE_BACKGROUND_COLOR);
        background_image = g_settings_get_string (scatola_preferences, SCATOLA_PREFERENCES_SIDE_PANE_BACKGROUND_URI);
    }

    g_free (information_panel->details->default_background_color);
    information_panel->details->default_background_color = NULL;
    g_free (information_panel->details->default_background_image);
    information_panel->details->default_background_image = NULL;

    if (background_color && strlen (background_color))
    {
        information_panel->details->default_background_color = g_strdup (background_color);
    }

    /* set up the default background image */

    if (background_image && strlen (background_image))
    {
        information_panel->details->default_background_image = g_strdup (background_image);
    }

    g_free (background_color);
    g_free (background_image);
}

/* handler for handling theme changes */

static void
scatola_information_panel_theme_changed (GSettings   *settings,
                                      const gchar *key,
                                      gpointer user_data)
{
    ScatolaInformationPanel *information_panel;

    information_panel = SCATOLA_INFORMATION_PANEL (user_data);
    scatola_information_panel_read_defaults (information_panel);
    scatola_information_panel_update_appearance (information_panel);
    gtk_widget_queue_draw (GTK_WIDGET (information_panel)) ;
}

/* hit testing */

static InformationPanelPart
hit_test (ScatolaInformationPanel *information_panel,
          int x, int y)
{
    GtkAllocation *allocation;
    gboolean bg_hit;

    if (scatola_sidebar_title_hit_test_icon (information_panel->details->title, x, y))
    {
        return ICON_PART;
    }

    allocation = g_new0 (GtkAllocation, 1);
    gtk_widget_get_allocation (GTK_WIDGET (information_panel), allocation);

    bg_hit = allocation != NULL
             && x >= allocation->x && y >= allocation->y
             && x < allocation->x + allocation->width
             && y < allocation->y + allocation->height;
    g_free (allocation);

    if (bg_hit)
    {
        return BACKGROUND_PART;
    }

    return NO_PART;
}

/* utility to test if a uri refers to a local image */
static gboolean
uri_is_local_image (const char *uri)
{
    GdkPixbuf *pixbuf;
    char *image_path;

    image_path = g_filename_from_uri (uri, NULL, NULL);
    if (image_path == NULL)
    {
        return FALSE;
    }

    pixbuf = gdk_pixbuf_new_from_file (image_path, NULL);
    g_free (image_path);

    if (pixbuf == NULL)
    {
        return FALSE;
    }
    g_object_unref (pixbuf);
    return TRUE;
}

static void
receive_dropped_uri_list (ScatolaInformationPanel *information_panel,
                          GdkDragAction action,
                          int x, int y,
                          GtkSelectionData *selection_data)
{
    char **uris;
    gboolean exactly_one;
    GtkWindow *window;
    EelBackground *background;

    uris = g_uri_list_extract_uris ((gchar *) gtk_selection_data_get_data (selection_data));
    exactly_one = uris[0] != NULL && (uris[1] == NULL || uris[1][0] == '\0');
    window = GTK_WINDOW (gtk_widget_get_toplevel (GTK_WIDGET (information_panel)));

    switch (hit_test (information_panel, x, y))
    {
    case NO_PART:
    case BACKGROUND_PART:
        /* FIXME bugzilla.gnome.org 42507: Does this work for all images, or only background images?
         * Other views handle background images differently from other URIs.
         */
        if (exactly_one && uri_is_local_image (uris[0]))
        {
            if (action == GDK_ACTION_ASK)
            {
                action = scatola_drag_drop_background_ask (GTK_WIDGET (information_panel),
                             SCATOLA_DND_ACTION_SET_AS_BACKGROUND | SCATOLA_DND_ACTION_SET_AS_GLOBAL_BACKGROUND);
            }

            if (action > 0)
            {
                background = eel_get_widget_background (GTK_WIDGET (information_panel));
                eel_background_set_dropped_image (background, action, uris[0]);
            }
        }
        else if (exactly_one)
        {
            g_signal_emit (information_panel, signals[LOCATION_CHANGED], 0, uris[0]);
        }
        break;
    case ICON_PART:
        /* handle images dropped on the logo specially */

        if (!exactly_one)
        {
            eel_show_error_dialog (
                _("You cannot assign more than one custom icon at a time."),
                _("Please drag just one image to set a custom icon."),
                window);
            break;
        }

        if (uri_is_local_image (uris[0]))
        {
            if (information_panel->details->file != NULL)
            {
                scatola_file_set_metadata (information_panel->details->file,
                                        SCATOLA_METADATA_KEY_CUSTOM_ICON,
                                        NULL,
                                        uris[0]);
                scatola_file_set_metadata (information_panel->details->file,
                                        SCATOLA_METADATA_KEY_ICON_SCALE,
                                        NULL,
                                        NULL);
            }
        }
        else
        {
            GFile *f;

            f = g_file_new_for_uri (uris[0]);
            if (!g_file_is_native (f))
            {
                eel_show_error_dialog (
                    _("The file that you dropped is not local."),
                    _("You can only use local images as custom icons."),
                    window);

            }
            else
            {
                eel_show_error_dialog (
                    _("The file that you dropped is not an image."),
                    _("You can only use images as custom icons."),
                    window);
            }
            g_object_unref (f);
        }
        break;
    }

    g_strfreev (uris);
}

static void
receive_dropped_color (ScatolaInformationPanel *information_panel,
                       GdkDragAction action,
                       int x, int y,
                       GtkSelectionData *selection_data)
{
    guint16 *channels;
    char color_spec[8];
    EelBackground *background;

    if (gtk_selection_data_get_length (selection_data) != 8 ||
            gtk_selection_data_get_format (selection_data) != 16)
    {
        g_warning ("received invalid color data");
        return;
    }

    channels = (guint16 *) gtk_selection_data_get_data (selection_data);
    g_snprintf (color_spec, sizeof (color_spec),
                "#%02X%02X%02X", channels[0] >> 8, channels[1] >> 8, channels[2] >> 8);

    switch (hit_test (information_panel, x, y))
    {
    case NO_PART:
        g_warning ("dropped color, but not on any part of information_panel");
        break;
    case ICON_PART:
    case BACKGROUND_PART:
        if (action == GDK_ACTION_ASK)
        {
            action = scatola_drag_drop_background_ask (GTK_WIDGET (information_panel),
                         SCATOLA_DND_ACTION_SET_AS_BACKGROUND | SCATOLA_DND_ACTION_SET_AS_GLOBAL_BACKGROUND);
        }

        if (action > 0)
        {
            background = eel_get_widget_background (GTK_WIDGET (information_panel));
            eel_background_set_dropped_color (background, GTK_WIDGET (information_panel),
                                              action, x, y, selection_data);
        }

        break;
    }
}

/* handle receiving a dropped keyword */

static void
receive_dropped_keyword (ScatolaInformationPanel *information_panel,
                         int x, int y,
                         GtkSelectionData *selection_data)
{
    scatola_drag_file_receive_dropped_keyword (information_panel->details->file,
                                            gtk_selection_data_get_data (selection_data));

    /* regenerate the display */
    scatola_information_panel_update_appearance (information_panel);
}

static void
scatola_information_panel_drag_data_received (GtkWidget *widget, GdkDragContext *context,
        int x, int y,
        GtkSelectionData *selection_data,
        guint info, guint time)
{
    ScatolaInformationPanel *information_panel;
    EelBackground *background;

    g_return_if_fail (SCATOLA_IS_INFORMATION_PANEL (widget));

    information_panel = SCATOLA_INFORMATION_PANEL (widget);

    switch (info)
    {
    case TARGET_GDE2_URI_LIST:
    case TARGET_URI_LIST:
        receive_dropped_uri_list (information_panel,
                                  gdk_drag_context_get_selected_action (context), x, y, selection_data);
        break;
    case TARGET_COLOR:
        receive_dropped_color (information_panel,
                               gdk_drag_context_get_selected_action (context), x, y, selection_data);
        break;
    case TARGET_BGIMAGE:
        if (hit_test (information_panel, x, y) == BACKGROUND_PART)
            receive_dropped_uri_list (information_panel,
                                      gdk_drag_context_get_selected_action (context), x, y, selection_data);
        break;
    case TARGET_BACKGROUND_RESET:
        background = eel_get_widget_background ( GTK_WIDGET (information_panel));
        if (background != NULL)
        {
            eel_background_reset (background);
        }
        break;
    case TARGET_KEYWORD:
        receive_dropped_keyword (information_panel, x, y, selection_data);
        break;
    default:
        g_warning ("unknown drop type");
    }
}

/* handle the context menu if necessary */
static gboolean
scatola_information_panel_press_event (GtkWidget *widget, GdkEventButton *event)
{
    ScatolaInformationPanel *information_panel;
    GtkWidget *menu;

    if (gtk_widget_get_window (widget) != event->window)
    {
        return FALSE;
    }

    information_panel = SCATOLA_INFORMATION_PANEL (widget);

    /* handle the context menu */
    if (event->button == CONTEXTUAL_MENU_BUTTON)
    {
        menu = scatola_information_panel_create_context_menu (information_panel);
        eel_pop_up_context_menu (GTK_MENU(menu),
                                 EEL_DEFAULT_POPUP_MENU_DISPLACEMENT,
                                 EEL_DEFAULT_POPUP_MENU_DISPLACEMENT,
                                 event);
    }
    return TRUE;
}

static gboolean
value_different (const char *a, const char *b)
{
    if (!a && !b)
        return FALSE;

    if (!a || !b)
        return TRUE;

    return strcmp (a, b);
}

/* Handle the background changed signal by writing out the settings to metadata.
 */
static void
background_settings_changed_callback (EelBackground *background, GdkDragAction action, ScatolaInformationPanel *information_panel)
{
    char *image;
    char *color;

    g_assert (EEL_IS_BACKGROUND (background));
    g_assert (SCATOLA_IS_INFORMATION_PANEL (information_panel));

    if (information_panel->details->file == NULL)
    {
        return;
    }

    /* Block so we don't respond to our own metadata changes.
     */
    g_signal_handlers_block_by_func (information_panel->details->file,
                                     G_CALLBACK (background_metadata_changed_callback),
                                     information_panel);

    color = eel_background_get_color (background);
    image = eel_background_get_image_uri (background);

    if (action != (GdkDragAction) SCATOLA_DND_ACTION_SET_AS_BACKGROUND)
    {
        scatola_file_set_metadata (information_panel->details->file,
                                SCATOLA_METADATA_KEY_SIDEBAR_BACKGROUND_COLOR,
                                NULL,
                                NULL);

        scatola_file_set_metadata (information_panel->details->file,
                                SCATOLA_METADATA_KEY_SIDEBAR_BACKGROUND_IMAGE,
                                NULL,
                                NULL);

        g_signal_handlers_block_by_func (scatola_preferences,
                         G_CALLBACK(scatola_information_panel_theme_changed),
                         information_panel);
        g_settings_set_string (scatola_preferences,
                       SCATOLA_PREFERENCES_SIDE_PANE_BACKGROUND_COLOR, color ? color : "");
        g_settings_set_string (scatola_preferences,
                       SCATOLA_PREFERENCES_SIDE_PANE_BACKGROUND_URI, image ? image : "");
        g_settings_set_boolean (scatola_preferences,
                    SCATOLA_PREFERENCES_SIDE_PANE_BACKGROUND_SET, TRUE);
        g_signal_handlers_unblock_by_func (scatola_preferences,
                           G_CALLBACK(scatola_information_panel_theme_changed),
                           information_panel);
    }
    else
    {
        scatola_file_set_metadata (information_panel->details->file,
                                SCATOLA_METADATA_KEY_SIDEBAR_BACKGROUND_COLOR,
                                NULL,
                                color);

        scatola_file_set_metadata (information_panel->details->file,
                                SCATOLA_METADATA_KEY_SIDEBAR_BACKGROUND_IMAGE,
                                NULL,
                                image);
    }

    if (value_different (information_panel->details->current_background_color, color))
    {
        g_free (information_panel->details->current_background_color);
        information_panel->details->current_background_color = g_strdup (color);
    }

    if (value_different (information_panel->details->current_background_image, image))
    {
        g_free (information_panel->details->current_background_image);
        information_panel->details->current_background_image = g_strdup (image);
    }

    g_free (color);
    g_free (image);

    g_signal_handlers_unblock_by_func (information_panel->details->file,
                                       G_CALLBACK (background_metadata_changed_callback),
                                       information_panel);
}

/* handle the background reset signal by writing out NULL to metadata and setting the backgrounds
   fields to their default values */
static void
background_reset_callback (EelBackground *background, ScatolaInformationPanel *information_panel)
{
    char *color;
    char *image;
    g_assert (EEL_IS_BACKGROUND (background));
    g_assert (SCATOLA_IS_INFORMATION_PANEL (information_panel));

    if (information_panel->details->file == NULL)
    {
        return;
    }

    /* Block so we don't respond to our own metadata changes.
     */
    g_signal_handlers_block_by_func (information_panel->details->file,
                                     G_CALLBACK (background_metadata_changed_callback),
                                     information_panel);

    color = scatola_file_get_metadata (information_panel->details->file,
                                    SCATOLA_METADATA_KEY_SIDEBAR_BACKGROUND_COLOR,
                                    NULL);

    image = scatola_file_get_metadata (information_panel->details->file,
                                    SCATOLA_METADATA_KEY_SIDEBAR_BACKGROUND_IMAGE,
                                    NULL);
    if (color || image)
    {
        scatola_file_set_metadata (information_panel->details->file,
                                SCATOLA_METADATA_KEY_SIDEBAR_BACKGROUND_COLOR,
                                NULL,
                                NULL);

        scatola_file_set_metadata (information_panel->details->file,
                                SCATOLA_METADATA_KEY_SIDEBAR_BACKGROUND_IMAGE,
                                NULL,
                                NULL);
    }
    else
    {
        g_signal_handlers_block_by_func (scatola_preferences,
                         G_CALLBACK(scatola_information_panel_theme_changed),
                         information_panel);
        g_settings_set_boolean (scatola_preferences,
                    SCATOLA_PREFERENCES_SIDE_PANE_BACKGROUND_SET, FALSE);
        g_signal_handlers_unblock_by_func (scatola_preferences,
                         G_CALLBACK(scatola_information_panel_theme_changed),
                         information_panel);
    }

    g_signal_handlers_unblock_by_func (information_panel->details->file,
                                       G_CALLBACK (background_metadata_changed_callback),
                                       information_panel);

    /* Force a read from the metadata to set the defaults
     */
    background_metadata_changed_callback (information_panel);
}

static GtkWindow *
scatola_information_panel_get_window (ScatolaInformationPanel *information_panel)
{
    GtkWidget *result;

    result = gtk_widget_get_ancestor (GTK_WIDGET (information_panel), GTK_TYPE_WINDOW);

    return result == NULL ? NULL : GTK_WINDOW (result);
}

static void
command_button_callback (GtkWidget *button, GAppInfo *application)
{
    ScatolaInformationPanel *information_panel;
    GList files;

    information_panel = SCATOLA_INFORMATION_PANEL (g_object_get_data (G_OBJECT (button), "user_data"));

    files.next = NULL;
    files.prev = NULL;
    files.data = information_panel->details->file;
    scatola_launch_application (application, &files,
                             scatola_information_panel_get_window (information_panel));
}

/* interpret commands for buttons specified by metadata. Handle some built-in ones explicitly, or fork
   a shell to handle general ones */
/* for now, we don't have any of these */
static void
metadata_button_callback (GtkWidget *button, const char *command_str)
{
    //ScatolaInformationPanel *self = SCATOLA_INFORMATION_PANEL (g_object_get_data (G_OBJECT (button), "user_data"));
}

/* utility routine that allocates the command buttons from the command list */

static void
add_command_button (ScatolaInformationPanel *information_panel, GAppInfo *application)
{
    char *temp_str;
    GtkWidget *temp_button, *label;

    /* There's always at least the "Open with..." button */
    information_panel->details->has_buttons = TRUE;

    temp_str = g_strdup_printf (_("Open With %s"), g_app_info_get_display_name (application));
    temp_button = gtk_button_new_with_label (temp_str);
    label = gtk_bin_get_child (GTK_BIN (temp_button));
    gtk_label_set_ellipsize (GTK_LABEL (label), PANGO_ELLIPSIZE_START);
    g_free (temp_str);
    gtk_box_pack_start (GTK_BOX (information_panel->details->button_box),
                        temp_button,
                        FALSE, FALSE,
                        0);

    g_signal_connect_data (temp_button,
                           "clicked",
                           G_CALLBACK (command_button_callback),
                           g_object_ref (application),
                           (GClosureNotify)g_object_unref,
                           0);

    g_object_set_data (G_OBJECT (temp_button), "user_data", information_panel);

    gtk_widget_show (temp_button);
}

/* utility to construct command buttons for the information_panel from the passed in metadata string */

static void
add_buttons_from_metadata (ScatolaInformationPanel *information_panel, const char *button_data)
{
    char **terms;
    char *current_term, *temp_str;
    char *button_name, *command_string;
    const char *term;
    int index;
    GtkWidget *temp_button;

    /* split the button specification into a set of terms */
    button_name = NULL;
    terms = g_strsplit (button_data, ";", 0);

    /* for each term, either create a button or attach a property to one */
    for (index = 0; (term = terms[index]) != NULL; index++)
    {
        current_term = g_strdup (term);
        temp_str = strchr (current_term, '=');
        if (temp_str)
        {
            *temp_str = '\0';
            if (!g_ascii_strcasecmp (current_term, "button"))
            {
                button_name = g_strdup (temp_str + 1);
            }
            else if (!g_ascii_strcasecmp (current_term, "script"))
            {
                if (button_name != NULL)
                {
                    temp_button = gtk_button_new_with_label (button_name);
                    gtk_box_pack_start (GTK_BOX (information_panel->details->button_box),
                                        temp_button,
                                        FALSE, FALSE,
                                        0);
                    information_panel->details->has_buttons = TRUE;
                    command_string = g_strdup (temp_str + 1);
                    g_free (button_name);

                    g_signal_connect_data (temp_button,
                                           "clicked",
                                           G_CALLBACK (metadata_button_callback),
                                           command_string,
                                           (GClosureNotify)g_free,
                                           0);

                    g_object_set_data (G_OBJECT (temp_button), "user_data", information_panel);

                    gtk_widget_show (temp_button);
                }
            }
        }
        g_free(current_term);
    }
    g_strfreev (terms);
}

/*
 * scatola_information_panel_update_buttons:
 *
 * Update the list of program-launching buttons based on the current uri.
 */
static void
scatola_information_panel_update_buttons (ScatolaInformationPanel *information_panel)
{
    char *button_data;
    GAppInfo *default_app;

    /* dispose of any existing buttons */
    if (information_panel->details->has_buttons)
    {
        gtk_container_remove (GTK_CONTAINER (information_panel->details->container),
                              information_panel->details->button_box_centerer);
        make_button_box (information_panel);
    }

    /* create buttons from file metadata if necessary */
    button_data = scatola_file_get_metadata (information_panel->details->file,
                                          SCATOLA_METADATA_KEY_SIDEBAR_BUTTONS,
                                          NULL);
    if (button_data)
    {
        add_buttons_from_metadata (information_panel, button_data);
        g_free(button_data);
    }

    /* Make a button for the default application */
    if (scatola_mime_has_any_applications_for_file (information_panel->details->file) &&
            !scatola_file_is_directory (information_panel->details->file))
    {
        default_app =
            scatola_mime_get_default_application_for_file (information_panel->details->file);
        add_command_button (information_panel, default_app);
        g_object_unref (default_app);
    }

    gtk_widget_show (information_panel->details->button_box_centerer);
}

static void
scatola_information_panel_update_appearance (ScatolaInformationPanel *information_panel)
{

#if GTK_CHECK_VERSION(3,0,0)
    gtk_style_context_add_class (gtk_widget_get_style_context (GTK_WIDGET (information_panel)),
                                 GTK_STYLE_CLASS_VIEW);
#else
    EelBackground *background;
    char *background_color;
    char *background_image;

    g_return_if_fail (SCATOLA_IS_INFORMATION_PANEL (information_panel));

    /* Connect the background changed signal to code that writes the color. */
    background = eel_get_widget_background (GTK_WIDGET (information_panel));
    if (!information_panel->details->background_connected)
    {
        information_panel->details->background_connected = TRUE;
        g_signal_connect_object (background,"settings_changed",
                                 G_CALLBACK (background_settings_changed_callback), information_panel, 0);
        g_signal_connect_object (background, "reset",
                                 G_CALLBACK (background_reset_callback), information_panel, 0);
    }

    /* Set up the background color and image from the metadata. */
    background_color = scatola_file_get_metadata (information_panel->details->file,
                       SCATOLA_METADATA_KEY_SIDEBAR_BACKGROUND_COLOR,
                       NULL);
    background_image = scatola_file_get_metadata (information_panel->details->file,
                       SCATOLA_METADATA_KEY_SIDEBAR_BACKGROUND_IMAGE,
                       NULL);

    if (background_color == NULL && background_image == NULL)
    {
        background_color = g_strdup (information_panel->details->default_background_color);
        background_image = g_strdup (information_panel->details->default_background_image);
    }

    /* Block so we don't write these settings out in response to our set calls below */
    g_signal_handlers_block_by_func (background,
                                     G_CALLBACK (background_settings_changed_callback),
                                     information_panel);

    if (value_different (information_panel->details->current_background_color, background_color) ||
            value_different (information_panel->details->current_background_image, background_image))
    {

        g_free (information_panel->details->current_background_color);
        information_panel->details->current_background_color = g_strdup (background_color);
        g_free (information_panel->details->current_background_image);
        information_panel->details->current_background_image = g_strdup (background_image);

        eel_background_set_image_uri (background, background_image);
        eel_background_set_color (background, background_color);

        scatola_sidebar_title_select_text_color (information_panel->details->title, background);
    }

    g_free (background_color);
    g_free (background_image);

    g_signal_handlers_unblock_by_func (background,
                                       G_CALLBACK (background_settings_changed_callback),
                                       information_panel);
#endif
}

static void
background_metadata_changed_callback (ScatolaInformationPanel *information_panel)
{
    ScatolaFileAttributes attributes;
    gboolean ready;

    attributes = scatola_mime_actions_get_required_file_attributes ();
    ready = scatola_file_check_if_ready (information_panel->details->file, attributes);

    if (ready)
    {
        scatola_information_panel_update_appearance (information_panel);

        /* set up the command buttons */
        scatola_information_panel_update_buttons (information_panel);
    }
}

/* here is the key routine that populates the information_panel with the appropriate information when the uri changes */

static void
scatola_information_panel_set_uri (ScatolaInformationPanel *information_panel,
                                const char* new_uri,
                                const char* initial_title)
{
    ScatolaFile *file;
    ScatolaFileAttributes attributes;

    g_return_if_fail (SCATOLA_IS_INFORMATION_PANEL (information_panel));
    g_return_if_fail (new_uri != NULL);
    g_return_if_fail (initial_title != NULL);

    /* there's nothing to do if the uri is the same as the current one */
    if (information_panel->details->file != NULL &&
            scatola_file_matches_uri (information_panel->details->file, new_uri))
    {
        return;
    }

    if (information_panel->details->file != NULL)
    {
        g_signal_handler_disconnect (information_panel->details->file,
                                     information_panel->details->file_changed_connection);
        scatola_file_monitor_remove (information_panel->details->file, information_panel);
    }

    file = scatola_file_get_by_uri (new_uri);

    scatola_file_unref (information_panel->details->file);
    information_panel->details->file = file;

    information_panel->details->file_changed_connection =
        g_signal_connect_object (information_panel->details->file, "changed",
                                 G_CALLBACK (background_metadata_changed_callback),
                                 information_panel, G_CONNECT_SWAPPED);

    attributes = scatola_mime_actions_get_required_file_attributes ();
    scatola_file_monitor_add (information_panel->details->file, information_panel, attributes);

    background_metadata_changed_callback (information_panel);

    /* tell the title widget about it */
    scatola_sidebar_title_set_file (information_panel->details->title,
                                 information_panel->details->file,
                                 initial_title);
}

static void
title_changed_callback (ScatolaWindowInfo *window,
                        char               *new_title,
                        ScatolaInformationPanel *panel)
{
    scatola_sidebar_title_set_text (panel->details->title,
                                 new_title);
}

/* ::style_set handler for the information_panel */
static void
#if GTK_CHECK_VERSION (3, 0, 0)
scatola_information_panel_style_updated (GtkWidget *widget)
#else
scatola_information_panel_style_set (GtkWidget *widget, GtkStyle *previous_style)
#endif
{
    ScatolaInformationPanel *information_panel;

    information_panel = SCATOLA_INFORMATION_PANEL (widget);

    scatola_information_panel_theme_changed (NULL, NULL, information_panel);
}

static void
loading_uri_callback (ScatolaWindowInfo *window,
                      char               *uri,
                      ScatolaInformationPanel *panel)
{
    ScatolaWindowSlotInfo *slot;
    char *title;

    slot = scatola_window_info_get_active_slot (window);

    title = scatola_window_slot_info_get_title (slot);
    scatola_information_panel_set_uri (panel,
                                    uri,
                                    title);
    g_free (title);
}

static void
selection_changed_callback (ScatolaWindowInfo *window,
                            ScatolaInformationPanel *panel)
{
    int selection_count;
    GList *selection;
    GFile *selected;
    ScatolaFile *file;
    char *uri, *name;

    selection = scatola_window_info_get_selection (window);
    selection_count = g_list_length (selection);

    if (selection_count == 1)
    {
        selection = scatola_window_info_get_selection (window);
        selected = selection->data;

        /* this should never fail here, as we're displaying the file */
        file = scatola_file_get_existing (selected);
        uri = scatola_file_get_uri (file);
        name = scatola_file_get_display_name (file);

        scatola_file_unref (file);
    }
    else
    {
        uri = scatola_window_info_get_current_location (window);
        name = scatola_window_info_get_title (window);
    }

    scatola_information_panel_set_uri (panel, uri, name);

    g_list_free_full (selection, g_object_unref);
    g_free (uri);
    g_free (name);
}

static void
scatola_information_panel_set_parent_window (ScatolaInformationPanel *panel,
        ScatolaWindowInfo *window)
{
    gpointer slot;
    char *title, *location;

    panel->details->window = window;

    g_signal_connect_object (window, "loading_uri",
                             G_CALLBACK (loading_uri_callback), panel, 0);
    g_signal_connect_object (window, "title_changed",
                             G_CALLBACK (title_changed_callback), panel, 0);
    g_signal_connect_object (window, "selection-changed",
                             G_CALLBACK (selection_changed_callback), panel, 0);

    slot = scatola_window_info_get_active_slot (window);

    title = scatola_window_slot_info_get_title (slot);
    location = scatola_window_slot_info_get_current_location (slot);
    scatola_information_panel_set_uri (panel,
                                    location,
                                    title);
    g_free (location);
    g_free (title);
}

static ScatolaSidebar *
scatola_information_panel_create (ScatolaSidebarProvider *provider,
                               ScatolaWindowInfo *window)
{
    ScatolaInformationPanel *panel;

    panel = g_object_new (scatola_information_panel_get_type (), NULL);
    scatola_information_panel_set_parent_window (panel, window);
    g_object_ref_sink (panel);

    return SCATOLA_SIDEBAR (panel);
}

static void
sidebar_provider_iface_init (ScatolaSidebarProviderIface *iface)
{
    iface->create = scatola_information_panel_create;
}

static void
scatola_information_panel_provider_init (ScatolaInformationPanelProvider *sidebar)
{
}

static void
scatola_information_panel_provider_class_init (ScatolaInformationPanelProviderClass *class)
{
}

void
scatola_information_panel_register (void)
{
    scatola_module_add_type (scatola_information_panel_provider_get_type ());
}

