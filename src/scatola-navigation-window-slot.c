/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*-

   scatola-navigation-window-slot.c: Scatola navigation window slot

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
#include "scatola-window-private.h"
#include "scatola-search-bar.h"
#include "scatola-navigation-window-pane.h"
#include <libscatola-private/scatola-window-slot-info.h>
#include <libscatola-private/scatola-file.h>
#include <eel/eel-gtk-macros.h>

static void scatola_navigation_window_slot_init       (ScatolaNavigationWindowSlot *slot);
static void scatola_navigation_window_slot_class_init (ScatolaNavigationWindowSlotClass *class);

G_DEFINE_TYPE (ScatolaNavigationWindowSlot, scatola_navigation_window_slot, SCATOLA_TYPE_WINDOW_SLOT)
#define parent_class scatola_navigation_window_slot_parent_class

gboolean
scatola_navigation_window_slot_should_close_with_mount (ScatolaNavigationWindowSlot *slot,
        GMount *mount)
{
    ScatolaBookmark *bookmark;
    GFile *mount_location, *bookmark_location;
    GList *l;
    gboolean close_with_mount;

    if (slot->parent.pane->window->details->initiated_unmount)
    {
        return FALSE;
    }

    mount_location = g_mount_get_root (mount);

    close_with_mount = TRUE;

    for (l = slot->back_list; l != NULL; l = l->next)
    {
        bookmark = SCATOLA_BOOKMARK (l->data);

        bookmark_location = scatola_bookmark_get_location (bookmark);
        close_with_mount &= g_file_has_prefix (bookmark_location, mount_location) ||
                            g_file_equal (bookmark_location, mount_location);
        g_object_unref (bookmark_location);

        if (!close_with_mount)
        {
            break;
        }
    }

    close_with_mount &= g_file_has_prefix (SCATOLA_WINDOW_SLOT (slot)->location, mount_location) ||
                        g_file_equal (SCATOLA_WINDOW_SLOT (slot)->location, mount_location);

    /* we could also consider the forward list here, but since the “go home” request
     * in scatola-window-manager-views.c:mount_removed_callback() would discard those
     * anyway, we don't consider them.
     */

    g_object_unref (mount_location);

    return close_with_mount;
}

void
scatola_navigation_window_slot_clear_forward_list (ScatolaNavigationWindowSlot *slot)
{
    g_assert (SCATOLA_IS_NAVIGATION_WINDOW_SLOT (slot));

    g_list_free_full (slot->forward_list, g_object_unref);
    slot->forward_list = NULL;
}

void
scatola_navigation_window_slot_clear_back_list (ScatolaNavigationWindowSlot *slot)
{
    g_assert (SCATOLA_IS_NAVIGATION_WINDOW_SLOT (slot));

    g_list_free_full (slot->back_list, g_object_unref);
    slot->back_list = NULL;
}

static void
query_editor_changed_callback (ScatolaSearchBar *bar,
                               ScatolaQuery *query,
                               gboolean reload,
                               ScatolaWindowSlot *slot)
{
    ScatolaDirectory *directory;

    g_assert (SCATOLA_IS_FILE (slot->viewed_file));

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
scatola_navigation_window_slot_update_query_editor (ScatolaWindowSlot *slot)
{
    ScatolaDirectory *directory;
    ScatolaSearchDirectory *search_directory;
    ScatolaQuery *query;
    GtkWidget *query_editor;

    g_assert (slot->pane->window != NULL);

    query_editor = NULL;

    directory = scatola_directory_get (slot->location);
    if (SCATOLA_IS_SEARCH_DIRECTORY (directory))
    {
        search_directory = SCATOLA_SEARCH_DIRECTORY (directory);

        if (scatola_search_directory_is_saved_search (search_directory))
        {
            query_editor = scatola_query_editor_new (TRUE,
                                                  scatola_search_directory_is_indexed (search_directory));
        }
        else
        {
            query_editor = scatola_query_editor_new_with_bar (FALSE,
                           scatola_search_directory_is_indexed (search_directory),
                           slot->pane->window->details->active_pane->active_slot == slot,
                           SCATOLA_SEARCH_BAR (SCATOLA_NAVIGATION_WINDOW_PANE (slot->pane)->search_bar),
                           slot);
        }
    }

    slot->query_editor = SCATOLA_QUERY_EDITOR (query_editor);

    if (query_editor != NULL)
    {
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

        scatola_window_slot_add_extra_location_widget (slot, query_editor);
        gtk_widget_show (query_editor);
        scatola_query_editor_grab_focus (SCATOLA_QUERY_EDITOR (query_editor));
    }

    scatola_directory_unref (directory);
}

static void
scatola_navigation_window_slot_active (ScatolaWindowSlot *slot)
{
    ScatolaNavigationWindow *window;
    ScatolaNavigationWindowPane *pane;
    int page_num;

    pane = SCATOLA_NAVIGATION_WINDOW_PANE (slot->pane);
    window = SCATOLA_NAVIGATION_WINDOW (slot->pane->window);

    page_num = gtk_notebook_page_num (GTK_NOTEBOOK (pane->notebook),
                                      slot->content_box);
    g_assert (page_num >= 0);

    gtk_notebook_set_current_page (GTK_NOTEBOOK (pane->notebook), page_num);

    EEL_CALL_PARENT (SCATOLA_WINDOW_SLOT_CLASS, active, (slot));

    if (slot->viewed_file != NULL)
    {
        scatola_navigation_window_load_extension_toolbar_items (window);
    }
}

static void
scatola_navigation_window_slot_dispose (GObject *object)
{
    ScatolaNavigationWindowSlot *slot;

    slot = SCATOLA_NAVIGATION_WINDOW_SLOT (object);

    scatola_navigation_window_slot_clear_forward_list (slot);
    scatola_navigation_window_slot_clear_back_list (slot);

    G_OBJECT_CLASS (parent_class)->dispose (object);
}

static void
scatola_navigation_window_slot_init (ScatolaNavigationWindowSlot *slot)
{
}

static void
scatola_navigation_window_slot_class_init (ScatolaNavigationWindowSlotClass *class)
{
    SCATOLA_WINDOW_SLOT_CLASS (class)->active = scatola_navigation_window_slot_active;
    SCATOLA_WINDOW_SLOT_CLASS (class)->update_query_editor = scatola_navigation_window_slot_update_query_editor;

    G_OBJECT_CLASS (class)->dispose = scatola_navigation_window_slot_dispose;
}

