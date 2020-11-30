/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*-

   scatola-window-slot.c: Scatola window slot

   Copyright (C) 2008 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public
   License along with this program; if not, write to the
   Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
   Boston, MA 02110-1301, USA.

   Author: Christian Neumair <cneumair@gnome.org>
*/
#include "scatola-window-slot.h"
#include "scatola-navigation-window-slot.h"

#include "scatola-desktop-window.h"
#include "scatola-window-private.h"
#include "scatola-window-manage-views.h"
#include <libscatola-private/scatola-file.h>
#include <libscatola-private/scatola-file-utilities.h>
#include <libscatola-private/scatola-window-slot-info.h>
#include <eel/eel-gtk-macros.h>
#include <eel/eel-string.h>

#if GTK_CHECK_VERSION (3, 0, 0)
#define gtk_vbox_new(X,Y) gtk_box_new(GTK_ORIENTATION_VERTICAL,Y)
#endif

static void scatola_window_slot_init       (ScatolaWindowSlot *slot);
static void scatola_window_slot_class_init (ScatolaWindowSlotClass *class);
static void scatola_window_slot_dispose    (GObject *object);

static void scatola_window_slot_info_iface_init (ScatolaWindowSlotInfoIface *iface);

G_DEFINE_TYPE_WITH_CODE (ScatolaWindowSlot,
                         scatola_window_slot,
                         G_TYPE_OBJECT,
                         G_IMPLEMENT_INTERFACE (SCATOLA_TYPE_WINDOW_SLOT_INFO,
                                 scatola_window_slot_info_iface_init))
#define parent_class scatola_window_slot_parent_class

static void
query_editor_changed_callback (ScatolaSearchBar *bar,
                               ScatolaQuery *query,
                               gboolean reload,
                               ScatolaWindowSlot *slot)
{
    ScatolaDirectory *directory;

    directory = scatola_directory_get_for_file (slot->viewed_file);
    g_assert (SCATOLA_IS_SEARCH_DIRECTORY (directory));

    scatola_search_directory_set_query (SCATOLA_SEARCH_DIRECTORY (directory),
                                     query);
    if (reload)
    {
        scatola_window_slot_reload (slot);
    }

    scatola_directory_unref (directory);
}

static void
real_update_query_editor (ScatolaWindowSlot *slot)
{
    GtkWidget *query_editor;
    ScatolaQuery *query;
    ScatolaDirectory *directory;
    ScatolaSearchDirectory *search_directory;

    directory = scatola_directory_get (slot->location);

    if (SCATOLA_IS_SEARCH_DIRECTORY (directory))
    {
        search_directory = SCATOLA_SEARCH_DIRECTORY (directory);

        query_editor = scatola_query_editor_new (scatola_search_directory_is_saved_search (search_directory),
                                              scatola_search_directory_is_indexed (search_directory));

        slot->query_editor = SCATOLA_QUERY_EDITOR (query_editor);

        scatola_window_slot_add_extra_location_widget (slot, query_editor);
        gtk_widget_show (query_editor);
        g_signal_connect_object (query_editor, "changed",
                                 G_CALLBACK (query_editor_changed_callback), slot, 0);

        query = scatola_search_directory_get_query (search_directory);
        if (query != NULL)
        {
            scatola_query_editor_set_query (SCATOLA_QUERY_EDITOR (query_editor),
                                         query);
            g_object_unref (query);
        }
        else
        {
            scatola_query_editor_set_default_query (SCATOLA_QUERY_EDITOR (query_editor));
        }
    }

    scatola_directory_unref (directory);
}


static void
real_active (ScatolaWindowSlot *slot)
{
    ScatolaWindow *window;

    window = slot->pane->window;

    /* sync window to new slot */
    scatola_window_sync_status (window);
    scatola_window_sync_allow_stop (window, slot);
    scatola_window_sync_title (window, slot);
    scatola_window_sync_zoom_widgets (window);
    scatola_window_pane_sync_location_widgets (slot->pane);
    scatola_window_pane_sync_search_widgets (slot->pane);

    if (slot->viewed_file != NULL)
    {
        scatola_window_load_view_as_menus (window);
        scatola_window_load_extension_menus (window);
    }
}

