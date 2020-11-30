/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*-

   scatola-desktop-directory.c: Subclass of ScatolaDirectory to implement
   a virtual directory consisting of the desktop directory and the desktop
   icons

   Copyright (C) 2003 Red Hat, Inc.

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

   Author: Alexander Larsson <alexl@redhat.com>
*/

#include <config.h>
#include "scatola-desktop-directory.h"

#include "scatola-directory-private.h"
#include "scatola-file.h"
#include "scatola-file-private.h"
#include "scatola-file-utilities.h"
#include "scatola-global-preferences.h"
#include <eel/eel-glib-extensions.h>
#include <eel/eel-gtk-macros.h>
#include <gtk/gtk.h>

struct ScatolaDesktopDirectoryDetails
{
    ScatolaDirectory *real_directory;
    GHashTable *callbacks;
    GHashTable *monitors;
};

typedef struct
{
    ScatolaDesktopDirectory *desktop_dir;
    ScatolaDirectoryCallback callback;
    gpointer callback_data;

    ScatolaFileAttributes wait_for_attributes;
    gboolean wait_for_file_list;

    GList *non_ready_directories;
    GList *merged_file_list;
} MergedCallback;


typedef struct
{
    ScatolaDesktopDirectory *desktop_dir;

    gboolean monitor_hidden_files;
    ScatolaFileAttributes monitor_attributes;
} MergedMonitor;

static void desktop_directory_changed_callback (gpointer data);

G_DEFINE_TYPE (ScatolaDesktopDirectory, scatola_desktop_directory,
               SCATOLA_TYPE_DIRECTORY);
#define parent_class scatola_desktop_directory_parent_class

static gboolean
desktop_contains_file (ScatolaDirectory *directory,
                       ScatolaFile *file)
{
    ScatolaDesktopDirectory *desktop;

    desktop = SCATOLA_DESKTOP_DIRECTORY (directory);

    if (scatola_directory_contains_file (desktop->details->real_directory, file))
    {
        return TRUE;
    }

    return file->details->directory == directory;
}

static guint
merged_callback_hash (gconstpointer merged_callback_as_pointer)
{
    const MergedCallback *merged_callback;

    merged_callback = merged_callback_as_pointer;
    return GPOINTER_TO_UINT (merged_callback->callback)
           ^ GPOINTER_TO_UINT (merged_callback->callback_data);
}

static gboolean
merged_callback_equal (gconstpointer merged_callback_as_pointer,
                       gconstpointer merged_callback_as_pointer_2)
{
    const MergedCallback *merged_callback, *merged_callback_2;

    merged_callback = merged_callback_as_pointer;
    merged_callback_2 = merged_callback_as_pointer_2;

    return merged_callback->callback == merged_callback_2->callback
           && merged_callback->callback_data == merged_callback_2->callback_data;
}

static void
merged_callback_destroy (MergedCallback *merged_callback)
{
    g_assert (merged_callback != NULL);
    g_assert (SCATOLA_IS_DESKTOP_DIRECTORY (merged_callback->desktop_dir));

    g_list_free (merged_callback->non_ready_directories);
    scatola_file_list_free (merged_callback->merged_file_list);
    g_free (merged_callback);
}

static void
merged_callback_check_done (MergedCallback *merged_callback)
{
    /* Check if we are ready. */
    if (merged_callback->non_ready_directories != NULL)
    {
        return;
    }

    /* Remove from the hash table before sending it. */
    g_hash_table_steal (merged_callback->desktop_dir->details->callbacks, merged_callback);

    /* We are ready, so do the real callback. */
    (* merged_callback->callback) (SCATOLA_DIRECTORY (merged_callback->desktop_dir),
                                   merged_callback->merged_file_list,
                                   merged_callback->callback_data);

    /* And we are done. */
    merged_callback_destroy (merged_callback);
}

static void
merged_callback_remove_directory (MergedCallback *merged_callback,
                                  ScatolaDirectory *directory)
{
    merged_callback->non_ready_directories = g_list_remove
            (merged_callback->non_ready_directories, directory);
    merged_callback_check_done (merged_callback);
}

