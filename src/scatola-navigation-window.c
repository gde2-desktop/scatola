/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */

/*
 *  Scatola
 *
 *  Copyright (C) 1999, 2000 Red Hat, Inc.
 *  Copyright (C) 1999, 2000, 2001 Eazel, Inc.
 *  Copyright (C) 2003 Ximian, Inc.
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
#include "scatola-window-private.h"

#include "scatola-actions.h"
#include "scatola-application.h"
#include "scatola-bookmarks-window.h"
#include "scatola-main.h"
#include "scatola-location-bar.h"
#include "scatola-query-editor.h"
#include "scatola-search-bar.h"
#include "scatola-navigation-window-slot.h"
#include "scatola-notebook.h"
#include "scatola-window-manage-views.h"
#include "scatola-navigation-window-pane.h"
#include <eel/eel-gtk-extensions.h>
#include <eel/eel-gtk-macros.h>
#include <eel/eel-string.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <gdk/gdkx.h>
#include <gtk/gtk.h>
#include <glib/gi18n.h>
#ifdef HAVE_X11_XF86KEYSYM_H
#include <X11/XF86keysym.h>
#endif
#include <libscatola-private/scatola-file-utilities.h>
#include <libscatola-private/scatola-file-attributes.h>
#include <libscatola-private/scatola-global-preferences.h>
#include <libscatola-private/scatola-icon-info.h>
#include <libscatola-private/scatola-metadata.h>
#include <libscatola-private/scatola-mime-actions.h>
#include <libscatola-private/scatola-program-choosing.h>
#include <libscatola-private/scatola-sidebar.h>
#include <libscatola-private/scatola-view-factory.h>
#include <libscatola-private/scatola-clipboard.h>
#include <libscatola-private/scatola-module.h>
#include <libscatola-private/scatola-sidebar-provider.h>
#include <libscatola-private/scatola-search-directory.h>
#include <libscatola-private/scatola-signaller.h>
#include <math.h>
#include <sys/time.h>

#define MAX_TITLE_LENGTH 180

#define MENU_PATH_BOOKMARKS_PLACEHOLDER			"/MenuBar/Other Menus/Bookmarks/Bookmarks Placeholder"

/* Forward and back buttons on the mouse */
static gboolean mouse_extra_buttons = TRUE;
static int mouse_forward_button = 9;
static int mouse_back_button = 8;

static void add_sidebar_panels                       (ScatolaNavigationWindow *window);
static void side_panel_image_changed_callback        (ScatolaSidebar          *side_panel,
        gpointer                  callback_data);
static void always_use_location_entry_changed        (gpointer                  callback_data);
static void always_use_browser_changed               (gpointer                  callback_data);
static void mouse_back_button_changed		     (gpointer                  callback_data);
static void mouse_forward_button_changed	     (gpointer                  callback_data);
static void use_extra_mouse_buttons_changed          (gpointer                  callback_data);
static ScatolaWindowSlot *create_extra_pane         (ScatolaNavigationWindow *window);


G_DEFINE_TYPE (ScatolaNavigationWindow, scatola_navigation_window, SCATOLA_TYPE_WINDOW)
#define parent_class scatola_navigation_window_parent_class

#if GTK_CHECK_VERSION (3, 0, 0)
#define gtk_vbox_new(X,Y) gtk_box_new(GTK_ORIENTATION_VERTICAL,Y)
#endif

static const struct
{
    unsigned int keyval;
    const char *action;
} extra_navigation_window_keybindings [] =
{
#ifdef HAVE_X11_XF86KEYSYM_H
    { XF86XK_Back,		SCATOLA_ACTION_BACK },
    { XF86XK_Forward,	SCATOLA_ACTION_FORWARD }
#endif
};

