/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */

/*
 *  Scatola
 *
 *  Copyright (C) 1999, 2000 Red Hat, Inc.
 *  Copyright (C) 1999, 2000, 2001 Eazel, Inc.
 *
 *  Scatola is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  Scatola is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public
 *  License along with this program; if not, write to the Free
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 *  Authors: Elliot Lee <sopwith@redhat.com>
 *  	     John Sullivan <sullivan@eazel.com>
 *
 */

/* scatola-window.c: Implementation of the main window object */

#include <config.h>
#include "scatola-spatial-window.h"
#include "scatola-window-private.h"
#include "scatola-window-bookmarks.h"

#include "scatola-actions.h"
#include "scatola-application.h"
#include "scatola-desktop-window.h"
#include "scatola-bookmarks-window.h"
#include "scatola-location-dialog.h"
#include "scatola-main.h"
#include "scatola-query-editor.h"
#include "scatola-search-bar.h"
#include "scatola-window-manage-views.h"
#include <eel/eel-gtk-extensions.h>
#include <eel/eel-gtk-macros.h>
#include <eel/eel-string.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <gdk/gdkkeysyms.h>
#include <gdk/gdkx.h>
#include <gtk/gtk.h>
#include <glib/gi18n.h>
#include <libscatola-private/scatola-dnd.h>
#include <libscatola-private/scatola-file-utilities.h>
#include <libscatola-private/scatola-ui-utilities.h>
#include <libscatola-private/scatola-file-attributes.h>
#include <libscatola-private/scatola-global-preferences.h>
#include <libscatola-private/scatola-metadata.h>
#include <libscatola-private/scatola-mime-actions.h>
#include <libscatola-private/scatola-program-choosing.h>
#include <libscatola-private/scatola-search-directory.h>
#include <libscatola-private/scatola-search-engine.h>
#include <libscatola-private/scatola-signaller.h>

#if !GTK_CHECK_VERSION(3,0,0)
#define gtk_widget_get_preferred_size(x,y,z) gtk_widget_size_request(x,y)
#endif

#define MAX_TITLE_LENGTH 180
#define MAX_SHORTNAME_PATH 16

#define SPATIAL_ACTION_PLACES               "Places"
#define SPATIAL_ACTION_GO_TO_LOCATION       "Go to Location"
#define SPATIAL_ACTION_CLOSE_PARENT_FOLDERS "Close Parent Folders"
#define SPATIAL_ACTION_CLOSE_ALL_FOLDERS    "Close All Folders"
#define MENU_PATH_SPATIAL_BOOKMARKS_PLACEHOLDER	"/MenuBar/Other Menus/Places/Bookmarks Placeholder"

#if GTK_CHECK_VERSION (3, 0, 0)
#define gtk_hbox_new(X,Y) gtk_box_new(GTK_ORIENTATION_HORIZONTAL,Y)
#endif

struct _ScatolaSpatialWindowDetails
{
    GtkActionGroup *spatial_action_group; /* owned by ui_manager */
    char *last_geometry;
    guint save_geometry_timeout_id;

    gboolean saved_data_on_close;
    GtkWidget *content_box;
    GtkWidget *location_button;
    GtkWidget *location_label;
    GtkWidget *location_icon;
};

static const GtkTargetEntry location_button_drag_types[] =
{
    { SCATOLA_ICON_DND_GDE2_ICON_LIST_TYPE, 0, SCATOLA_ICON_DND_GDE2_ICON_LIST },
    { SCATOLA_ICON_DND_URI_LIST_TYPE, 0, SCATOLA_ICON_DND_URI_LIST },
};

G_DEFINE_TYPE(ScatolaSpatialWindow, scatola_spatial_window, SCATOLA_TYPE_WINDOW)
#define parent_class scatola_spatial_window_parent_class

static void scatola_spatial_window_save_geometry (ScatolaSpatialWindow *window,
						   ScatolaFile *viewed_file);

static gboolean
save_window_geometry_timeout (gpointer callback_data)
{
    ScatolaSpatialWindow *window;
    ScatolaWindowSlot *slot;

    window = SCATOLA_SPATIAL_WINDOW (callback_data);
    slot = scatola_window_get_active_slot (SCATOLA_WINDOW (window));

    if (slot != NULL)
    {
        scatola_spatial_window_save_geometry (window, slot->viewed_file);
    }

    window->details->save_geometry_timeout_id = 0;

    return FALSE;
}

static gboolean
scatola_spatial_window_configure_event (GtkWidget *widget,
                                     GdkEventConfigure *event)
{
    ScatolaSpatialWindow *window;

    window = SCATOLA_SPATIAL_WINDOW (widget);

    GTK_WIDGET_CLASS (scatola_spatial_window_parent_class)->configure_event (widget, event);

    /* Only save the geometry if the user hasn't resized the window
     * for a second. Otherwise delay the callback another second.
     */
    if (window->details->save_geometry_timeout_id != 0)
    {
        g_source_remove (window->details->save_geometry_timeout_id);
    }

	window->details->save_geometry_timeout_id =
		g_timeout_add_seconds (1, save_window_geometry_timeout, window);

    return FALSE;
}

