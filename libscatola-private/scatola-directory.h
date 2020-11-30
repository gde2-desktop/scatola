/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*-

   scatola-directory.h: Scatola directory model.

   Copyright (C) 1999, 2000, 2001 Eazel, Inc.

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

   Author: Darin Adler <darin@bentspoon.com>
*/

#ifndef SCATOLA_DIRECTORY_H
#define SCATOLA_DIRECTORY_H

#include <gtk/gtk.h>
#include <gio/gio.h>
#include <libscatola-private/scatola-file-attributes.h>

/* ScatolaDirectory is a class that manages the model for a directory,
   real or virtual, for Scatola, mainly the file-manager component. The directory is
   responsible for managing both real data and cached metadata. On top of
   the file system independence provided by gio, the directory
   object also provides:

       1) A synchronization framework, which notifies via signals as the
          set of known files changes.
       2) An abstract interface for getting attributes and performing
          operations on files.
*/

#define SCATOLA_TYPE_DIRECTORY scatola_directory_get_type()
#define SCATOLA_DIRECTORY(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), SCATOLA_TYPE_DIRECTORY, ScatolaDirectory))
#define SCATOLA_DIRECTORY_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), SCATOLA_TYPE_DIRECTORY, ScatolaDirectoryClass))
#define SCATOLA_IS_DIRECTORY(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SCATOLA_TYPE_DIRECTORY))
#define SCATOLA_IS_DIRECTORY_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), SCATOLA_TYPE_DIRECTORY))
#define SCATOLA_DIRECTORY_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), SCATOLA_TYPE_DIRECTORY, ScatolaDirectoryClass))

/* ScatolaFile is defined both here and in scatola-file.h. */
#ifndef SCATOLA_FILE_DEFINED
#define SCATOLA_FILE_DEFINED
typedef struct ScatolaFile ScatolaFile;
#endif

typedef struct ScatolaDirectoryDetails ScatolaDirectoryDetails;

typedef struct
{
    GObject object;
    ScatolaDirectoryDetails *details;
} ScatolaDirectory;

typedef void (*ScatolaDirectoryCallback) (ScatolaDirectory *directory,
                                       GList             *files,
                                       gpointer           callback_data);

typedef struct
{
    GObjectClass parent_class;

    /*** Notification signals for clients to connect to. ***/

    /* The files_added signal is emitted as the directory model
     * discovers new files.
     */
    void     (* files_added)         (ScatolaDirectory          *directory,
                                      GList                      *added_files);

    /* The files_changed signal is emitted as changes occur to
     * existing files that are noticed by the synchronization framework,
     * including when an old file has been deleted. When an old file
     * has been deleted, this is the last chance to forget about these
     * file objects, which are about to be unref'd. Use a call to
     * scatola_file_is_gone () to test for this case.
     */
    void     (* files_changed)       (ScatolaDirectory         *directory,
                                      GList                     *changed_files);

    /* The done_loading signal is emitted when a directory load
     * request completes. This is needed because, at least in the
     * case where the directory is empty, the caller will receive
     * no kind of notification at all when a directory load
     * initiated by `scatola_directory_file_monitor_add' completes.
     */
    void     (* done_loading)        (ScatolaDirectory         *directory);

    void     (* load_error)          (ScatolaDirectory         *directory,
                                      GError                    *error);

    /*** Virtual functions for subclasses to override. ***/
    gboolean (* contains_file)       (ScatolaDirectory         *directory,
                                      ScatolaFile              *file);
    void     (* call_when_ready)     (ScatolaDirectory         *directory,
                                      ScatolaFileAttributes     file_attributes,
                                      gboolean                   wait_for_file_list,
                                      ScatolaDirectoryCallback  callback,
                                      gpointer                   callback_data);
    void     (* cancel_callback)     (ScatolaDirectory         *directory,
                                      ScatolaDirectoryCallback  callback,
                                      gpointer                   callback_data);
    void     (* file_monitor_add)    (ScatolaDirectory          *directory,
                                      gconstpointer              client,
                                      gboolean                   monitor_hidden_files,
                                      ScatolaFileAttributes     monitor_attributes,
                                      ScatolaDirectoryCallback  initial_files_callback,
                                      gpointer                   callback_data);
    void     (* file_monitor_remove) (ScatolaDirectory         *directory,
                                      gconstpointer              client);
    void     (* force_reload)        (ScatolaDirectory         *directory);
    gboolean (* are_all_files_seen)  (ScatolaDirectory         *directory);
    gboolean (* is_not_empty)        (ScatolaDirectory         *directory);
    char *	 (* get_name_for_self_as_new_file) (ScatolaDirectory *directory);

    /* get_file_list is a function pointer that subclasses may override to
     * customize collecting the list of files in a directory.
     * For example, the ScatolaDesktopDirectory overrides this so that it can
     * merge together the list of files in the $HOME/Desktop directory with
     * the list of standard icons (Computer, Home, Trash) on the desktop.
     */
    GList *	 (* get_file_list)	 (ScatolaDirectory *directory);

    /* Should return FALSE if the directory is read-only and doesn't
     * allow setting of metadata.
     * An example of this is the search directory.
     */
    gboolean (* is_editable)         (ScatolaDirectory *directory);
} ScatolaDirectoryClass;