static void
scatola_navigation_window_init (ScatolaNavigationWindow *window)
{
    GtkUIManager *ui_manager;
    GtkWidget *toolbar;
    ScatolaWindow *win;
    ScatolaNavigationWindowPane *pane;
    GtkWidget *hpaned;
    GtkWidget *vbox;

    win = SCATOLA_WINDOW (window);

    window->details = G_TYPE_INSTANCE_GET_PRIVATE (window, SCATOLA_TYPE_NAVIGATION_WINDOW, ScatolaNavigationWindowDetails);

#if GTK_CHECK_VERSION(3, 0, 0)
    GtkStyleContext *context;

    context = gtk_widget_get_style_context (GTK_WIDGET (window));
    gtk_style_context_add_class (context, "scatola-navigation-window");
#endif

    pane = scatola_navigation_window_pane_new (win);
    win->details->panes = g_list_prepend (win->details->panes, pane);

    window->details->header_size_group = gtk_size_group_new (GTK_SIZE_GROUP_VERTICAL);
    gtk_size_group_set_ignore_hidden (window->details->header_size_group, FALSE);

#if GTK_CHECK_VERSION(3, 0, 0)
    window->details->content_paned = gtk_paned_new (GTK_ORIENTATION_HORIZONTAL);
    gtk_widget_set_hexpand (window->details->content_paned, TRUE);
    gtk_widget_set_vexpand (window->details->content_paned, TRUE);
    gtk_grid_attach (GTK_GRID (SCATOLA_WINDOW (window)->details->grid),
                     window->details->content_paned,
                     0, 3, 1, 1);
#else
    window->details->content_paned = gtk_hpaned_new ();
    gtk_table_attach (GTK_TABLE (SCATOLA_WINDOW (window)->details->table),
                      window->details->content_paned,
                      /* X direction */                   /* Y direction */
                      0, 1,                               3, 4,
                      GTK_EXPAND | GTK_FILL | GTK_SHRINK, GTK_EXPAND | GTK_FILL | GTK_SHRINK,
                      0,                                  0);
#endif
    gtk_widget_show (window->details->content_paned);

    vbox = gtk_vbox_new (FALSE, 0);
    gtk_paned_pack2 (GTK_PANED (window->details->content_paned), vbox,
    		     TRUE, FALSE);
    gtk_widget_show (vbox);

#if GTK_CHECK_VERSION(3, 0, 0)    
    hpaned = gtk_paned_new (GTK_ORIENTATION_HORIZONTAL);
#else
    hpaned = gtk_hpaned_new ();
#endif
    gtk_box_pack_start (GTK_BOX (vbox), hpaned, TRUE, TRUE, 0);
    gtk_widget_show (hpaned);
    window->details->split_view_hpane = hpaned;

    gtk_box_pack_start (GTK_BOX (vbox), win->details->statusbar, FALSE, FALSE, 0);
    gtk_widget_show (win->details->statusbar);

    scatola_navigation_window_pane_setup (pane);

    gtk_paned_pack1 (GTK_PANED(hpaned), pane->widget, TRUE, FALSE);
    gtk_widget_show (pane->widget);

    /* this has to be done after the location bar has been set up,
     * but before menu stuff is being called */
    scatola_window_set_active_pane (win, SCATOLA_WINDOW_PANE (pane));

    scatola_navigation_window_initialize_actions (window);

    scatola_navigation_window_initialize_menus (window);

    ui_manager = scatola_window_get_ui_manager (SCATOLA_WINDOW (window));
    toolbar = gtk_ui_manager_get_widget (ui_manager, "/Toolbar");
#if GTK_CHECK_VERSION(3, 0, 0)
    gtk_style_context_add_class (gtk_widget_get_style_context (toolbar), GTK_STYLE_CLASS_PRIMARY_TOOLBAR);
    window->details->toolbar = toolbar;
    gtk_widget_set_hexpand (toolbar, TRUE);
    gtk_grid_attach (GTK_GRID (SCATOLA_WINDOW (window)->details->grid),
                     toolbar,
                     0, 1, 1, 1);
#else
    window->details->toolbar = toolbar;
    gtk_table_attach (GTK_TABLE (SCATOLA_WINDOW (window)->details->table),
                      toolbar,
                      /* X direction */                   /* Y direction */
                      0, 1,                               1, 2,
                      GTK_EXPAND | GTK_FILL | GTK_SHRINK, 0,
                      0,                                  0);
#endif
    gtk_widget_show (toolbar);

    scatola_navigation_window_initialize_toolbars (window);

    /* Set initial sensitivity of some buttons & menu items
     * now that they're all created.
     */
    scatola_navigation_window_allow_back (window, FALSE);
    scatola_navigation_window_allow_forward (window, FALSE);

    g_signal_connect_swapped (scatola_preferences,
                              "changed::" SCATOLA_PREFERENCES_ALWAYS_USE_LOCATION_ENTRY,
                              G_CALLBACK(always_use_location_entry_changed),
                              window);

    g_signal_connect_swapped (scatola_preferences,
                              "changed::" SCATOLA_PREFERENCES_ALWAYS_USE_BROWSER,
                              G_CALLBACK(always_use_browser_changed),
                              window);
}

static void
always_use_location_entry_changed (gpointer callback_data)
{
    ScatolaNavigationWindow *window;
    GList *walk;
    gboolean use_entry;

    window = SCATOLA_NAVIGATION_WINDOW (callback_data);

    use_entry = g_settings_get_boolean (scatola_preferences, SCATOLA_PREFERENCES_ALWAYS_USE_LOCATION_ENTRY);

    for (walk = SCATOLA_WINDOW(window)->details->panes; walk; walk = walk->next)
    {
        scatola_navigation_window_pane_always_use_location_entry (walk->data, use_entry);
    }
}

static void
always_use_browser_changed (gpointer callback_data)
{
    ScatolaNavigationWindow *window;

    window = SCATOLA_NAVIGATION_WINDOW (callback_data);

    scatola_navigation_window_update_spatial_menu_item (window);
}

/* Sanity check: highest mouse button value I could find was 14. 5 is our
 * lower threshold (well-documented to be the one of the button events for the
 * scrollwheel), so it's hardcoded in the functions below. However, if you have
 * a button that registers higher and want to map it, file a bug and
 * we'll move the bar. Makes you wonder why the X guys don't have
 * defined values for these like the XKB stuff, huh?
 */
#define UPPER_MOUSE_LIMIT 14

static void
mouse_back_button_changed (gpointer callback_data)
{
    int new_back_button;

    new_back_button = g_settings_get_int (scatola_preferences, SCATOLA_PREFERENCES_MOUSE_BACK_BUTTON);

    /* Bounds checking */
    if (new_back_button < 6 || new_back_button > UPPER_MOUSE_LIMIT)
        return;

    mouse_back_button = new_back_button;
}

static void
mouse_forward_button_changed (gpointer callback_data)
{
    int new_forward_button;

    new_forward_button = g_settings_get_int (scatola_preferences, SCATOLA_PREFERENCES_MOUSE_FORWARD_BUTTON);

    /* Bounds checking */
    if (new_forward_button < 6 || new_forward_button > UPPER_MOUSE_LIMIT)
        return;

    mouse_forward_button = new_forward_button;
}

static void
use_extra_mouse_buttons_changed (gpointer callback_data)
{
    mouse_extra_buttons = g_settings_get_boolean (scatola_preferences, SCATOLA_PREFERENCES_MOUSE_USE_EXTRA_BUTTONS);
}

void
scatola_navigation_window_unset_focus_widget (ScatolaNavigationWindow *window)
{
    if (window->details->last_focus_widget != NULL)
    {
        g_object_remove_weak_pointer (G_OBJECT (window->details->last_focus_widget),
                                      (gpointer *) &window->details->last_focus_widget);
        window->details->last_focus_widget = NULL;
    }
}

gboolean
scatola_navigation_window_is_in_temporary_navigation_bar (GtkWidget *widget,
        ScatolaNavigationWindow *window)
{
    GList *walk;
    gboolean is_in_any = FALSE;

    for (walk = SCATOLA_WINDOW(window)->details->panes; walk; walk = walk->next)
    {
        ScatolaNavigationWindowPane *pane = walk->data;
        if(gtk_widget_get_ancestor (widget, SCATOLA_TYPE_LOCATION_BAR) != NULL &&
                pane->temporary_navigation_bar)
            is_in_any = TRUE;
    }
    return is_in_any;
}