static void
scatola_spatial_window_unrealize (GtkWidget *widget)
{
    ScatolaSpatialWindow *window;
    ScatolaWindowSlot *slot;

    window = SCATOLA_SPATIAL_WINDOW (widget);
    slot = scatola_window_get_active_slot (SCATOLA_WINDOW (window));

    GTK_WIDGET_CLASS (scatola_spatial_window_parent_class)->unrealize (widget);

    if (window->details->save_geometry_timeout_id != 0)
    {
        g_source_remove (window->details->save_geometry_timeout_id);
        window->details->save_geometry_timeout_id = 0;

        if (slot != NULL)
        {
            scatola_spatial_window_save_geometry (window, slot->viewed_file);
        }
    }
}

static gboolean
scatola_spatial_window_state_event (GtkWidget *widget,
                                 GdkEventWindowState *event)
{
    ScatolaWindow *window;
    ScatolaWindowSlot *slot;
    ScatolaFile *viewed_file;

    window = SCATOLA_WINDOW (widget);
    slot = window->details->active_pane->active_slot;
    viewed_file = slot->viewed_file;

    if (!SCATOLA_IS_DESKTOP_WINDOW (widget))
    {

        if (event->changed_mask & GDK_WINDOW_STATE_MAXIMIZED &&
                viewed_file != NULL)
        {
            scatola_file_set_boolean_metadata (viewed_file,
                                            SCATOLA_METADATA_KEY_WINDOW_MAXIMIZED,
                                            FALSE,
                                            event->new_window_state & GDK_WINDOW_STATE_MAXIMIZED);
        }

        if (event->changed_mask & GDK_WINDOW_STATE_STICKY &&
                viewed_file != NULL)
        {
            scatola_file_set_boolean_metadata (viewed_file,
                                            SCATOLA_METADATA_KEY_WINDOW_STICKY,
                                            FALSE,
                                            event->new_window_state & GDK_WINDOW_STATE_STICKY);
        }

        if (event->changed_mask & GDK_WINDOW_STATE_ABOVE &&
                viewed_file != NULL)
        {
            scatola_file_set_boolean_metadata (viewed_file,
                                            SCATOLA_METADATA_KEY_WINDOW_KEEP_ABOVE,
                                            FALSE,
                                            event->new_window_state & GDK_WINDOW_STATE_ABOVE);
        }

    }

    if (GTK_WIDGET_CLASS (scatola_spatial_window_parent_class)->window_state_event != NULL)
    {
        return GTK_WIDGET_CLASS (scatola_spatial_window_parent_class)->window_state_event (widget, event);
    }

    return FALSE;
}

static void
scatola_spatial_window_finalize (GObject *object)
{
    ScatolaSpatialWindow *window;

    window = SCATOLA_SPATIAL_WINDOW (object);

    g_free (window->details->last_geometry);

    G_OBJECT_CLASS (scatola_spatial_window_parent_class)->finalize (object);
}

static void
scatola_spatial_window_save_geometry (ScatolaSpatialWindow *window,
        		   ScatolaFile *viewed_file)
{
    char *geometry_string;

    if (viewed_file == NULL)
    {
        /* We never showed a file */
        return;
    }

    if (gtk_widget_get_window (GTK_WIDGET (window)) &&
    	    gtk_widget_get_visible (GTK_WIDGET (window)) &&
	    !SCATOLA_IS_DESKTOP_WINDOW (window) &&
            !(gdk_window_get_state (gtk_widget_get_window (GTK_WIDGET(window))) & GDK_WINDOW_STATE_MAXIMIZED)) {

        geometry_string = eel_gtk_window_get_geometry_string (GTK_WINDOW (window));

        if (!g_strcmp0 (window->details->last_geometry, geometry_string)) {
        	/* Don't save geometry if it's the same as before. */
        	g_free (geometry_string);
        	return;
        }

        g_free (window->details->last_geometry);
        window->details->last_geometry = geometry_string;

        scatola_file_set_metadata (viewed_file,
                                SCATOLA_METADATA_KEY_WINDOW_GEOMETRY,
                                NULL,
                                geometry_string);
    }
}

static void
scatola_spatial_window_save_scroll_position (ScatolaSpatialWindow *window,
					  ScatolaWindowSlot *slot)
{
    char *scroll_string;

    if (slot->content_view == NULL ||
            slot->viewed_file == NULL)
    {
        return;
    }

    scroll_string = scatola_view_get_first_visible_file (slot->content_view);
    scatola_file_set_metadata (slot->viewed_file,
                            SCATOLA_METADATA_KEY_WINDOW_SCROLL_POSITION,
                            NULL,
                            scroll_string);
    g_free (scroll_string);
}