static void
directory_ready_callback (ScatolaDirectory *directory,
                          GList *files,
                          gpointer callback_data)
{
    MergedCallback *merged_callback;

    g_assert (SCATOLA_IS_DIRECTORY (directory));
    g_assert (callback_data != NULL);

    merged_callback = callback_data;
    g_assert (g_list_find (merged_callback->non_ready_directories, directory) != NULL);

    /* Update based on this call. */
    merged_callback->merged_file_list = g_list_concat
                                        (merged_callback->merged_file_list,
                                         scatola_file_list_copy (files));

    /* Check if we are ready. */
    merged_callback_remove_directory (merged_callback, directory);
}

static void
desktop_call_when_ready (ScatolaDirectory *directory,
                         ScatolaFileAttributes file_attributes,
                         gboolean wait_for_file_list,
                         ScatolaDirectoryCallback callback,
                         gpointer callback_data)
{
    ScatolaDesktopDirectory *desktop;
    MergedCallback search_key, *merged_callback;

    desktop = SCATOLA_DESKTOP_DIRECTORY (directory);

    /* Check to be sure we aren't overwriting. */
    search_key.callback = callback;
    search_key.callback_data = callback_data;
    if (g_hash_table_lookup (desktop->details->callbacks, &search_key) != NULL)
    {
        g_warning ("tried to add a new callback while an old one was pending");
        return;
    }

    /* Create a merged_callback record. */
    merged_callback = g_new0 (MergedCallback, 1);
    merged_callback->desktop_dir = desktop;
    merged_callback->callback = callback;
    merged_callback->callback_data = callback_data;
    merged_callback->wait_for_attributes = file_attributes;
    merged_callback->wait_for_file_list = wait_for_file_list;
    merged_callback->non_ready_directories = g_list_prepend
            (merged_callback->non_ready_directories, directory);
    merged_callback->non_ready_directories = g_list_prepend
            (merged_callback->non_ready_directories, desktop->details->real_directory);


    merged_callback->merged_file_list = g_list_concat (NULL,
                                        scatola_file_list_copy (directory->details->file_list));

    /* Put it in the hash table. */
    g_hash_table_insert (desktop->details->callbacks,
                         merged_callback, merged_callback);

    /* Now tell all the directories about it. */
    scatola_directory_call_when_ready
    (desktop->details->real_directory,
     merged_callback->wait_for_attributes,
     merged_callback->wait_for_file_list,
     directory_ready_callback, merged_callback);
    scatola_directory_call_when_ready_internal
    (directory,
     NULL,
     merged_callback->wait_for_attributes,
     merged_callback->wait_for_file_list,
     directory_ready_callback,
     NULL,
     merged_callback);

}

static void
desktop_cancel_callback (ScatolaDirectory *directory,
                         ScatolaDirectoryCallback callback,
                         gpointer callback_data)
{
    ScatolaDesktopDirectory *desktop;
    MergedCallback search_key, *merged_callback;
    GList *node;

    desktop = SCATOLA_DESKTOP_DIRECTORY (directory);

    /* Find the entry in the table. */
    search_key.callback = callback;
    search_key.callback_data = callback_data;
    merged_callback = g_hash_table_lookup (desktop->details->callbacks, &search_key);
    if (merged_callback == NULL)
    {
        return;
    }

    /* Remove from the hash table before working with it. */
    g_hash_table_steal (merged_callback->desktop_dir->details->callbacks, merged_callback);

    /* Tell all the directories to cancel the call. */
    for (node = merged_callback->non_ready_directories; node != NULL; node = node->next)
    {
        scatola_directory_cancel_callback
        (node->data,
         directory_ready_callback, merged_callback);
    }
    merged_callback_destroy (merged_callback);
}

static void
merged_monitor_destroy (MergedMonitor *monitor)
{
    ScatolaDesktopDirectory *desktop;

    desktop = monitor->desktop_dir;

    /* Call through to the real directory remove calls. */
    scatola_directory_file_monitor_remove (desktop->details->real_directory, monitor);

    scatola_directory_monitor_remove_internal (SCATOLA_DIRECTORY (desktop), NULL, monitor);

    g_free (monitor);
}

static void
build_merged_callback_list (ScatolaDirectory *directory,
                            GList *file_list,
                            gpointer callback_data)
{
    GList **merged_list;

    merged_list = callback_data;
    *merged_list = g_list_concat (*merged_list,
                                  scatola_file_list_copy (file_list));
}

