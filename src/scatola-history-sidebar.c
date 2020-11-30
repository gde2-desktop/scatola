/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/*
 *  Scatola
 *
 *  Copyright (C) 1999, 2000 Red Hat, Inc.
 *  Copyright (C) 2000, 2001 Eazel, Inc.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License as
 *  published by the Free Software Foundation; either version 2 of the
 *  License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 *  Authors: Elliot Lee <sopwith@redhat.com>
 *           Darin Adler <darin@bentspoon.com>
 *
 */

#include <config.h>

#include <eel/eel-gtk-extensions.h>
#include <gtk/gtk.h>
#include <glib/gi18n.h>
#include <libscatola-private/scatola-bookmark.h>
#include <libscatola-private/scatola-global-preferences.h>
#include <libscatola-private/scatola-sidebar-provider.h>
#include <libscatola-private/scatola-module.h>
#include <libscatola-private/scatola-signaller.h>
#include <libscatola-private/scatola-window-info.h>
#include <libscatola-private/scatola-window-slot-info.h>

#include "scatola-history-sidebar.h"

#define SCATOLA_HISTORY_SIDEBAR_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), SCATOLA_TYPE_HISTORY_SIDEBAR, ScatolaHistorySidebarClass))
#define SCATOLA_IS_HISTORY_SIDEBAR(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SCATOLA_TYPE_HISTORY_SIDEBAR))
#define SCATOLA_IS_HISTORY_SIDEBAR_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), SCATOLA_TYPE_HISTORY_SIDEBAR))

typedef struct
{
    GtkScrolledWindowClass parent;
} ScatolaHistorySidebarClass;

typedef struct
{
    GObject parent;
} ScatolaHistorySidebarProvider;

typedef struct
{
    GObjectClass parent;
} ScatolaHistorySidebarProviderClass;


enum
{
    HISTORY_SIDEBAR_COLUMN_ICON,
    HISTORY_SIDEBAR_COLUMN_NAME,
    HISTORY_SIDEBAR_COLUMN_BOOKMARK,
    HISTORY_SIDEBAR_COLUMN_COUNT
};

static void  scatola_history_sidebar_iface_init        (ScatolaSidebarIface         *iface);
static void  sidebar_provider_iface_init                (ScatolaSidebarProviderIface *iface);
static GType scatola_history_sidebar_provider_get_type (void);
#if GTK_CHECK_VERSION (3, 0, 0)
static void  scatola_history_sidebar_style_updated	        (GtkWidget *widget);
#else
static void  scatola_history_sidebar_style_set	        (GtkWidget *widget,
        GtkStyle  *previous_style);
#endif

G_DEFINE_TYPE_WITH_CODE (ScatolaHistorySidebar, scatola_history_sidebar, GTK_TYPE_SCROLLED_WINDOW,
                         G_IMPLEMENT_INTERFACE (SCATOLA_TYPE_SIDEBAR,
                                 scatola_history_sidebar_iface_init));

G_DEFINE_TYPE_WITH_CODE (ScatolaHistorySidebarProvider, scatola_history_sidebar_provider, G_TYPE_OBJECT,
                         G_IMPLEMENT_INTERFACE (SCATOLA_TYPE_SIDEBAR_PROVIDER,
                                 sidebar_provider_iface_init));

static void
update_history (ScatolaHistorySidebar *sidebar)
{
    GtkListStore         *store;
    GtkTreeSelection     *selection;
    ScatolaBookmark     *bookmark;
    GdkPixbuf            *pixbuf;
    GtkTreeIter           iter;
    char *name;
    GList *l, *history;

    store = GTK_LIST_STORE (gtk_tree_view_get_model (sidebar->tree_view));

    gtk_list_store_clear (store);

    history = scatola_window_info_get_history (sidebar->window);
    for (l = history; l != NULL; l = l->next)
    {
        bookmark = scatola_bookmark_copy (l->data);

        pixbuf = scatola_bookmark_get_pixbuf (bookmark, GTK_ICON_SIZE_MENU);
        name = scatola_bookmark_get_name (bookmark);
        gtk_list_store_append (store, &iter);
        gtk_list_store_set (store, &iter,
                            HISTORY_SIDEBAR_COLUMN_ICON, pixbuf,
                            HISTORY_SIDEBAR_COLUMN_NAME, name,
                            HISTORY_SIDEBAR_COLUMN_BOOKMARK, bookmark,
                            -1);
        g_object_unref (bookmark);

        if (pixbuf != NULL)
        {
            g_object_unref (pixbuf);
        }
        g_free (name);
    }
    g_list_free_full (history, g_object_unref);

    selection = GTK_TREE_SELECTION (gtk_tree_view_get_selection (sidebar->tree_view));

    if (gtk_tree_model_get_iter_first (GTK_TREE_MODEL (store), &iter))
    {
        gtk_tree_selection_select_iter (selection, &iter);
    }
}

static void
history_changed_callback (GObject *signaller,
                          ScatolaHistorySidebar *sidebar)
{
    update_history (sidebar);
}