static void
scatola_spatial_window_save_show_hidden_files_mode (ScatolaSpatialWindow *window,
						 ScatolaFile *viewed_file)
{
    char *show_hidden_file_setting;
    ScatolaWindowShowHiddenFilesMode mode;

    if (viewed_file == NULL) {
        return;
    }

    mode = SCATOLA_WINDOW (window)->details->show_hidden_files_mode;

    if (mode != SCATOLA_WINDOW_SHOW_HIDDEN_FILES_DEFAULT) {
        if (mode == SCATOLA_WINDOW_SHOW_HIDDEN_FILES_ENABLE) {
            show_hidden_file_setting = "1";
        } else {
            show_hidden_file_setting = "0";
        }
        scatola_file_set_metadata (viewed_file,
                                SCATOLA_METADATA_KEY_WINDOW_SHOW_HIDDEN_FILES,
                                NULL,
                                show_hidden_file_setting);
    }
}

static void
scatola_spatial_window_show (GtkWidget *widget)
{
    ScatolaWindow *window;
    ScatolaWindowSlot *slot;

    window = SCATOLA_WINDOW (widget);
    slot = scatola_window_get_active_slot (window);

    GTK_WIDGET_CLASS (scatola_spatial_window_parent_class)->show (widget);

    if (slot != NULL && slot->query_editor != NULL)
    {
        scatola_query_editor_grab_focus (SCATOLA_QUERY_EDITOR (slot->query_editor));
    }
}

static void
action_close_parent_folders_callback (GtkAction *action,
                                      gpointer user_data)
{
    scatola_application_close_parent_windows (SCATOLA_SPATIAL_WINDOW (user_data));
}

static void
action_close_all_folders_callback (GtkAction *action,
                                   gpointer user_data)
{
    scatola_application_close_all_spatial_windows ();
}

static void
real_prompt_for_location (ScatolaWindow *window,
                          const char     *initial)
{
    GtkWidget *dialog;

    dialog = scatola_location_dialog_new (window);
    if (initial != NULL)
    {
        scatola_location_dialog_set_location (SCATOLA_LOCATION_DIALOG (dialog),
                                           initial);
    }

    gtk_widget_show (dialog);
}

static ScatolaIconInfo *
real_get_icon (ScatolaWindow *window,
               ScatolaWindowSlot *slot)
{
    return scatola_file_get_icon (slot->viewed_file, 48,
                               SCATOLA_FILE_ICON_FLAGS_IGNORE_VISITING |
                               SCATOLA_FILE_ICON_FLAGS_USE_MOUNT_ICON);
}

static void
sync_window_title (ScatolaWindow *window)
{
    ScatolaWindowSlot *slot;

    slot = scatola_window_get_active_slot (window);

    if (slot->title == NULL || slot->title[0] == '\0')
    {
        gtk_window_set_title (GTK_WINDOW (window), _("Scatola"));
    }
    else
    {
        char *window_title;

        window_title = eel_str_middle_truncate (slot->title, MAX_TITLE_LENGTH);
        gtk_window_set_title (GTK_WINDOW (window), window_title);
        g_free (window_title);
    }
}

static void
real_sync_title (ScatolaWindow *window,
                 ScatolaWindowSlot *slot)
{
    g_assert (slot == scatola_window_get_active_slot (window));

    sync_window_title (window);
}

static void
real_get_min_size (ScatolaWindow *window,
                   guint *min_width, guint *min_height)
{
    if (min_width)
    {
        *min_width = SCATOLA_SPATIAL_WINDOW_MIN_WIDTH;
    }
    if (min_height)
    {
        *min_height = SCATOLA_SPATIAL_WINDOW_MIN_HEIGHT;
    }
}

static void
real_get_default_size (ScatolaWindow *window,
                       guint *default_width, guint *default_height)
{
    if (default_width)
    {
        *default_width = SCATOLA_SPATIAL_WINDOW_DEFAULT_WIDTH;
    }
    if (default_height)
    {
        *default_height = SCATOLA_SPATIAL_WINDOW_DEFAULT_HEIGHT;
    }
}

static void
real_sync_allow_stop (ScatolaWindow *window,
                      ScatolaWindowSlot *slot)
{
}

static void
real_set_allow_up (ScatolaWindow *window, gboolean allow)
{
    ScatolaSpatialWindow *spatial;
    GtkAction *action;

    spatial = SCATOLA_SPATIAL_WINDOW (window);

    action = gtk_action_group_get_action (spatial->details->spatial_action_group,
                                          SPATIAL_ACTION_CLOSE_PARENT_FOLDERS);
    gtk_action_set_sensitive (action, allow);

    SCATOLA_WINDOW_CLASS (scatola_spatial_window_parent_class)->set_allow_up (window, allow);
}

static ScatolaWindowSlot *
real_open_slot (ScatolaWindowPane *pane,
                ScatolaWindowOpenSlotFlags flags)
{
    ScatolaWindowSlot *slot;
    GList *slots;

    g_assert (scatola_window_get_active_slot (pane->window) == NULL);

    slots = scatola_window_get_slots (pane->window);
    g_assert (slots == NULL);
    g_list_free (slots);

    slot = g_object_new (SCATOLA_TYPE_WINDOW_SLOT, NULL);
    slot->pane = pane;
    gtk_container_add (GTK_CONTAINER (SCATOLA_SPATIAL_WINDOW (pane->window)->details->content_box),
                       slot->content_box);
    gtk_widget_show (slot->content_box);
    return slot;
}