gboolean
scatola_navigation_window_is_in_temporary_search_bar (GtkWidget *widget,
        ScatolaNavigationWindow *window)
{
    GList *walk;
    gboolean is_in_any = FALSE;

    for (walk = SCATOLA_WINDOW(window)->details->panes; walk; walk = walk->next)
    {
        ScatolaNavigationWindowPane *pane = walk->data;
        if(gtk_widget_get_ancestor (widget, SCATOLA_TYPE_SEARCH_BAR) != NULL &&
                pane->temporary_search_bar)
            is_in_any = TRUE;
    }
    return is_in_any;
}

static void
remember_focus_widget (ScatolaNavigationWindow *window)
{
    ScatolaNavigationWindow *navigation_window;
    GtkWidget *focus_widget;

    navigation_window = SCATOLA_NAVIGATION_WINDOW (window);

    focus_widget = gtk_window_get_focus (GTK_WINDOW (window));
    if (focus_widget != NULL &&
            !scatola_navigation_window_is_in_temporary_navigation_bar (focus_widget, navigation_window) &&
            !scatola_navigation_window_is_in_temporary_search_bar (focus_widget, navigation_window))
    {
        scatola_navigation_window_unset_focus_widget (navigation_window);

        navigation_window->details->last_focus_widget = focus_widget;
        g_object_add_weak_pointer (G_OBJECT (focus_widget),
                                   (gpointer *) &(SCATOLA_NAVIGATION_WINDOW (window)->details->last_focus_widget));
    }
}

void
scatola_navigation_window_restore_focus_widget (ScatolaNavigationWindow *window)
{
    if (window->details->last_focus_widget != NULL)
    {
        if (SCATOLA_IS_VIEW (window->details->last_focus_widget))
        {
            scatola_view_grab_focus (SCATOLA_VIEW (window->details->last_focus_widget));
        }
        else
        {
            gtk_widget_grab_focus (window->details->last_focus_widget);
        }

        scatola_navigation_window_unset_focus_widget (window);
    }
}

static void
side_pane_close_requested_callback (GtkWidget *widget,
                                    gpointer user_data)
{
    ScatolaNavigationWindow *window;

    window = SCATOLA_NAVIGATION_WINDOW (user_data);

    scatola_navigation_window_hide_sidebar (window);
}

static void
side_pane_size_allocate_callback (GtkWidget *widget,
                                  GtkAllocation *allocation,
                                  gpointer user_data)
{
    ScatolaNavigationWindow *window;

    window = SCATOLA_NAVIGATION_WINDOW (user_data);

    if (allocation->width != window->details->side_pane_width)
    {
        window->details->side_pane_width = allocation->width;
        g_settings_set_int (scatola_window_state,
                            SCATOLA_WINDOW_STATE_SIDEBAR_WIDTH,
                            allocation->width <= 1 ? 0 : allocation->width);
    }
}

static void
setup_side_pane_width (ScatolaNavigationWindow *window)
{
    g_return_if_fail (window->sidebar != NULL);

    window->details->side_pane_width =
        g_settings_get_int (scatola_window_state,
                            SCATOLA_WINDOW_STATE_SIDEBAR_WIDTH);

    gtk_paned_set_position (GTK_PANED (window->details->content_paned),
                            window->details->side_pane_width);
}

static void
set_current_side_panel (ScatolaNavigationWindow *window,
                        ScatolaSidebar *panel)
{
    if (window->details->current_side_panel)
    {
        scatola_sidebar_is_visible_changed (window->details->current_side_panel,
                                         FALSE);
        eel_remove_weak_pointer (&window->details->current_side_panel);
    }

    if (panel != NULL)
    {
        scatola_sidebar_is_visible_changed (panel, TRUE);
    }
    window->details->current_side_panel = panel;
    eel_add_weak_pointer (&window->details->current_side_panel);
}

static void
side_pane_switch_page_callback (ScatolaSidePane *side_pane,
                                GtkWidget *widget,
                                ScatolaNavigationWindow *window)
{
    const char *id;
    ScatolaSidebar *sidebar;

    sidebar = SCATOLA_SIDEBAR (widget);

    if (sidebar == NULL)
    {
        return;
    }

    set_current_side_panel (window, sidebar);

    id = scatola_sidebar_get_sidebar_id (sidebar);
    g_settings_set_string (scatola_window_state, SCATOLA_WINDOW_STATE_SIDE_PANE_VIEW, id);
}

static void
scatola_navigation_window_set_up_sidebar (ScatolaNavigationWindow *window)
{
    GtkWidget *title;

    window->sidebar = scatola_side_pane_new ();

    title = scatola_side_pane_get_title (window->sidebar);
    gtk_size_group_add_widget (window->details->header_size_group,
                               title);

    gtk_paned_pack1 (GTK_PANED (window->details->content_paned),
                     GTK_WIDGET (window->sidebar),
                     FALSE, FALSE);

    setup_side_pane_width (window);
    g_signal_connect (window->sidebar,
                      "size_allocate",
                      G_CALLBACK (side_pane_size_allocate_callback),
                      window);

    add_sidebar_panels (window);

    g_signal_connect (window->sidebar,
                      "close_requested",
                      G_CALLBACK (side_pane_close_requested_callback),
                      window);

    g_signal_connect (window->sidebar,
                      "switch_page",
                      G_CALLBACK (side_pane_switch_page_callback),
                      window);

    gtk_widget_show (GTK_WIDGET (window->sidebar));
}

static void
scatola_navigation_window_tear_down_sidebar (ScatolaNavigationWindow *window)
{
    GList *node, *next;
    ScatolaSidebar *sidebar_panel;

    g_signal_handlers_disconnect_by_func (window->sidebar,
                                          side_pane_switch_page_callback,
                                          window);

    for (node = window->sidebar_panels; node != NULL; node = next)
    {
        next = node->next;

        sidebar_panel = SCATOLA_SIDEBAR (node->data);

        scatola_navigation_window_remove_sidebar_panel (window,
                sidebar_panel);
    }

    gtk_widget_destroy (GTK_WIDGET (window->sidebar));
    window->sidebar = NULL;
}