static void
open_selected_item (ScatolaHistorySidebar *sidebar,
                    GtkTreePath *path,
                    ScatolaWindowOpenFlags flags)
{
    ScatolaWindowSlotInfo *slot;
    GtkTreeModel *model;
    GtkTreeIter iter;
    ScatolaBookmark *bookmark;
    GFile *location;

    model = gtk_tree_view_get_model (sidebar->tree_view);

    if (!gtk_tree_model_get_iter (model, &iter, path))
    {
        return;
    }

    gtk_tree_model_get
    (model, &iter, HISTORY_SIDEBAR_COLUMN_BOOKMARK, &bookmark, -1);

    /* Navigate to the clicked location. */
    location = scatola_bookmark_get_location (SCATOLA_BOOKMARK (bookmark));
    slot = scatola_window_info_get_active_slot (sidebar->window);
    scatola_window_slot_info_open_location
    (slot,
     location, SCATOLA_WINDOW_OPEN_ACCORDING_TO_MODE,
     flags, NULL);
    g_object_unref (location);
}

static void
row_activated_callback (GtkTreeView *tree_view,
                        GtkTreePath *path,
                        GtkTreeViewColumn *column,
                        gpointer user_data)
{
    ScatolaHistorySidebar *sidebar;

    sidebar = SCATOLA_HISTORY_SIDEBAR (user_data);
    g_assert (sidebar->tree_view == tree_view);

    open_selected_item (sidebar, path, 0);
}

static gboolean
button_press_event_callback (GtkWidget *widget,
                             GdkEventButton *event,
                             gpointer user_data)
{
    if (event->button == 2 && event->type == GDK_BUTTON_PRESS)
    {
        /* Open new tab on middle click. */
        ScatolaHistorySidebar *sidebar;
        GtkTreePath *path;

        sidebar = SCATOLA_HISTORY_SIDEBAR (user_data);
        g_assert (sidebar->tree_view == GTK_TREE_VIEW (widget));

        if (gtk_tree_view_get_path_at_pos (sidebar->tree_view,
                                           event->x, event->y,
                                           &path, NULL, NULL, NULL))
        {
            open_selected_item (sidebar,
                                path,
                                SCATOLA_WINDOW_OPEN_FLAG_NEW_TAB);
            gtk_tree_path_free (path);
        }
    }

    return FALSE;
}

static void
update_click_policy (ScatolaHistorySidebar *sidebar)
{
    int policy;

    policy = g_settings_get_enum (scatola_preferences, SCATOLA_PREFERENCES_CLICK_POLICY);

    eel_gtk_tree_view_set_activate_on_single_click
    (sidebar->tree_view, policy == SCATOLA_CLICK_POLICY_SINGLE);
}

static void
click_policy_changed_callback (gpointer user_data)
{
    ScatolaHistorySidebar *sidebar;

    sidebar = SCATOLA_HISTORY_SIDEBAR (user_data);

    update_click_policy (sidebar);
}

static void
scatola_history_sidebar_init (ScatolaHistorySidebar *sidebar)
{
    GtkTreeView       *tree_view;
    GtkTreeViewColumn *col;
    GtkCellRenderer   *cell;
    GtkListStore      *store;
    GtkTreeSelection  *selection;

    tree_view = GTK_TREE_VIEW (gtk_tree_view_new ());
    gtk_tree_view_set_headers_visible (tree_view, FALSE);
    gtk_widget_show (GTK_WIDGET (tree_view));

    col = GTK_TREE_VIEW_COLUMN (gtk_tree_view_column_new ());

    cell = gtk_cell_renderer_pixbuf_new ();
    gtk_tree_view_column_pack_start (col, cell, FALSE);
    gtk_tree_view_column_set_attributes (col, cell,
                                         "pixbuf", HISTORY_SIDEBAR_COLUMN_ICON,
                                         NULL);

    cell = gtk_cell_renderer_text_new ();
    gtk_tree_view_column_pack_start (col, cell, TRUE);
    gtk_tree_view_column_set_attributes (col, cell,
                                         "text", HISTORY_SIDEBAR_COLUMN_NAME,
                                         NULL);

    gtk_tree_view_column_set_fixed_width (col, SCATOLA_ICON_SIZE_SMALLER);
    gtk_tree_view_append_column (tree_view, col);

    store = gtk_list_store_new (HISTORY_SIDEBAR_COLUMN_COUNT,
                                GDK_TYPE_PIXBUF,
                                G_TYPE_STRING,
                                SCATOLA_TYPE_BOOKMARK);

    gtk_tree_view_set_model (tree_view, GTK_TREE_MODEL (store));
    g_object_unref (store);

    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (sidebar),
                                    GTK_POLICY_AUTOMATIC,
                                    GTK_POLICY_AUTOMATIC);
    gtk_scrolled_window_set_hadjustment (GTK_SCROLLED_WINDOW (sidebar), NULL);
    gtk_scrolled_window_set_vadjustment (GTK_SCROLLED_WINDOW (sidebar), NULL);
    gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (sidebar), GTK_SHADOW_IN);
    gtk_container_add (GTK_CONTAINER (sidebar), GTK_WIDGET (tree_view));
    gtk_widget_show (GTK_WIDGET (sidebar));

    sidebar->tree_view = tree_view;

    selection = gtk_tree_view_get_selection (tree_view);
    gtk_tree_selection_set_mode (selection, GTK_SELECTION_SINGLE);

    g_signal_connect_object
    (tree_view, "row_activated",
     G_CALLBACK (row_activated_callback), sidebar, 0);

    g_signal_connect_object (scatola_signaller_get_current (),
                             "history_list_changed",
                             G_CALLBACK (history_changed_callback), sidebar, 0);

    g_signal_connect (tree_view, "button-press-event",
                      G_CALLBACK (button_press_event_callback), sidebar);

    g_signal_connect_swapped (scatola_preferences,
                              "changed::" SCATOLA_PREFERENCES_CLICK_POLICY,
                              G_CALLBACK(click_policy_changed_callback),
                              sidebar);
    update_click_policy (sidebar);
}