static void
save_spatial_data (ScatolaSpatialWindow *window,
		   ScatolaWindowSlot *slot)
{
    scatola_spatial_window_save_geometry (window, slot->viewed_file);
    scatola_spatial_window_save_scroll_position (window, slot);
    scatola_spatial_window_save_show_hidden_files_mode (window, slot->viewed_file);
}

static void
real_close_slot (ScatolaWindowPane *pane,
                 ScatolaWindowSlot *slot)
{
    ScatolaSpatialWindow *window;

    window = SCATOLA_SPATIAL_WINDOW (pane->window);

    /* Save spatial data for close if we didn't already */
    if (!window->details->saved_data_on_close) {
        save_spatial_data (window, slot);
    }

    SCATOLA_WINDOW_CLASS (scatola_spatial_window_parent_class)->close_slot (pane, slot);
}

static void
real_window_close (ScatolaWindow *window)
{
    ScatolaWindowSlot *slot;
    ScatolaSpatialWindow *self;

    self = SCATOLA_SPATIAL_WINDOW (window);

    /* We're closing the window, save the geometry. */
    /* Note that we do this in window close, not slot close, because slot
     * close is too late, by then the widgets have been unrealized.
     * This is for the close by WM case, if you're closing via Ctrl-W that
     * means we close the slots first and this is not an issue */
    slot = scatola_window_get_active_slot (window);

    if (slot != NULL) {
        save_spatial_data (self, slot);
        self->details->saved_data_on_close = TRUE;
    }

    if (SCATOLA_WINDOW_CLASS (scatola_spatial_window_parent_class)->close != NULL) {
        SCATOLA_WINDOW_CLASS (scatola_spatial_window_parent_class)->close (window);
    }
}

static void
location_menu_item_activated_callback (GtkWidget *menu_item,
                                       ScatolaWindow *window)
{
    ScatolaWindowSlot *slot;
    GFile *current;
    GFile *dest;
    GdkEvent *event;

	slot = scatola_window_get_active_slot (window);
	current = scatola_window_slot_get_location (slot);
	dest = g_object_get_data (G_OBJECT (menu_item), "location");

    event = gtk_get_current_event();

    if (!g_file_equal (current, dest))
    {
        GFile *child;
        gboolean close_behind;
        GList *selection;

        close_behind = FALSE;
        selection = NULL;

        child = g_object_get_data (G_OBJECT(menu_item), "child_location");
        if (child != NULL) {
            selection = g_list_prepend (NULL, g_object_ref (child));
        }

        if (event != NULL && ((GdkEventAny *) event)->type == GDK_BUTTON_RELEASE &&
                (((GdkEventButton *) event)->button == 2 ||
                 (((GdkEventButton *) event)->state & GDK_SHIFT_MASK) != 0)) {
            close_behind = TRUE;
        }

        scatola_window_slot_open_location_with_selection
        (slot, dest, selection, close_behind);

    	g_list_free_full (selection, g_object_unref);
    }

    if (event != NULL) {
        gdk_event_free (event);
    }

    g_object_unref (current);
}

static void
got_file_info_for_location_menu_callback (ScatolaFile *file,
        gpointer callback_data)
{
    GtkWidget *menu_item = callback_data;
    GtkWidget *label;
    GtkWidget *icon;
    GdkPixbuf *pixbuf;
    char *name;

    g_return_if_fail (SCATOLA_IS_FILE (file));

    pixbuf = NULL;

    name = scatola_file_get_display_name (file);
    label = gtk_bin_get_child (GTK_BIN (menu_item));
    gtk_label_set_label (GTK_LABEL (label), name);
    g_free (name);

    pixbuf = scatola_file_get_icon_pixbuf (file,
                                        scatola_get_icon_size_for_stock_size (GTK_ICON_SIZE_MENU),
                                        TRUE,
                                        SCATOLA_FILE_ICON_FLAGS_IGNORE_VISITING);

    if (pixbuf != NULL)
    {
        icon = gtk_image_new_from_pixbuf (pixbuf);
        g_object_unref (pixbuf);
    }
    else
    {
        icon = gtk_image_new_from_stock (GTK_STOCK_OPEN, GTK_ICON_SIZE_MENU);
    }

    if (icon)
    {
        gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (menu_item), icon);
    }
    g_object_unref (file);
    g_object_unref (menu_item);
}

static void
menu_deactivate_callback (GtkWidget *menu,
                          gpointer   data)
{
    GMainLoop *loop;

    loop = data;

    if (g_main_loop_is_running (loop))
    {
        g_main_loop_quit (loop);
    }
}