static void
desktop_monitor_add (ScatolaDirectory *directory,
                     gconstpointer client,
                     gboolean monitor_hidden_files,
                     ScatolaFileAttributes file_attributes,
                     ScatolaDirectoryCallback callback,
                     gpointer callback_data)
{
    ScatolaDesktopDirectory *desktop;
    MergedMonitor *monitor;
    GList *merged_callback_list;

    desktop = SCATOLA_DESKTOP_DIRECTORY (directory);

    /* Map the client to a unique value so this doesn't interfere
     * with direct monitoring of the directory by the same client.
     */
    monitor = g_hash_table_lookup (desktop->details->monitors, client);
    if (monitor != NULL)
    {
        g_assert (monitor->desktop_dir == desktop);
    }
    else
    {
        monitor = g_new0 (MergedMonitor, 1);
        monitor->desktop_dir = desktop;
        g_hash_table_insert (desktop->details->monitors,
                             (gpointer) client, monitor);
    }
    monitor->monitor_hidden_files = monitor_hidden_files;
    monitor->monitor_attributes = file_attributes;

    /* Call through to the real directory add calls. */
    merged_callback_list = NULL;

    /* Call up to real dir */
    scatola_directory_file_monitor_add
    (desktop->details->real_directory, monitor,
     monitor_hidden_files,
     file_attributes,
     build_merged_callback_list, &merged_callback_list);

    /* Handle the desktop part */
    merged_callback_list = g_list_concat (merged_callback_list,
                                          scatola_file_list_copy (directory->details->file_list));


    if (callback != NULL)
    {
        (* callback) (directory, merged_callback_list, callback_data);
    }
    scatola_file_list_free (merged_callback_list);
}

static void
desktop_monitor_remove (ScatolaDirectory *directory,
                        gconstpointer client)
{
    ScatolaDesktopDirectory *desktop;
    MergedMonitor *monitor;

    desktop = SCATOLA_DESKTOP_DIRECTORY (directory);

    monitor = g_hash_table_lookup (desktop->details->monitors, client);
    if (monitor == NULL)
    {
        return;
    }

    g_hash_table_remove (desktop->details->monitors, client);
}

static void
desktop_force_reload (ScatolaDirectory *directory)
{
    ScatolaDesktopDirectory *desktop;

    desktop = SCATOLA_DESKTOP_DIRECTORY (directory);

    scatola_directory_force_reload (desktop->details->real_directory);

    /* We don't invalidate the files in desktop, since they are always
       up to date. (And we don't ever want to mark them invalid.) */
}

static gboolean
desktop_are_all_files_seen (ScatolaDirectory *directory)
{
    ScatolaDesktopDirectory *desktop;

    desktop = SCATOLA_DESKTOP_DIRECTORY (directory);

    if (!scatola_directory_are_all_files_seen (desktop->details->real_directory))
    {
        return FALSE;
    }

    return TRUE;
}

static gboolean
desktop_is_not_empty (ScatolaDirectory *directory)
{
    ScatolaDesktopDirectory *desktop;

    desktop = SCATOLA_DESKTOP_DIRECTORY (directory);

    if (scatola_directory_is_not_empty (desktop->details->real_directory))
    {
        return TRUE;
    }

    return directory->details->file_list != NULL;
}

static GList *
desktop_get_file_list (ScatolaDirectory *directory)
{
    GList *real_dir_file_list, *desktop_dir_file_list = NULL;

    real_dir_file_list = scatola_directory_get_file_list
                         (SCATOLA_DESKTOP_DIRECTORY (directory)->details->real_directory);
    desktop_dir_file_list = EEL_CALL_PARENT_WITH_RETURN_VALUE (SCATOLA_DIRECTORY_CLASS, get_file_list, (directory));

    return g_list_concat (real_dir_file_list, desktop_dir_file_list);
}

ScatolaDirectory *
scatola_desktop_directory_get_real_directory (ScatolaDesktopDirectory *desktop)
{
    scatola_directory_ref (desktop->details->real_directory);
    return desktop->details->real_directory;
}


static void
desktop_finalize (GObject *object)
{
    ScatolaDesktopDirectory *desktop;

    desktop = SCATOLA_DESKTOP_DIRECTORY (object);

    scatola_directory_unref (desktop->details->real_directory);

    g_hash_table_destroy (desktop->details->callbacks);
    g_hash_table_destroy (desktop->details->monitors);
    g_free (desktop->details);

    g_signal_handlers_disconnect_by_func (scatola_preferences,
                                          desktop_directory_changed_callback,
                                          desktop);

    G_OBJECT_CLASS (scatola_desktop_directory_parent_class)->finalize (object);
}