static void
scatola_window_slot_active (ScatolaWindowSlot *slot)
{
    ScatolaWindow *window;
    ScatolaWindowPane *pane;

    g_assert (SCATOLA_IS_WINDOW_SLOT (slot));

    pane = SCATOLA_WINDOW_PANE (slot->pane);
    window = SCATOLA_WINDOW (slot->pane->window);
    g_assert (g_list_find (pane->slots, slot) != NULL);
    g_assert (slot == window->details->active_pane->active_slot);

    EEL_CALL_METHOD (SCATOLA_WINDOW_SLOT_CLASS, slot,
                     active, (slot));
}

static void
real_inactive (ScatolaWindowSlot *slot)
{
    ScatolaWindow *window;

    window = SCATOLA_WINDOW (slot->pane->window);
    g_assert (slot == window->details->active_pane->active_slot);
}

static void
scatola_window_slot_inactive (ScatolaWindowSlot *slot)
{
    ScatolaWindow *window;
    ScatolaWindowPane *pane;

    g_assert (SCATOLA_IS_WINDOW_SLOT (slot));

    pane = SCATOLA_WINDOW_PANE (slot->pane);
    window = SCATOLA_WINDOW (pane->window);

    g_assert (g_list_find (pane->slots, slot) != NULL);
    g_assert (slot == window->details->active_pane->active_slot);

    EEL_CALL_METHOD (SCATOLA_WINDOW_SLOT_CLASS, slot,
                     inactive, (slot));
}


static void
scatola_window_slot_init (ScatolaWindowSlot *slot)
{
    GtkWidget *content_box, *eventbox, *extras_vbox, *frame;

    content_box = gtk_vbox_new (FALSE, 0);
    slot->content_box = content_box;
    gtk_widget_show (content_box);

    frame = gtk_frame_new (NULL);
    gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_ETCHED_IN);
    gtk_box_pack_start (GTK_BOX (content_box), frame, FALSE, FALSE, 0);
    slot->extra_location_frame = frame;

    eventbox = gtk_event_box_new ();
    gtk_widget_set_name (eventbox, "scatola-extra-view-widget");
    gtk_container_add (GTK_CONTAINER (frame), eventbox);
    gtk_widget_show (eventbox);

    extras_vbox = gtk_vbox_new (FALSE, 6);
    gtk_container_set_border_width (GTK_CONTAINER (extras_vbox), 6);
    slot->extra_location_widgets = extras_vbox;
    gtk_container_add (GTK_CONTAINER (eventbox), extras_vbox);
    gtk_widget_show (extras_vbox);

    slot->view_box = gtk_vbox_new (FALSE, 0);
    gtk_box_pack_start (GTK_BOX (content_box), slot->view_box, TRUE, TRUE, 0);
    gtk_widget_show (slot->view_box);

    slot->title = g_strdup (_("Loading..."));
}

static void
scatola_window_slot_class_init (ScatolaWindowSlotClass *class)
{
    class->active = real_active;
    class->inactive = real_inactive;
    class->update_query_editor = real_update_query_editor;

    G_OBJECT_CLASS (class)->dispose = scatola_window_slot_dispose;
}

static int
scatola_window_slot_get_selection_count (ScatolaWindowSlot *slot)
{
    g_assert (SCATOLA_IS_WINDOW_SLOT (slot));

    if (slot->content_view != NULL)
    {
        return scatola_view_get_selection_count (slot->content_view);
    }
    return 0;
}

GFile *
scatola_window_slot_get_location (ScatolaWindowSlot *slot)
{
    g_assert (slot != NULL);
    g_assert (SCATOLA_IS_WINDOW (slot->pane->window));

    if (slot->location != NULL)
    {
        return g_object_ref (slot->location);
    }
    return NULL;
}

char *
scatola_window_slot_get_location_uri (ScatolaWindowSlotInfo *slot)
{
    g_assert (SCATOLA_IS_WINDOW_SLOT (slot));

    if (slot->location)
    {
        return g_file_get_uri (slot->location);
    }
    return NULL;
}