static gboolean
scatola_navigation_window_state_event (GtkWidget *widget,
                                    GdkEventWindowState *event)
{
    if (event->changed_mask & GDK_WINDOW_STATE_MAXIMIZED)
    {
        g_settings_set_boolean (scatola_window_state, SCATOLA_WINDOW_STATE_MAXIMIZED,
                                event->new_window_state & GDK_WINDOW_STATE_MAXIMIZED);
    }

    if (GTK_WIDGET_CLASS (parent_class)->window_state_event != NULL)
    {
        return GTK_WIDGET_CLASS (parent_class)->window_state_event (widget, event);
    }

    return FALSE;
}

static gboolean
scatola_navigation_window_key_press_event (GtkWidget *widget,
                                        GdkEventKey *event)
{
    ScatolaNavigationWindow *window;
    int i;

    window = SCATOLA_NAVIGATION_WINDOW (widget);

    for (i = 0; i < G_N_ELEMENTS (extra_navigation_window_keybindings); i++)
    {
        if (extra_navigation_window_keybindings[i].keyval == event->keyval)
        {
            GtkAction *action;

            action = gtk_action_group_get_action (window->details->navigation_action_group,
                                                  extra_navigation_window_keybindings[i].action);

            g_assert (action != NULL);
            if (gtk_action_is_sensitive (action))
            {
                gtk_action_activate (action);
                return TRUE;
            }

            break;
        }
    }

    return GTK_WIDGET_CLASS (scatola_navigation_window_parent_class)->key_press_event (widget, event);
}

static gboolean
scatola_navigation_window_button_press_event (GtkWidget *widget,
        GdkEventButton *event)
{
    ScatolaNavigationWindow *window;
    gboolean handled;

    handled = FALSE;
    window = SCATOLA_NAVIGATION_WINDOW (widget);

    if (mouse_extra_buttons && (event->button == mouse_back_button))
    {
        scatola_navigation_window_go_back (window);
        handled = TRUE;
    }
    else if (mouse_extra_buttons && (event->button == mouse_forward_button))
    {
        scatola_navigation_window_go_forward (window);
        handled = TRUE;
    }
    else if (GTK_WIDGET_CLASS (scatola_navigation_window_parent_class)->button_press_event)
    {
        handled = GTK_WIDGET_CLASS (scatola_navigation_window_parent_class)->button_press_event (widget, event);
    }
    else
    {
        handled = FALSE;
    }
    return handled;
}

static void
#if GTK_CHECK_VERSION (3, 0, 0)
scatola_navigation_window_destroy (GtkWidget *object)
#else
scatola_navigation_window_destroy (GtkObject *object)
#endif
{
    ScatolaNavigationWindow *window;

    window = SCATOLA_NAVIGATION_WINDOW (object);

    scatola_navigation_window_unset_focus_widget (window);

    window->sidebar = NULL;
    g_list_foreach (window->sidebar_panels, (GFunc)g_object_unref, NULL);
    g_list_free (window->sidebar_panels);
    window->sidebar_panels = NULL;

    window->details->content_paned = NULL;
    window->details->split_view_hpane = NULL;

#if GTK_CHECK_VERSION (3, 0, 0)
    GTK_WIDGET_CLASS (parent_class)->destroy (object);
#else
    GTK_OBJECT_CLASS (parent_class)->destroy (object);
#endif
}

static void
scatola_navigation_window_finalize (GObject *object)
{
    ScatolaNavigationWindow *window;

    window = SCATOLA_NAVIGATION_WINDOW (object);

    scatola_navigation_window_remove_go_menu_callback (window);

    g_signal_handlers_disconnect_by_func (scatola_preferences,
                                          always_use_browser_changed,
                                          window);
    g_signal_handlers_disconnect_by_func (scatola_preferences,
                                          always_use_location_entry_changed,
                                          window);

    G_OBJECT_CLASS (parent_class)->finalize (object);
}

/*
 * Main API
 */

void
scatola_navigation_window_add_sidebar_panel (ScatolaNavigationWindow *window,
        ScatolaSidebar *sidebar_panel)
{
    const char *sidebar_id;
    char *label;
    char *tooltip;
    char *default_id;
    GdkPixbuf *icon;

    g_return_if_fail (SCATOLA_IS_NAVIGATION_WINDOW (window));
    g_return_if_fail (SCATOLA_IS_SIDEBAR (sidebar_panel));
    g_return_if_fail (SCATOLA_IS_SIDE_PANE (window->sidebar));
    g_return_if_fail (g_list_find (window->sidebar_panels, sidebar_panel) == NULL);

    label = scatola_sidebar_get_tab_label (sidebar_panel);
    tooltip = scatola_sidebar_get_tab_tooltip (sidebar_panel);
    scatola_side_pane_add_panel (window->sidebar,
                              GTK_WIDGET (sidebar_panel),
                              label,
                              tooltip);
    g_free (tooltip);
    g_free (label);

    icon = scatola_sidebar_get_tab_icon (sidebar_panel);
    scatola_side_pane_set_panel_image (SCATOLA_NAVIGATION_WINDOW (window)->sidebar,
                                    GTK_WIDGET (sidebar_panel),
                                    icon);
    if (icon)
    {
        g_object_unref (icon);
    }

    g_signal_connect (sidebar_panel, "tab_icon_changed",
                      (GCallback)side_panel_image_changed_callback, window);

    window->sidebar_panels = g_list_prepend (window->sidebar_panels,
                             g_object_ref (sidebar_panel));

    /* Show if default */
    sidebar_id = scatola_sidebar_get_sidebar_id (sidebar_panel);
    default_id = g_settings_get_string (scatola_window_state, SCATOLA_WINDOW_STATE_SIDE_PANE_VIEW);
    if (sidebar_id && default_id && !strcmp (sidebar_id, default_id))
    {
        scatola_side_pane_show_panel (window->sidebar,
                                   GTK_WIDGET (sidebar_panel));
    }
    g_free (default_id);
}