static void
menu_popup_pos (GtkMenu   *menu,
                gint      *x,
                gint      *y,
                gboolean  *push_in,
                gpointer	user_data)
{
    GtkWidget *widget;
    GtkRequisition menu_requisition, button_requisition;
    GtkAllocation allocation;

    widget = user_data;

    gtk_widget_get_preferred_size (GTK_WIDGET (menu), &menu_requisition, NULL);
    gtk_widget_get_preferred_size (widget, &button_requisition, NULL);
    gtk_widget_get_allocation (widget, &allocation);

    gdk_window_get_origin (gtk_widget_get_window (widget), x, y);
    *x += allocation.x;
    *y += allocation.y;

    *y -= menu_requisition.height - button_requisition.height;

    *push_in = TRUE;
}

static gboolean
location_button_pressed_callback (GtkWidget      *widget,
                                  GdkEventButton *event,
                                  ScatolaWindow *window)
{
	ScatolaWindowSlot *slot;
    ScatolaView *view;

	slot = scatola_window_get_active_slot (window);
	view = slot->content_view;

    if (event->button == 3 && view != NULL)
    {
        scatola_view_pop_up_location_context_menu (view, event, NULL);
    }

    return FALSE;
}

static void
location_button_clicked_callback (GtkWidget             *widget,
                                  ScatolaSpatialWindow *window)
{
    ScatolaWindowSlot *slot;
    GtkWidget *popup, *menu_item, *first_item = NULL;
    GFile *location;
    GFile *child_location;
    GMainLoop *loop;

    slot = scatola_window_get_active_slot (SCATOLA_WINDOW (window));

    popup = gtk_menu_new ();
    first_item = NULL;

    location = scatola_window_slot_get_location (slot);
    g_return_if_fail (location != NULL);

    child_location = NULL;
    while (location != NULL) {
        ScatolaFile *file;
        char *name;

        file = scatola_file_get (location);

        name = scatola_file_get_display_name (file);
        menu_item = gtk_image_menu_item_new_with_label (name);
        gtk_image_menu_item_set_always_show_image (GTK_IMAGE_MENU_ITEM (menu_item), TRUE);
        g_free (name);

        if (first_item == NULL) {
            first_item = menu_item;
        }

        g_object_ref (menu_item);
        scatola_file_call_when_ready (file,
                                   SCATOLA_FILE_ATTRIBUTE_INFO,
                                   got_file_info_for_location_menu_callback,
                                   menu_item);

        gtk_widget_show (menu_item);
        g_signal_connect (menu_item, "activate",
                          G_CALLBACK (location_menu_item_activated_callback),
                          window);

        g_object_set_data_full (G_OBJECT (menu_item),
                                "location",
                                g_object_ref (location),
                                (GDestroyNotify)g_object_unref);

        if (child_location) {
            g_object_set_data_full (G_OBJECT (menu_item),
                                    "child_location",
                                    g_object_ref (child_location),
                                    (GDestroyNotify)g_object_unref);
        }

        gtk_menu_shell_prepend (GTK_MENU_SHELL (popup), menu_item);

        if (child_location) {
            g_object_unref (child_location);
        }
        child_location = location;
        location = g_file_get_parent (location);
    }

    if (child_location) {
        g_object_unref (child_location);
    }
    if (location) {
        g_object_unref (location);
    }

    gtk_menu_set_screen (GTK_MENU (popup), gtk_widget_get_screen (widget));

    loop = g_main_loop_new (NULL, FALSE);

    g_signal_connect (popup, "deactivate",
                      G_CALLBACK (menu_deactivate_callback),
                      loop);

    gtk_grab_add (popup);
    gtk_menu_popup (GTK_MENU (popup), NULL, NULL, menu_popup_pos, widget, 1, GDK_CURRENT_TIME);
    gtk_menu_shell_select_item (GTK_MENU_SHELL (popup), first_item);
    g_main_loop_run (loop);
    gtk_grab_remove (popup);
    g_main_loop_unref (loop);
    g_object_ref_sink (popup);
    g_object_unref (popup);
}

static int
get_dnd_icon_size (ScatolaSpatialWindow *window)
{
	ScatolaWindowSlot *active_slot;
    ScatolaView *view;
    ScatolaZoomLevel zoom_level;

	active_slot = scatola_window_get_active_slot (SCATOLA_WINDOW (window));
	view = active_slot->content_view;

    if (view == NULL)
    {
        return SCATOLA_ICON_SIZE_STANDARD;
    }
    else
    {
        zoom_level = scatola_view_get_zoom_level (view);
        return scatola_get_icon_size_for_zoom_level (zoom_level);
    }
}

static void
location_button_drag_begin_callback (GtkWidget             *widget,
                                     GdkDragContext        *context,
                                     ScatolaSpatialWindow *window)
{
    ScatolaWindowSlot *slot;
    GdkPixbuf *pixbuf;

    slot = SCATOLA_WINDOW (window)->details->active_pane->active_slot;

    pixbuf = scatola_file_get_icon_pixbuf (slot->viewed_file,
                                        get_dnd_icon_size (window),
                                        FALSE,
                                        SCATOLA_FILE_ICON_FLAGS_IGNORE_VISITING | SCATOLA_FILE_ICON_FLAGS_FOR_DRAG_ACCEPT);

    gtk_drag_set_icon_pixbuf (context, pixbuf, 0, 0);

    g_object_unref (pixbuf);
}