static void
done_loading_callback (ScatolaDirectory *real_directory,
                       ScatolaDesktopDirectory *desktop)
{
    scatola_directory_emit_done_loading (SCATOLA_DIRECTORY (desktop));
}


static void
forward_files_added_cover (ScatolaDirectory *real_directory,
                           GList *files,
                           gpointer callback_data)
{
    scatola_directory_emit_files_added (SCATOLA_DIRECTORY (callback_data), files);
}

static void
forward_files_changed_cover (ScatolaDirectory *real_directory,
                             GList *files,
                             gpointer callback_data)
{
    scatola_directory_emit_files_changed (SCATOLA_DIRECTORY (callback_data), files);
}

static void
update_desktop_directory (ScatolaDesktopDirectory *desktop)
{
    char *desktop_path;
    char *desktop_uri;
    ScatolaDirectory *real_directory;

    real_directory = desktop->details->real_directory;
    if (real_directory != NULL)
    {
        g_hash_table_remove_all (desktop->details->callbacks);
        g_hash_table_remove_all (desktop->details->monitors);

        g_signal_handlers_disconnect_by_func (real_directory, done_loading_callback, desktop);
        g_signal_handlers_disconnect_by_func (real_directory, forward_files_added_cover, desktop);
        g_signal_handlers_disconnect_by_func (real_directory, forward_files_changed_cover, desktop);

        scatola_directory_unref (real_directory);
    }

    desktop_path = scatola_get_desktop_directory ();
    desktop_uri = g_filename_to_uri (desktop_path, NULL, NULL);
    real_directory = scatola_directory_get_by_uri (desktop_uri);
    g_free (desktop_uri);
    g_free (desktop_path);

    g_signal_connect_object (real_directory, "done_loading",
                             G_CALLBACK (done_loading_callback), desktop, 0);
    g_signal_connect_object (real_directory, "files_added",
                             G_CALLBACK (forward_files_added_cover), desktop, 0);
    g_signal_connect_object (real_directory, "files_changed",
                             G_CALLBACK (forward_files_changed_cover), desktop, 0);

    desktop->details->real_directory = real_directory;
}

static void
desktop_directory_changed_callback (gpointer data)
{
    update_desktop_directory (SCATOLA_DESKTOP_DIRECTORY (data));
    scatola_directory_force_reload (SCATOLA_DIRECTORY (data));
}

static void
scatola_desktop_directory_init (ScatolaDesktopDirectory *desktop)
{
    desktop->details = g_new0 (ScatolaDesktopDirectoryDetails, 1);

    desktop->details->callbacks = g_hash_table_new_full
                                  (merged_callback_hash, merged_callback_equal,
                                   NULL, (GDestroyNotify)merged_callback_destroy);
    desktop->details->monitors = g_hash_table_new_full (NULL, NULL,
                                 NULL, (GDestroyNotify)merged_monitor_destroy);

    update_desktop_directory (SCATOLA_DESKTOP_DIRECTORY (desktop));

    g_signal_connect_swapped (scatola_preferences, "changed::" SCATOLA_PREFERENCES_DESKTOP_IS_HOME_DIR,
                              G_CALLBACK(desktop_directory_changed_callback),
                              desktop);
}

static void
scatola_desktop_directory_class_init (ScatolaDesktopDirectoryClass *class)
{
    ScatolaDirectoryClass *directory_class;

    directory_class = SCATOLA_DIRECTORY_CLASS (class);

    G_OBJECT_CLASS (class)->finalize = desktop_finalize;

    directory_class->contains_file = desktop_contains_file;
    directory_class->call_when_ready = desktop_call_when_ready;
    directory_class->cancel_callback = desktop_cancel_callback;
    directory_class->file_monitor_add = desktop_monitor_add;
    directory_class->file_monitor_remove = desktop_monitor_remove;
    directory_class->force_reload = desktop_force_reload;
    directory_class->are_all_files_seen = desktop_are_all_files_seen;
    directory_class->is_not_empty = desktop_is_not_empty;
    /* Override get_file_list so that we can return the list of files
     * in ScatolaDesktopDirectory->details->real_directory,
     * in addition to the list of standard desktop icons on the desktop.
     */
    directory_class->get_file_list = desktop_get_file_list;
}