static void
scatola_window_slot_make_hosting_pane_active (ScatolaWindowSlot *slot)
{
    g_assert (SCATOLA_IS_WINDOW_SLOT (slot));
    g_assert (SCATOLA_IS_WINDOW_PANE (slot->pane));

    scatola_window_set_active_slot (slot->pane->window, slot);
}

char *
scatola_window_slot_get_title (ScatolaWindowSlot *slot)
{
    char *title;

    g_assert (SCATOLA_IS_WINDOW_SLOT (slot));

    title = NULL;
    if (slot->new_content_view != NULL)
    {
        title = scatola_view_get_title (slot->new_content_view);
    }
    else if (slot->content_view != NULL)
    {
        title = scatola_view_get_title (slot->content_view);
    }

    if (title == NULL)
    {
        title = scatola_compute_title_for_location (slot->location);
    }

    return title;
}

static ScatolaWindow *
scatola_window_slot_get_window (ScatolaWindowSlot *slot)
{
    g_assert (SCATOLA_IS_WINDOW_SLOT (slot));
    return slot->pane->window;
}

/* scatola_window_slot_set_title:
 *
 * Sets slot->title, and if it changed
 * synchronizes the actual GtkWindow title which
 * might look a bit different (e.g. with "file browser:" added)
 */
static void
scatola_window_slot_set_title (ScatolaWindowSlot *slot,
                            const char *title)
{
    ScatolaWindow *window;
    gboolean changed;

    g_assert (SCATOLA_IS_WINDOW_SLOT (slot));

    window = SCATOLA_WINDOW (slot->pane->window);

    changed = FALSE;

    if (eel_strcmp (title, slot->title) != 0)
    {
        changed = TRUE;

        g_free (slot->title);
        slot->title = g_strdup (title);
    }

    if (eel_strlen (slot->title) > 0 && slot->current_location_bookmark &&
            scatola_bookmark_set_name (slot->current_location_bookmark,
                                    slot->title))
    {
        changed = TRUE;

        /* Name of item in history list changed, tell listeners. */
        scatola_send_history_list_changed ();
    }

    if (changed)
    {
        scatola_window_sync_title (window, slot);
    }
}


/* scatola_window_slot_update_title:
 *
 * Re-calculate the slot title.
 * Called when the location or view has changed.
 * @slot: The ScatolaWindowSlot in question.
 *
 */
void
scatola_window_slot_update_title (ScatolaWindowSlot *slot)
{
    char *title;

    title = scatola_window_slot_get_title (slot);
    scatola_window_slot_set_title (slot, title);
    g_free (title);
}

/* scatola_window_slot_update_icon:
 *
 * Re-calculate the slot icon
 * Called when the location or view or icon set has changed.
 * @slot: The ScatolaWindowSlot in question.
 */
void
scatola_window_slot_update_icon (ScatolaWindowSlot *slot)
{
    ScatolaWindow *window;
    ScatolaIconInfo *info;
    const char *icon_name;
    GdkPixbuf *pixbuf;

    window = slot->pane->window;

    g_return_if_fail (SCATOLA_IS_WINDOW (window));

    info = EEL_CALL_METHOD_WITH_RETURN_VALUE (SCATOLA_WINDOW_CLASS, window,
            get_icon, (window, slot));

    icon_name = NULL;
    if (info)
    {
        icon_name = scatola_icon_info_get_used_name (info);
        if (icon_name != NULL)
        {
            /* Gtk+ doesn't short circuit this (yet), so avoid lots of work
             * if we're setting to the same icon. This happens a lot e.g. when
             * the trash directory changes due to the file count changing.
             */
            if (g_strcmp0 (icon_name, gtk_window_get_icon_name (GTK_WINDOW (window))) != 0)
            {
                gtk_window_set_icon_name (GTK_WINDOW (window), icon_name);
            }
        }
        else
        {
            pixbuf = scatola_icon_info_get_pixbuf_nodefault (info);

            if (pixbuf)
            {
                gtk_window_set_icon (GTK_WINDOW (window), pixbuf);
                g_object_unref (pixbuf);
            }
        }

        g_object_unref (info);
    }
}