void
scatola_navigation_window_remove_sidebar_panel (ScatolaNavigationWindow *window,
        ScatolaSidebar *sidebar_panel)
{
    g_return_if_fail (SCATOLA_IS_NAVIGATION_WINDOW (window));
    g_return_if_fail (SCATOLA_IS_SIDEBAR (sidebar_panel));

    if (g_list_find (window->sidebar_panels, sidebar_panel) == NULL)
    {
        return;
    }

    g_signal_handlers_disconnect_by_func (sidebar_panel, side_panel_image_changed_callback, window);

    scatola_side_pane_remove_panel (window->sidebar,
                                 GTK_WIDGET (sidebar_panel));
    window->sidebar_panels = g_list_remove (window->sidebar_panels, sidebar_panel);
    g_object_unref (sidebar_panel);
}

void
scatola_navigation_window_go_back (ScatolaNavigationWindow *window)
{
    scatola_navigation_window_back_or_forward (window, TRUE, 0, FALSE);
}

void
scatola_navigation_window_go_forward (ScatolaNavigationWindow *window)
{
    scatola_navigation_window_back_or_forward (window, FALSE, 0, FALSE);
}

void
scatola_navigation_window_allow_back (ScatolaNavigationWindow *window, gboolean allow)
{
    GtkAction *action;

    action = gtk_action_group_get_action (window->details->navigation_action_group,
                                          SCATOLA_ACTION_BACK);

    gtk_action_set_sensitive (action, allow);
}

void
scatola_navigation_window_allow_forward (ScatolaNavigationWindow *window, gboolean allow)
{
    GtkAction *action;

    action = gtk_action_group_get_action (window->details->navigation_action_group,
                                          SCATOLA_ACTION_FORWARD);

    gtk_action_set_sensitive (action, allow);
}