/* Basic GObject requirements. */
GType              scatola_directory_get_type                 (void);

/* Get a directory given a uri.
 * Creates the appropriate subclass given the uri mappings.
 * Returns a referenced object, not a floating one. Unref when finished.
 * If two windows are viewing the same uri, the directory object is shared.
 */
ScatolaDirectory *scatola_directory_get                      (GFile                     *location);
ScatolaDirectory *scatola_directory_get_by_uri               (const char                *uri);
ScatolaDirectory *scatola_directory_get_for_file             (ScatolaFile              *file);

/* Covers for g_object_ref and g_object_unref that provide two conveniences:
 * 1) Using these is type safe.
 * 2) You are allowed to call these with NULL,
 */
ScatolaDirectory *scatola_directory_ref                      (ScatolaDirectory         *directory);
void               scatola_directory_unref                    (ScatolaDirectory         *directory);

/* Access to a URI. */
char *             scatola_directory_get_uri                  (ScatolaDirectory         *directory);
GFile *            scatola_directory_get_location             (ScatolaDirectory         *directory);

/* Is this file still alive and in this directory? */
gboolean           scatola_directory_contains_file            (ScatolaDirectory         *directory,
        ScatolaFile              *file);

/* Get (and ref) a ScatolaFile object for this directory. */
ScatolaFile *     scatola_directory_get_corresponding_file   (ScatolaDirectory         *directory);

/* Waiting for data that's read asynchronously.
 * The file attribute and metadata keys are for files in the directory.
 */
void               scatola_directory_call_when_ready          (ScatolaDirectory         *directory,
        ScatolaFileAttributes     file_attributes,
        gboolean                   wait_for_all_files,
        ScatolaDirectoryCallback  callback,
        gpointer                   callback_data);
void               scatola_directory_cancel_callback          (ScatolaDirectory         *directory,
        ScatolaDirectoryCallback  callback,
        gpointer                   callback_data);


/* Monitor the files in a directory. */
void               scatola_directory_file_monitor_add         (ScatolaDirectory         *directory,
        gconstpointer              client,
        gboolean                   monitor_hidden_files,
        ScatolaFileAttributes     attributes,
        ScatolaDirectoryCallback  initial_files_callback,
        gpointer                   callback_data);
void               scatola_directory_file_monitor_remove      (ScatolaDirectory         *directory,
        gconstpointer              client);
void               scatola_directory_force_reload             (ScatolaDirectory         *directory);

/* Get a list of all files currently known in the directory. */
GList *            scatola_directory_get_file_list            (ScatolaDirectory         *directory);

GList *            scatola_directory_match_pattern            (ScatolaDirectory         *directory,
        const char *glob);


/* Return true if the directory has information about all the files.
 * This will be false until the directory has been read at least once.
 */
gboolean           scatola_directory_are_all_files_seen       (ScatolaDirectory         *directory);

/* Return true if the directory is local. */
gboolean           scatola_directory_is_local                 (ScatolaDirectory         *directory);

gboolean           scatola_directory_is_in_trash              (ScatolaDirectory         *directory);

/* Return false if directory contains anything besides a Scatola metafile.
 * Only valid if directory is monitored. Used by the Trash monitor.
 */
gboolean           scatola_directory_is_not_empty             (ScatolaDirectory         *directory);

/* Convenience functions for dealing with a list of ScatolaDirectory objects that each have a ref.
 * These are just convenient names for functions that work on lists of GtkObject *.
 */
GList *            scatola_directory_list_ref                 (GList                     *directory_list);
void               scatola_directory_list_unref               (GList                     *directory_list);
void               scatola_directory_list_free                (GList                     *directory_list);
GList *            scatola_directory_list_copy                (GList                     *directory_list);
GList *            scatola_directory_list_sort_by_uri         (GList                     *directory_list);

/* Fast way to check if a directory is the desktop directory */
gboolean           scatola_directory_is_desktop_directory     (ScatolaDirectory         *directory);

gboolean           scatola_directory_is_editable              (ScatolaDirectory         *directory);


#endif /* SCATOLA_DIRECTORY_H */