void
scatola_window_slot_is_in_active_pane (ScatolaWindowSlot *slot,
                                    gboolean is_active)
{
    /* NULL is valid, and happens during init */
    if (!slot)
    {
        return;
    }

    /* it may also be that the content is not a valid directory view during init */
    if (slot->content_view != NULL)
    {
        scatola_view_set_is_active (slot->content_view, is_active);
    }

    if (slot->new_content_view != NULL)
    {
        scatola_view_set_is_active (slot->new_content_view, is_active);
    }
}

void
scatola_window_slot_connect_content_view (ScatolaWindowSlot *slot,
                                       ScatolaView *view)
{
    ScatolaWindow *window;

    window = slot->pane->window;
    if (window != NULL && slot == scatola_window_get_active_slot (window))
    {
        scatola_window_connect_content_view (window, view);
    }
}

void
scatola_window_slot_disconnect_content_view (ScatolaWindowSlot *slot,
        ScatolaView *view)
{
    ScatolaWindow *window;

    window = slot->pane->window;
    if (window != NULL && window->details->active_pane && window->details->active_pane->active_slot == slot)
    {
        scatola_window_disconnect_content_view (window, view);
    }
}

void
scatola_window_slot_set_content_view_widget (ScatolaWindowSlot *slot,
        ScatolaView *new_view)
{
    ScatolaWindow *window;
    GtkWidget *widget;

    window = slot->pane->window;
    g_assert (SCATOLA_IS_WINDOW (window));

    if (slot->content_view != NULL)
    {
        /* disconnect old view */
        scatola_window_slot_disconnect_content_view (slot, slot->content_view);

        widget = scatola_view_get_widget (slot->content_view);
        gtk_widget_destroy (widget);
        g_object_unref (slot->content_view);
        slot->content_view = NULL;
    }

    if (new_view != NULL)
    {
        widget = scatola_view_get_widget (new_view);
#if GTK_CHECK_VERSION (3, 0, 0)
        gtk_box_pack_start (GTK_BOX (slot->view_box), widget,
                            TRUE, TRUE, 0);
#else
        gtk_container_add (GTK_CONTAINER (slot->view_box),
                           GTK_WIDGET (new_view));
#endif

        gtk_widget_show (widget);

        slot->content_view = new_view;
        g_object_ref (slot->content_view);

        /* connect new view */
        scatola_window_slot_connect_content_view (slot, new_view);
    }
}

void
scatola_window_slot_set_allow_stop (ScatolaWindowSlot *slot,
                                 gboolean allow)
{
    ScatolaWindow *window;

    g_assert (SCATOLA_IS_WINDOW_SLOT (slot));

    slot->allow_stop = allow;

    window = SCATOLA_WINDOW (slot->pane->window);
    scatola_window_sync_allow_stop (window, slot);
}

void
scatola_window_slot_set_status (ScatolaWindowSlot *slot,
                             const char *status)
{
    ScatolaWindow *window;

    g_assert (SCATOLA_IS_WINDOW_SLOT (slot));

    g_free (slot->status_text);
    slot->status_text = g_strdup (status);

    window = SCATOLA_WINDOW (slot->pane->window);
    if (slot == window->details->active_pane->active_slot)
    {
        scatola_window_sync_status (window);
    }
}

/* scatola_window_slot_update_query_editor:
 *
 * Update the query editor.
 * Called when the location has changed.
 *
 * @slot: The ScatolaWindowSlot in question.
 */
void
scatola_window_slot_update_query_editor (ScatolaWindowSlot *slot)
{
    if (slot->query_editor != NULL)
    {
        gtk_widget_destroy (GTK_WIDGET (slot->query_editor));
        g_assert (slot->query_editor == NULL);
    }

    EEL_CALL_METHOD (SCATOLA_WINDOW_SLOT_CLASS, slot,
                     update_query_editor, (slot));

    eel_add_weak_pointer (&slot->query_editor);
}

static void
remove_all (GtkWidget *widget,
            gpointer data)
{
    GtkContainer *container;
    container = GTK_CONTAINER (data);

    gtk_container_remove (container, widget);
}