/* build GDE2 icon list, which only contains the window's URI.
 * If we just used URIs, moving the folder to trash
 * wouldn't work */
static void
get_data_binder (ScatolaDragEachSelectedItemDataGet iteratee,
                 gpointer                            iterator_context,
                 gpointer                            data)
{
    ScatolaSpatialWindow *window;
    ScatolaWindowSlot *slot;
    char *location;
    int icon_size;

    g_assert (SCATOLA_IS_SPATIAL_WINDOW (iterator_context));
    window = SCATOLA_SPATIAL_WINDOW (iterator_context);

    slot = SCATOLA_WINDOW (window)->details->active_pane->active_slot;

    location = scatola_window_slot_get_location_uri (slot);
    icon_size = get_dnd_icon_size (window);

    iteratee (location,
              0,
              0,
              icon_size,
              icon_size,
              data);

    g_free (location);
}

static void
location_button_drag_data_get_callback (GtkWidget             *widget,
                                        GdkDragContext        *context,
                                        GtkSelectionData      *selection_data,
                                        guint                  info,
                                        guint                  time,
                                        ScatolaSpatialWindow *window)
{
    scatola_drag_drag_data_get (widget, context, selection_data,
                             info, time, window, get_data_binder);
}

void
scatola_spatial_window_set_location_button  (ScatolaSpatialWindow *window,
        GFile                 *location)
{
    if (location != NULL)
    {
        ScatolaFile *file;
        char *name;
        GError *error;

        file = scatola_file_get (location);

        /* FIXME: monitor for name change... */
        name = scatola_file_get_display_name (file);
        gtk_label_set_label (GTK_LABEL (window->details->location_label),
                             name);
        g_free (name);
        gtk_widget_set_sensitive (window->details->location_button, TRUE);

        error = scatola_file_get_file_info_error (file);
        if (error == NULL)
        {
            GdkPixbuf *pixbuf;

            pixbuf = scatola_file_get_icon_pixbuf (file,
                                                scatola_get_icon_size_for_stock_size (GTK_ICON_SIZE_MENU),
                                                TRUE,
                                                SCATOLA_FILE_ICON_FLAGS_IGNORE_VISITING);

            if (pixbuf != NULL)
            {
                gtk_image_set_from_pixbuf (GTK_IMAGE (window->details->location_icon),  pixbuf);
                g_object_unref (pixbuf);
            }
            else
            {
                gtk_image_set_from_stock (GTK_IMAGE (window->details->location_icon),
                                          GTK_STOCK_OPEN, GTK_ICON_SIZE_MENU);
            }
        }
        g_object_unref (file);

    }
    else
    {
        gtk_label_set_label (GTK_LABEL (window->details->location_label),
                             "");
        gtk_widget_set_sensitive (window->details->location_button, FALSE);
    }
}

static void
action_go_to_location_callback (GtkAction *action,
                                gpointer user_data)
{
    ScatolaWindow *window;

    window = SCATOLA_WINDOW (user_data);

    scatola_window_prompt_for_location (window, NULL);
}

static void
action_add_bookmark_callback (GtkAction *action,
                              gpointer user_data)
{
    ScatolaWindow *window;

    window = SCATOLA_WINDOW (user_data);

    if (!SCATOLA_IS_DESKTOP_WINDOW (window))   /* don't bookmark x-scatola-desktop:/// */
    {
        scatola_window_add_bookmark_for_current_location (window);
    }
}

static void
action_edit_bookmarks_callback (GtkAction *action,
                                gpointer user_data)
{
    scatola_window_edit_bookmarks (SCATOLA_WINDOW (user_data));
}

static void
action_search_callback (GtkAction *action,
                        gpointer user_data)
{
    ScatolaWindow *window;
    char *uri;
    GFile *f;

    window = SCATOLA_WINDOW (user_data);

    uri = scatola_search_directory_generate_new_uri ();
    f = g_file_new_for_uri (uri);
    scatola_window_go_to (window, f);
    g_object_unref (f);
    g_free (uri);
}

static const GtkActionEntry spatial_entries[] =
{
    /* name, stock id, label */  { SPATIAL_ACTION_PLACES, NULL, N_("_Places") },
    /* name, stock id, label */  {
        SPATIAL_ACTION_GO_TO_LOCATION, NULL, N_("Open _Location..."),
        "<control>L", N_("Specify a location to open"),
        G_CALLBACK (action_go_to_location_callback)
    },
    /* name, stock id, label */  {
        SPATIAL_ACTION_CLOSE_PARENT_FOLDERS, NULL, N_("Close P_arent Folders"),
        "<control><shift>W", N_("Close this folder's parents"),
        G_CALLBACK (action_close_parent_folders_callback)
    },
    /* name, stock id, label */  {
        SPATIAL_ACTION_CLOSE_ALL_FOLDERS, NULL, N_("Clos_e All Folders"),
        "<control>Q", N_("Close all folder windows"),
        G_CALLBACK (action_close_all_folders_callback)
    },
    /* name, stock id, label */  { "Add Bookmark", GTK_STOCK_ADD, N_("_Add Bookmark"),
        "<control>d", N_("Add a bookmark for the current location to this menu"),
        G_CALLBACK (action_add_bookmark_callback)
    },
    /* name, stock id, label */  { "Edit Bookmarks", NULL, N_("_Edit Bookmarks..."),
        "<control>b", N_("Display a window that allows editing the bookmarks in this menu"),
        G_CALLBACK (action_edit_bookmarks_callback)
    },
    /* name, stock id, label */  { "Search", "gtk-find", N_("_Search for Files..."),
        "<control>F", N_("Locate documents and folders on this computer by name or content"),
        G_CALLBACK (action_search_callback)
    },
};