static void
scatola_history_sidebar_finalize (GObject *object)
{
    ScatolaHistorySidebar *sidebar;

    sidebar = SCATOLA_HISTORY_SIDEBAR (object);

    g_signal_handlers_disconnect_by_func (scatola_preferences,
                                          click_policy_changed_callback,
                                          sidebar);

    G_OBJECT_CLASS (scatola_history_sidebar_parent_class)->finalize (object);
}

static void
scatola_history_sidebar_class_init (ScatolaHistorySidebarClass *class)
{
    G_OBJECT_CLASS (class)->finalize = scatola_history_sidebar_finalize;

#if GTK_CHECK_VERSION (3, 0, 0)
    GTK_WIDGET_CLASS (class)->style_updated = scatola_history_sidebar_style_updated;
#else
    GTK_WIDGET_CLASS (class)->style_set = scatola_history_sidebar_style_set;
#endif
}

static const char *
scatola_history_sidebar_get_sidebar_id (ScatolaSidebar *sidebar)
{
    return SCATOLA_HISTORY_SIDEBAR_ID;
}

static char *
scatola_history_sidebar_get_tab_label (ScatolaSidebar *sidebar)
{
    return g_strdup (_("History"));
}

static char *
scatola_history_sidebar_get_tab_tooltip (ScatolaSidebar *sidebar)
{
    return g_strdup (_("Show History"));
}

static GdkPixbuf *
scatola_history_sidebar_get_tab_icon (ScatolaSidebar *sidebar)
{
    return NULL;
}

static void
scatola_history_sidebar_is_visible_changed (ScatolaSidebar *sidebar,
        gboolean         is_visible)
{
    /* Do nothing */
}

static void
scatola_history_sidebar_iface_init (ScatolaSidebarIface *iface)
{
    iface->get_sidebar_id = scatola_history_sidebar_get_sidebar_id;
    iface->get_tab_label = scatola_history_sidebar_get_tab_label;
    iface->get_tab_tooltip = scatola_history_sidebar_get_tab_tooltip;
    iface->get_tab_icon = scatola_history_sidebar_get_tab_icon;
    iface->is_visible_changed = scatola_history_sidebar_is_visible_changed;
}

static void
scatola_history_sidebar_set_parent_window (ScatolaHistorySidebar *sidebar,
                                        ScatolaWindowInfo *window)
{
    sidebar->window = window;
    update_history (sidebar);
}

static void
#if GTK_CHECK_VERSION (3, 0, 0)
scatola_history_sidebar_style_updated (GtkWidget *widget)
#else
scatola_history_sidebar_style_set (GtkWidget *widget,
                                GtkStyle  *previous_style)
#endif
{
    ScatolaHistorySidebar *sidebar;

    sidebar = SCATOLA_HISTORY_SIDEBAR (widget);

    update_history (sidebar);
}

static ScatolaSidebar *
scatola_history_sidebar_create (ScatolaSidebarProvider *provider,
                             ScatolaWindowInfo *window)
{
    ScatolaHistorySidebar *sidebar;

    sidebar = g_object_new (scatola_history_sidebar_get_type (), NULL);
    scatola_history_sidebar_set_parent_window (sidebar, window);
    g_object_ref_sink (sidebar);

    return SCATOLA_SIDEBAR (sidebar);
}

static void
sidebar_provider_iface_init (ScatolaSidebarProviderIface *iface)
{
    iface->create = scatola_history_sidebar_create;
}

static void
scatola_history_sidebar_provider_init (ScatolaHistorySidebarProvider *sidebar)
{
}

static void
scatola_history_sidebar_provider_class_init (ScatolaHistorySidebarProviderClass *class)
{
}

void
scatola_history_sidebar_register (void)
{
    scatola_module_add_type (scatola_history_sidebar_provider_get_type ());
}