static void
real_sync_title (ScatolaWindow *window,
                 ScatolaWindowSlot *slot)
{
    ScatolaNavigationWindowPane *pane;
    ScatolaNotebook *notebook;
    char *full_title;
    char *window_title;

    EEL_CALL_PARENT (SCATOLA_WINDOW_CLASS,
                     sync_title, (window, slot));

    if (slot == window->details->active_pane->active_slot)
    {
        /* if spatial mode is default, we keep "File Browser" in the window title
         * to recognize browser windows. Otherwise, we default to the directory name.
         */
        if (!g_settings_get_boolean (scatola_preferences, SCATOLA_PREFERENCES_ALWAYS_USE_BROWSER))
        {
            full_title = g_strdup_printf (_("%s - File Browser"), slot->title);
            window_title = eel_str_middle_truncate (full_title, MAX_TITLE_LENGTH);
            g_free (full_title);
        }
        else
        {
            window_title = eel_str_middle_truncate (slot->title, MAX_TITLE_LENGTH);
        }

        gtk_window_set_title (GTK_WINDOW (window), window_title);
        g_free (window_title);
    }

    pane = SCATOLA_NAVIGATION_WINDOW_PANE (slot->pane);
    notebook = SCATOLA_NOTEBOOK (pane->notebook);
    scatola_notebook_sync_tab_label (notebook, slot);
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
real_sync_allow_stop (ScatolaWindow *window,
                      ScatolaWindowSlot *slot)
{
    ScatolaNavigationWindow *navigation_window;
    ScatolaNotebook *notebook;

    navigation_window = SCATOLA_NAVIGATION_WINDOW (window);
    scatola_navigation_window_set_spinner_active (navigation_window, slot->allow_stop);

    notebook = SCATOLA_NOTEBOOK (SCATOLA_NAVIGATION_WINDOW_PANE (slot->pane)->notebook);
    scatola_notebook_sync_loading (notebook, slot);
}

static void
real_prompt_for_location (ScatolaWindow *window, const char *initial)
{
    ScatolaNavigationWindowPane *pane;

    remember_focus_widget (SCATOLA_NAVIGATION_WINDOW (window));

    pane = SCATOLA_NAVIGATION_WINDOW_PANE (window->details->active_pane);

    scatola_navigation_window_pane_show_location_bar_temporarily (pane);
    scatola_navigation_window_pane_show_navigation_bar_temporarily (pane);

    if (initial)
    {
        scatola_location_bar_set_location (SCATOLA_LOCATION_BAR (pane->navigation_bar),
                                        initial);
    }
}

void
scatola_navigation_window_show_search (ScatolaNavigationWindow *window)
{
    ScatolaNavigationWindowPane *pane;

    pane = SCATOLA_NAVIGATION_WINDOW_PANE (SCATOLA_WINDOW (window)->details->active_pane);
    if (!scatola_navigation_window_pane_search_bar_showing (pane))
    {
        remember_focus_widget (window);

        scatola_navigation_window_pane_show_location_bar_temporarily (pane);
        scatola_navigation_window_pane_set_bar_mode (pane, SCATOLA_BAR_SEARCH);
        pane->temporary_search_bar = TRUE;
        scatola_search_bar_clear (SCATOLA_SEARCH_BAR (pane->search_bar));
    }

    scatola_search_bar_grab_focus (SCATOLA_SEARCH_BAR (pane->search_bar));
}

void
scatola_navigation_window_hide_search (ScatolaNavigationWindow *window)
{
    ScatolaNavigationWindowPane *pane = SCATOLA_NAVIGATION_WINDOW_PANE (SCATOLA_WINDOW (window)->details->active_pane);
    if (scatola_navigation_window_pane_search_bar_showing (pane))
    {
        if (scatola_navigation_window_pane_hide_temporary_bars (pane))
        {
            scatola_navigation_window_restore_focus_widget (window);
        }
    }
}

/* This updates the UI state of the search button, but does not
   in itself trigger a search action */
void
scatola_navigation_window_set_search_button (ScatolaNavigationWindow *window,
        gboolean state)
{
    GtkAction *action;

    action = gtk_action_group_get_action (window->details->navigation_action_group,
                                          "Search");

    /* Block callback so we don't activate the action and thus focus the
       search entry */
    g_object_set_data (G_OBJECT (action), "blocked", GINT_TO_POINTER (1));
    gtk_toggle_action_set_active (GTK_TOGGLE_ACTION (action), state);
    g_object_set_data (G_OBJECT (action), "blocked", NULL);
}

static void
side_panel_image_changed_callback (ScatolaSidebar *side_panel,
                                   gpointer callback_data)
{
    ScatolaWindow *window;
    GdkPixbuf *icon;

    window = SCATOLA_WINDOW (callback_data);

    icon = scatola_sidebar_get_tab_icon (side_panel);
    scatola_side_pane_set_panel_image (SCATOLA_NAVIGATION_WINDOW (window)->sidebar,
                                    GTK_WIDGET (side_panel),
                                    icon);
    if (icon != NULL)
    {
        g_object_unref (icon);
    }
}

/**
 * add_sidebar_panels:
 * @window:	A ScatolaNavigationWindow
 *
 * Adds all sidebars available
 *
 */
static void
add_sidebar_panels (ScatolaNavigationWindow *window)
{
    GtkWidget *current;
    GList *providers;
    GList *p;
    ScatolaSidebar *sidebar_panel;

    g_assert (SCATOLA_IS_NAVIGATION_WINDOW (window));

    if (window->sidebar == NULL)
    {
        return;
    }

    providers = scatola_module_get_extensions_for_type (SCATOLA_TYPE_SIDEBAR_PROVIDER);

    for (p = providers; p != NULL; p = p->next)
    {
        ScatolaSidebarProvider *provider;

        provider = SCATOLA_SIDEBAR_PROVIDER (p->data);

        sidebar_panel = scatola_sidebar_provider_create (provider,
                        SCATOLA_WINDOW_INFO (window));
        scatola_navigation_window_add_sidebar_panel (window,
                sidebar_panel);

        g_object_unref (sidebar_panel);
    }

    scatola_module_extension_list_free (providers);

    current = scatola_side_pane_get_current_panel (window->sidebar);
    set_current_side_panel
    (window,
     SCATOLA_SIDEBAR (current));
}

gboolean
scatola_navigation_window_toolbar_showing (ScatolaNavigationWindow *window)
{
    if (window->details->toolbar != NULL)
    {
        return gtk_widget_get_visible (window->details->toolbar);
    }
    /* If we're not visible yet we haven't changed visibility, so its TRUE */
    return TRUE;
}

void
scatola_navigation_window_hide_status_bar (ScatolaNavigationWindow *window)
{
    gtk_widget_hide (SCATOLA_WINDOW (window)->details->statusbar);

    scatola_navigation_window_update_show_hide_menu_items (window);

    g_settings_set_boolean (scatola_window_state, SCATOLA_WINDOW_STATE_START_WITH_STATUS_BAR, FALSE);
}

void
scatola_navigation_window_show_status_bar (ScatolaNavigationWindow *window)
{
    gtk_widget_show (SCATOLA_WINDOW (window)->details->statusbar);

    scatola_navigation_window_update_show_hide_menu_items (window);

    g_settings_set_boolean (scatola_window_state, SCATOLA_WINDOW_STATE_START_WITH_STATUS_BAR, TRUE);
}

gboolean
scatola_navigation_window_status_bar_showing (ScatolaNavigationWindow *window)
{
    if (SCATOLA_WINDOW (window)->details->statusbar != NULL)
    {
        return gtk_widget_get_visible (SCATOLA_WINDOW (window)->details->statusbar);
    }
    /* If we're not visible yet we haven't changed visibility, so its TRUE */
    return TRUE;
}


void
scatola_navigation_window_hide_toolbar (ScatolaNavigationWindow *window)
{
    gtk_widget_hide (window->details->toolbar);
    scatola_navigation_window_update_show_hide_menu_items (window);
    g_settings_set_boolean (scatola_window_state, SCATOLA_WINDOW_STATE_START_WITH_TOOLBAR, FALSE);
}

void
scatola_navigation_window_show_toolbar (ScatolaNavigationWindow *window)
{
    gtk_widget_show (window->details->toolbar);
    scatola_navigation_window_update_show_hide_menu_items (window);
    g_settings_set_boolean (scatola_window_state, SCATOLA_WINDOW_STATE_START_WITH_TOOLBAR, TRUE);
}

void
scatola_navigation_window_hide_sidebar (ScatolaNavigationWindow *window)
{
    if (window->sidebar == NULL)
    {
        return;
    }

    scatola_navigation_window_tear_down_sidebar (window);
    scatola_navigation_window_update_show_hide_menu_items (window);

    g_settings_set_boolean (scatola_window_state, SCATOLA_WINDOW_STATE_START_WITH_SIDEBAR, FALSE);
}

void
scatola_navigation_window_show_sidebar (ScatolaNavigationWindow *window)
{
    if (window->sidebar != NULL)
    {
        return;
    }

    scatola_navigation_window_set_up_sidebar (window);
    scatola_navigation_window_update_show_hide_menu_items (window);
    g_settings_set_boolean (scatola_window_state, SCATOLA_WINDOW_STATE_START_WITH_SIDEBAR, TRUE);
}

gboolean
scatola_navigation_window_sidebar_showing (ScatolaNavigationWindow *window)
{
    g_return_val_if_fail (SCATOLA_IS_NAVIGATION_WINDOW (window), FALSE);

    return (window->sidebar != NULL)
           && gtk_widget_get_visible (gtk_paned_get_child1 (GTK_PANED (window->details->content_paned)));
}

/**
 * scatola_navigation_window_get_base_page_index:
 * @window:	Window to get index from
 *
 * Returns the index of the base page in the history list.
 * Base page is not the currently displayed page, but the page
 * that acts as the base from which the back and forward commands
 * navigate from.
 */
gint
scatola_navigation_window_get_base_page_index (ScatolaNavigationWindow *window)
{
    ScatolaNavigationWindowSlot *slot;
    gint forward_count;

    slot = SCATOLA_NAVIGATION_WINDOW_SLOT (SCATOLA_WINDOW (window)->details->active_pane->active_slot);

    forward_count = g_list_length (slot->forward_list);

    /* If forward is empty, the base it at the top of the list */
    if (forward_count == 0)
    {
        return 0;
    }

    /* The forward count indicate the relative postion of the base page
     * in the history list
     */
    return forward_count;
}

/**
 * scatola_navigation_window_show:
 * @widget: a #GtkWidget.
 *
 * Call parent and then show/hide window items
 * base on user prefs.
 */
static void
scatola_navigation_window_show (GtkWidget *widget)
{
    ScatolaNavigationWindow *window;
    gboolean show_location_bar;
    gboolean always_use_location_entry;
    GList *walk;

    window = SCATOLA_NAVIGATION_WINDOW (widget);

    /* Initially show or hide views based on preferences; once the window is displayed
     * these can be controlled on a per-window basis from View menu items.
     */

    if (g_settings_get_boolean (scatola_window_state, SCATOLA_WINDOW_STATE_START_WITH_TOOLBAR))
    {
        scatola_navigation_window_show_toolbar (window);
    }
    else
    {
        scatola_navigation_window_hide_toolbar (window);
    }

    show_location_bar = g_settings_get_boolean (scatola_window_state, SCATOLA_WINDOW_STATE_START_WITH_LOCATION_BAR);
    always_use_location_entry = g_settings_get_boolean (scatola_preferences, SCATOLA_PREFERENCES_ALWAYS_USE_LOCATION_ENTRY);
    for (walk = SCATOLA_WINDOW(window)->details->panes; walk; walk = walk->next)
    {
        ScatolaNavigationWindowPane *pane = walk->data;
        if (show_location_bar)
        {
            scatola_navigation_window_pane_show_location_bar (pane, FALSE);
        }
        else
        {
            scatola_navigation_window_pane_hide_location_bar (pane, FALSE);
        }

        if (always_use_location_entry)
        {
            scatola_navigation_window_pane_set_bar_mode (pane, SCATOLA_BAR_NAVIGATION);
        }
        else
        {
            scatola_navigation_window_pane_set_bar_mode (pane, SCATOLA_BAR_PATH);
        }
    }

    if (g_settings_get_boolean (scatola_window_state, SCATOLA_WINDOW_STATE_START_WITH_SIDEBAR))
    {
        scatola_navigation_window_show_sidebar (window);
    }
    else
    {
        scatola_navigation_window_hide_sidebar (window);
    }

    if (g_settings_get_boolean (scatola_window_state, SCATOLA_WINDOW_STATE_START_WITH_STATUS_BAR))
    {
        scatola_navigation_window_show_status_bar (window);
    }
    else
    {
        scatola_navigation_window_hide_status_bar (window);
    }

    GTK_WIDGET_CLASS (parent_class)->show (widget);
}

static void
scatola_navigation_window_save_geometry (ScatolaNavigationWindow *window)
{
    char *geometry_string;
    gboolean is_maximized;

    g_assert (SCATOLA_IS_WINDOW (window));

    if (gtk_widget_get_window (GTK_WIDGET (window)))
    {
        geometry_string = eel_gtk_window_get_geometry_string (GTK_WINDOW (window));
        is_maximized = gdk_window_get_state (gtk_widget_get_window (GTK_WIDGET (window)))
                       & GDK_WINDOW_STATE_MAXIMIZED;

        if (!is_maximized)
        {
            g_settings_set_string (scatola_window_state,
                                   SCATOLA_WINDOW_STATE_GEOMETRY,
                                   geometry_string);
        }
        g_free (geometry_string);

        g_settings_set_boolean (scatola_window_state,
                                SCATOLA_WINDOW_STATE_MAXIMIZED,
                                is_maximized);
    }
}

static void
real_window_close (ScatolaWindow *window)
{
    scatola_navigation_window_save_geometry (SCATOLA_NAVIGATION_WINDOW (window));
}

static void
real_get_min_size (ScatolaWindow *window,
                   guint *min_width, guint *min_height)
{
    if (min_width)
    {
        *min_width = SCATOLA_NAVIGATION_WINDOW_MIN_WIDTH;
    }
    if (min_height)
    {
        *min_height = SCATOLA_NAVIGATION_WINDOW_MIN_HEIGHT;
    }
}

static void
real_get_default_size (ScatolaWindow *window,
                       guint *default_width, guint *default_height)
{
    if (default_width)
    {
        *default_width = SCATOLA_NAVIGATION_WINDOW_DEFAULT_WIDTH;
    }

    if (default_height)
    {
        *default_height = SCATOLA_NAVIGATION_WINDOW_DEFAULT_HEIGHT;
    }
}

static ScatolaWindowSlot *
real_open_slot (ScatolaWindowPane *pane,
                ScatolaWindowOpenSlotFlags flags)
{
    ScatolaWindowSlot *slot;

    slot = (ScatolaWindowSlot *) g_object_new (SCATOLA_TYPE_NAVIGATION_WINDOW_SLOT, NULL);
    slot->pane = pane;

    scatola_navigation_window_pane_add_slot_in_tab (SCATOLA_NAVIGATION_WINDOW_PANE (pane), slot, flags);
    gtk_widget_show (slot->content_box);

    return slot;
}

static void
real_close_slot (ScatolaWindowPane *pane,
                 ScatolaWindowSlot *slot)
{
    int page_num;
    GtkNotebook *notebook;

    notebook = GTK_NOTEBOOK (SCATOLA_NAVIGATION_WINDOW_PANE (pane)->notebook);

    page_num = gtk_notebook_page_num (notebook, slot->content_box);
    g_assert (page_num >= 0);

    scatola_navigation_window_pane_remove_page (SCATOLA_NAVIGATION_WINDOW_PANE (pane), page_num);

    gtk_notebook_set_show_tabs (notebook,
                                gtk_notebook_get_n_pages (notebook) > 1);

    EEL_CALL_PARENT (SCATOLA_WINDOW_CLASS,
                     close_slot, (pane, slot));
}

static void
scatola_navigation_window_class_init (ScatolaNavigationWindowClass *class)
{
    SCATOLA_WINDOW_CLASS (class)->window_type = SCATOLA_WINDOW_NAVIGATION;
    SCATOLA_WINDOW_CLASS (class)->bookmarks_placeholder = MENU_PATH_BOOKMARKS_PLACEHOLDER;

    G_OBJECT_CLASS (class)->finalize = scatola_navigation_window_finalize;
#if GTK_CHECK_VERSION (3, 0, 0)
    GTK_WIDGET_CLASS (class)->destroy = scatola_navigation_window_destroy;
#else
    GTK_OBJECT_CLASS (class)->destroy = scatola_navigation_window_destroy;
#endif
    GTK_WIDGET_CLASS (class)->show = scatola_navigation_window_show;
    GTK_WIDGET_CLASS (class)->window_state_event = scatola_navigation_window_state_event;
    GTK_WIDGET_CLASS (class)->key_press_event = scatola_navigation_window_key_press_event;
    GTK_WIDGET_CLASS (class)->button_press_event = scatola_navigation_window_button_press_event;
    SCATOLA_WINDOW_CLASS (class)->sync_allow_stop = real_sync_allow_stop;
    SCATOLA_WINDOW_CLASS (class)->prompt_for_location = real_prompt_for_location;
    SCATOLA_WINDOW_CLASS (class)->sync_title = real_sync_title;
    SCATOLA_WINDOW_CLASS (class)->get_icon = real_get_icon;
    SCATOLA_WINDOW_CLASS (class)->get_min_size = real_get_min_size;
    SCATOLA_WINDOW_CLASS (class)->get_default_size = real_get_default_size;
    SCATOLA_WINDOW_CLASS (class)->close = real_window_close;

    SCATOLA_WINDOW_CLASS (class)->open_slot = real_open_slot;
    SCATOLA_WINDOW_CLASS (class)->close_slot = real_close_slot;

    g_type_class_add_private (G_OBJECT_CLASS (class), sizeof (ScatolaNavigationWindowDetails));

    g_signal_connect_swapped (scatola_preferences,
                              "changed::" SCATOLA_PREFERENCES_MOUSE_BACK_BUTTON,
                              G_CALLBACK(mouse_back_button_changed),
                              NULL);

    g_signal_connect_swapped (scatola_preferences,
                              "changed::" SCATOLA_PREFERENCES_MOUSE_FORWARD_BUTTON,
                              G_CALLBACK(mouse_forward_button_changed),
                              NULL);

    g_signal_connect_swapped (scatola_preferences,
                              "changed::" SCATOLA_PREFERENCES_MOUSE_USE_EXTRA_BUTTONS,
                              G_CALLBACK(use_extra_mouse_buttons_changed),
                              NULL);
}

static ScatolaWindowSlot *
create_extra_pane (ScatolaNavigationWindow *window)
{
    ScatolaWindow *win;
    ScatolaNavigationWindowPane *pane;
    ScatolaWindowSlot *slot;
    GtkPaned *paned;

    win = SCATOLA_WINDOW (window);

    /* New pane */
    pane = scatola_navigation_window_pane_new (win);
    win->details->panes = g_list_append (win->details->panes, pane);

    scatola_navigation_window_pane_setup (pane);

    paned = GTK_PANED (window->details->split_view_hpane);
    if (gtk_paned_get_child1 (paned) == NULL)
    {
        gtk_paned_pack1 (paned, pane->widget, TRUE, FALSE);
    }
    else
    {
        gtk_paned_pack2 (paned, pane->widget, TRUE, FALSE);
    }

    /* slot */
    slot = scatola_window_open_slot (SCATOLA_WINDOW_PANE (pane),
                                  SCATOLA_WINDOW_OPEN_SLOT_APPEND);
    SCATOLA_WINDOW_PANE (pane)->active_slot = slot;

    return slot;
}

void
scatola_navigation_window_split_view_on (ScatolaNavigationWindow *window)
{
    ScatolaWindow *win;
    ScatolaNavigationWindowPane *pane;
    ScatolaWindowSlot *slot, *old_active_slot;
    GFile *location;
    GtkAction *action;

    win = SCATOLA_WINDOW (window);

    old_active_slot = scatola_window_get_active_slot (win);
    slot = create_extra_pane (window);
    pane = SCATOLA_NAVIGATION_WINDOW_PANE (slot->pane);

    location = NULL;
    if (old_active_slot != NULL)
    {
        location = scatola_window_slot_get_location (old_active_slot);
        if (location != NULL)
        {
            if (g_file_has_uri_scheme (location, "x-scatola-search"))
            {
                g_object_unref (location);
                location = NULL;
            }
        }
    }
    if (location == NULL)
    {
        location = g_file_new_for_path (g_get_home_dir ());
    }

    scatola_window_slot_go_to (slot, location, FALSE);
    g_object_unref (location);

    action = gtk_action_group_get_action (SCATOLA_NAVIGATION_WINDOW (SCATOLA_WINDOW_PANE (pane)->window)->details->navigation_action_group,
                                          SCATOLA_ACTION_SHOW_HIDE_LOCATION_BAR);
    if (gtk_toggle_action_get_active (GTK_TOGGLE_ACTION (action)))
    {
        scatola_navigation_window_pane_show_location_bar (pane, TRUE);
    }
    else
    {
        scatola_navigation_window_pane_hide_location_bar (pane, TRUE);
    }
}

void
scatola_navigation_window_split_view_off (ScatolaNavigationWindow *window)
{
    ScatolaWindow *win;
    ScatolaWindowPane *pane, *active_pane;
    GList *l, *next;

    win = SCATOLA_WINDOW (window);

    g_return_if_fail (win);

    active_pane = win->details->active_pane;

    /* delete all panes except the first (main) pane */
    for (l = win->details->panes; l != NULL; l = next)
    {
        next = l->next;
        pane = l->data;
        if (pane != active_pane)
        {
            scatola_window_close_pane (pane);
        }
    }

    scatola_navigation_window_update_show_hide_menu_items (window);
    scatola_navigation_window_update_split_view_actions_sensitivity (window);
}

gboolean
scatola_navigation_window_split_view_showing (ScatolaNavigationWindow *window)
{
    return g_list_length (SCATOLA_WINDOW (window)->details->panes) > 1;
}