static const char* icon_entries[] =
{
    "/MenuBar/Other Menus/Places/Home",
    "/MenuBar/Other Menus/Places/Go to Computer",
    "/MenuBar/Other Menus/Places/Go to Templates",
    "/MenuBar/Other Menus/Places/Go to Trash",
    "/MenuBar/Other Menus/Places/Go to Network"
};

static void
scatola_spatial_window_init (ScatolaSpatialWindow *window)
{
#if !GTK_CHECK_VERSION (3, 0, 0)
    GtkRcStyle *rc_style;
#endif
    GtkWidget *arrow;
    GtkWidget *hbox, *vbox;
    GtkActionGroup *action_group;
    GtkUIManager *ui_manager;
    GtkTargetList *targets;
    const char *ui;
    int i;
    GtkWidget *menuitem;
    ScatolaWindow *win;
    ScatolaWindowPane *pane;

    window->details = G_TYPE_INSTANCE_GET_PRIVATE (window,
                      SCATOLA_TYPE_SPATIAL_WINDOW,
                      ScatolaSpatialWindowDetails);

    win = SCATOLA_WINDOW (window);

#if GTK_CHECK_VERSION(3, 0, 0)
    gtk_widget_set_hexpand (win->details->statusbar, TRUE);
    gtk_grid_attach (GTK_GRID (win->details->grid),
                     win->details->statusbar,
                     0, 5, 1, 1);
    gtk_widget_show (win->details->statusbar);

    pane = scatola_window_pane_new (win);
    win->details->panes = g_list_prepend (win->details->panes, pane);

    vbox = gtk_vbox_new (FALSE, 0);
    gtk_widget_set_hexpand (vbox, TRUE);
    gtk_widget_set_vexpand (vbox, TRUE);
    gtk_grid_attach (GTK_GRID (SCATOLA_WINDOW (window)->details->grid),
                     vbox,
                     0, 1, 1, 3);
#else
    gtk_table_attach (GTK_TABLE (win->details->table),
                      win->details->statusbar,
                      /* X direction */                   /* Y direction */
                      0, 1,                               5, 6,
                      GTK_EXPAND | GTK_FILL | GTK_SHRINK, 0,
                      0,                                  0);
    gtk_widget_show (win->details->statusbar);

    pane = scatola_window_pane_new (win);
    win->details->panes = g_list_prepend (win->details->panes, pane);

    vbox = gtk_vbox_new (FALSE, 0);
    gtk_table_attach (GTK_TABLE (SCATOLA_WINDOW (window)->details->table),
                      vbox,
                      /* X direction */                   /* Y direction */
                      0, 1,                               1, 4,
                      GTK_EXPAND | GTK_FILL | GTK_SHRINK, GTK_EXPAND | GTK_FILL | GTK_SHRINK,
                      0,                                  0);
#endif
    gtk_widget_show (vbox);
    window->details->content_box = vbox;

    window->details->location_button = gtk_button_new ();
    g_signal_connect (window->details->location_button,
                      "button-press-event",
                      G_CALLBACK (location_button_pressed_callback),
                      window);
    gtk_button_set_relief (GTK_BUTTON (window->details->location_button),
                           GTK_RELIEF_NORMAL);
#if !GTK_CHECK_VERSION (3, 0, 0)
    rc_style = gtk_widget_get_modifier_style (window->details->location_button);
    rc_style->xthickness = 0;
    rc_style->ythickness = 0;
    gtk_widget_modify_style (window->details->location_button,
                             rc_style);
#endif

    gtk_widget_show (window->details->location_button);
    hbox = gtk_hbox_new (FALSE, 3);
    gtk_container_add (GTK_CONTAINER (window->details->location_button),
                       hbox);
    gtk_widget_show (hbox);

    window->details->location_icon = gtk_image_new_from_stock (GTK_STOCK_OPEN, GTK_ICON_SIZE_MENU);
    gtk_box_pack_start (GTK_BOX (hbox), window->details->location_icon, FALSE, FALSE, 0);
    gtk_widget_show (window->details->location_icon);

    window->details->location_label = gtk_label_new ("");
    gtk_label_set_ellipsize (GTK_LABEL (window->details->location_label), PANGO_ELLIPSIZE_END);
    gtk_label_set_max_width_chars (GTK_LABEL (window->details->location_label), MAX_SHORTNAME_PATH);
    gtk_box_pack_start (GTK_BOX (hbox), window->details->location_label,
                        FALSE, FALSE, 0);
    gtk_widget_show (window->details->location_label);

#if GTK_CHECK_VERSION (3, 0, 0)
    arrow = gtk_image_new_from_icon_name ("pan-down-symbolic", GTK_ICON_SIZE_BUTTON);
#else
    arrow = gtk_arrow_new (GTK_ARROW_DOWN, GTK_SHADOW_NONE);
#endif
    gtk_box_pack_start (GTK_BOX (hbox), arrow, FALSE, FALSE, 0);
    gtk_widget_show (arrow);

    gtk_drag_source_set (window->details->location_button,
                         GDK_BUTTON1_MASK | GDK_BUTTON2_MASK, location_button_drag_types,
                         G_N_ELEMENTS (location_button_drag_types),
                         GDK_ACTION_MOVE | GDK_ACTION_COPY | GDK_ACTION_LINK | GDK_ACTION_ASK);
    g_signal_connect (window->details->location_button,
                      "drag_begin",
                      G_CALLBACK (location_button_drag_begin_callback),
                      window);
    g_signal_connect (window->details->location_button,
                      "drag_data_get",
                      G_CALLBACK (location_button_drag_data_get_callback),
                      window);

    targets = gtk_drag_source_get_target_list (window->details->location_button);
    gtk_target_list_add_text_targets (targets, SCATOLA_ICON_DND_TEXT);

    gtk_widget_set_sensitive (window->details->location_button, FALSE);
    g_signal_connect (window->details->location_button,
                      "clicked",
                      G_CALLBACK (location_button_clicked_callback), window);
    gtk_box_pack_start (GTK_BOX (SCATOLA_WINDOW (window)->details->statusbar),
                        window->details->location_button,
                        FALSE, TRUE, 0);

    gtk_box_reorder_child (GTK_BOX (SCATOLA_WINDOW (window)->details->statusbar),
                           window->details->location_button, 0);

    action_group = gtk_action_group_new ("SpatialActions");
    gtk_action_group_set_translation_domain (action_group, GETTEXT_PACKAGE);
    window->details->spatial_action_group = action_group;
    gtk_action_group_add_actions (action_group,
                                  spatial_entries, G_N_ELEMENTS (spatial_entries),
                                  window);

    ui_manager = scatola_window_get_ui_manager (SCATOLA_WINDOW (window));
    gtk_ui_manager_insert_action_group (ui_manager, action_group, 0);
    g_object_unref (action_group); /* owned by ui manager */

    ui = scatola_ui_string_get ("scatola-spatial-window-ui.xml");
    gtk_ui_manager_add_ui_from_string (ui_manager, ui, -1, NULL);

    for (i = 0; i < G_N_ELEMENTS (icon_entries); i++)
    {
        menuitem = gtk_ui_manager_get_widget (ui_manager, icon_entries[i]);
        gtk_image_menu_item_set_always_show_image (GTK_IMAGE_MENU_ITEM (menuitem), TRUE);
    }
    scatola_window_set_active_pane (win, pane);
}