void
scatola_window_slot_remove_extra_location_widgets (ScatolaWindowSlot *slot)
{
    gtk_container_foreach (GTK_CONTAINER (slot->extra_location_widgets),
                           remove_all,
                           slot->extra_location_widgets);
    gtk_widget_hide (slot->extra_location_frame);
}

void
scatola_window_slot_add_extra_location_widget (ScatolaWindowSlot *slot,
        GtkWidget *widget)
{
    gtk_box_pack_start (GTK_BOX (slot->extra_location_widgets),
                        widget, TRUE, TRUE, 0);
    gtk_widget_show (slot->extra_location_frame);
}

void
scatola_window_slot_add_current_location_to_history_list (ScatolaWindowSlot *slot)
{

    if ((slot->pane->window == NULL || !SCATOLA_IS_DESKTOP_WINDOW (slot->pane->window)) &&
            scatola_add_bookmark_to_history_list (slot->current_location_bookmark))
    {
        scatola_send_history_list_changed ();
    }
}

/* returns either the pending or the actual current location - used by side panes. */
static char *
real_slot_info_get_current_location (ScatolaWindowSlotInfo *info)
{
    ScatolaWindowSlot *slot;

    slot = SCATOLA_WINDOW_SLOT (info);

    if (slot->pending_location != NULL)
    {
        return g_file_get_uri (slot->pending_location);
    }

    if (slot->location != NULL)
    {
        return g_file_get_uri (slot->location);
    }

    g_assert_not_reached ();
    return NULL;
}

static ScatolaView *
real_slot_info_get_current_view (ScatolaWindowSlotInfo *info)
{
    ScatolaWindowSlot *slot;

    slot = SCATOLA_WINDOW_SLOT (info);

    if (slot->content_view != NULL)
    {
        return g_object_ref (slot->content_view);
    }
    else if (slot->new_content_view)
    {
        return g_object_ref (slot->new_content_view);
    }

    return NULL;
}

static void
scatola_window_slot_dispose (GObject *object)
{
    ScatolaWindowSlot *slot;
    GtkWidget *widget;

    slot = SCATOLA_WINDOW_SLOT (object);

    if (slot->content_view)
    {
        widget = scatola_view_get_widget (slot->content_view);
        gtk_widget_destroy (widget);
        g_object_unref (slot->content_view);
        slot->content_view = NULL;
    }

    if (slot->new_content_view)
    {
        widget = scatola_view_get_widget (slot->new_content_view);
        gtk_widget_destroy (widget);
        g_object_unref (slot->new_content_view);
        slot->new_content_view = NULL;
    }

    scatola_window_slot_set_viewed_file (slot, NULL);

    g_clear_object (&slot->location);

    g_list_free_full (slot->pending_selection, g_free);
    slot->pending_selection = NULL;

    if (slot->current_location_bookmark != NULL)
    {
        g_object_unref (slot->current_location_bookmark);
        slot->current_location_bookmark = NULL;
    }
    if (slot->last_location_bookmark != NULL)
    {
        g_object_unref (slot->last_location_bookmark);
        slot->last_location_bookmark = NULL;
    }

    if (slot->find_mount_cancellable != NULL)
    {
        g_cancellable_cancel (slot->find_mount_cancellable);
        slot->find_mount_cancellable = NULL;
    }

    slot->pane = NULL;

    g_free (slot->title);
    slot->title = NULL;

    g_free (slot->status_text);
    slot->status_text = NULL;

    G_OBJECT_CLASS (parent_class)->dispose (object);
}

static void
scatola_window_slot_info_iface_init (ScatolaWindowSlotInfoIface *iface)
{
    iface->active = scatola_window_slot_active;
    iface->inactive = scatola_window_slot_inactive;
    iface->get_window = scatola_window_slot_get_window;
    iface->get_selection_count = scatola_window_slot_get_selection_count;
    iface->get_current_location = real_slot_info_get_current_location;
    iface->get_current_view = real_slot_info_get_current_view;
    iface->set_status = scatola_window_slot_set_status;
    iface->get_title = scatola_window_slot_get_title;
    iface->open_location = scatola_window_slot_open_location_full;
    iface->make_hosting_pane_active = scatola_window_slot_make_hosting_pane_active;
}