static void
scatola_spatial_window_class_init (ScatolaSpatialWindowClass *klass)
{
    GtkBindingSet *binding_set;
	ScatolaWindowClass *nclass = SCATOLA_WINDOW_CLASS (klass);
	GtkWidgetClass *wclass = GTK_WIDGET_CLASS (klass);

	nclass->window_type = SCATOLA_WINDOW_SPATIAL;
	nclass->bookmarks_placeholder = MENU_PATH_SPATIAL_BOOKMARKS_PLACEHOLDER;
	nclass->prompt_for_location = real_prompt_for_location;
	nclass->get_icon = real_get_icon;
	nclass->sync_title = real_sync_title;
	nclass->get_min_size = real_get_min_size;
	nclass->get_default_size = real_get_default_size;
	nclass->sync_allow_stop = real_sync_allow_stop;
	nclass->set_allow_up = real_set_allow_up;
	nclass->open_slot = real_open_slot;
	nclass->close = real_window_close;
	nclass->close_slot = real_close_slot;

	wclass->show = scatola_spatial_window_show;
	wclass->configure_event = scatola_spatial_window_configure_event;
	wclass->unrealize = scatola_spatial_window_unrealize;
	wclass->window_state_event = scatola_spatial_window_state_event;

	G_OBJECT_CLASS (klass)->finalize = scatola_spatial_window_finalize;

	binding_set = gtk_binding_set_by_class (klass);
	gtk_binding_entry_add_signal (binding_set, GDK_KEY_BackSpace, GDK_SHIFT_MASK,
                                  "go_up", 1,
                                  G_TYPE_BOOLEAN, TRUE);
	gtk_binding_entry_add_signal (binding_set, GDK_KEY_Up, GDK_SHIFT_MASK | GDK_MOD1_MASK,
                                  "go_up", 1,
                                  G_TYPE_BOOLEAN, TRUE);

	g_type_class_add_private (klass, sizeof(ScatolaSpatialWindowDetails));
}
