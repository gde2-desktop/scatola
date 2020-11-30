/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*-

   scatola-file.h: Scatola file model.

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

#ifndef SCATOLA_FILE_H
#define SCATOLA_FILE_H

#include <gtk/gtk.h>
#include <gio/gio.h>
#include <libscatola-private/scatola-file-attributes.h>
#include <libscatola-private/scatola-icon-info.h>

/* ScatolaFile is an object used to represent a single element of a
 * ScatolaDirectory. It's lightweight and relies on ScatolaDirectory
 * to do most of the work.
 */

/* ScatolaFile is defined both here and in scatola-directory.h. */
#ifndef SCATOLA_FILE_DEFINED
#define SCATOLA_FILE_DEFINED
typedef struct ScatolaFile ScatolaFile;
#endif

#define SCATOLA_TYPE_FILE scatola_file_get_type()
#define SCATOLA_FILE(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), SCATOLA_TYPE_FILE, ScatolaFile))
#define SCATOLA_FILE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), SCATOLA_TYPE_FILE, ScatolaFileClass))
#define SCATOLA_IS_FILE(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SCATOLA_TYPE_FILE))
#define SCATOLA_IS_FILE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), SCATOLA_TYPE_FILE))
#define SCATOLA_FILE_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), SCATOLA_TYPE_FILE, ScatolaFileClass))

typedef enum
{
    SCATOLA_FILE_SORT_NONE,
    SCATOLA_FILE_SORT_BY_DISPLAY_NAME,
    SCATOLA_FILE_SORT_BY_DIRECTORY,
    SCATOLA_FILE_SORT_BY_SIZE,
    SCATOLA_FILE_SORT_BY_TYPE,
    SCATOLA_FILE_SORT_BY_MTIME,
    SCATOLA_FILE_SORT_BY_ATIME,
    SCATOLA_FILE_SORT_BY_EMBLEMS,
    SCATOLA_FILE_SORT_BY_TRASHED_TIME
} ScatolaFileSortType;

typedef enum
{
    SCATOLA_REQUEST_NOT_STARTED,
    SCATOLA_REQUEST_IN_PROGRESS,
    SCATOLA_REQUEST_DONE
} ScatolaRequestStatus;

typedef enum
{
    SCATOLA_FILE_ICON_FLAGS_NONE = 0,
    SCATOLA_FILE_ICON_FLAGS_USE_THUMBNAILS = (1<<0),
    SCATOLA_FILE_ICON_FLAGS_IGNORE_VISITING = (1<<1),
    SCATOLA_FILE_ICON_FLAGS_EMBEDDING_TEXT = (1<<2),
    SCATOLA_FILE_ICON_FLAGS_FOR_DRAG_ACCEPT = (1<<3),
    SCATOLA_FILE_ICON_FLAGS_FOR_OPEN_FOLDER = (1<<4),
    /* whether the thumbnail size must match the display icon size */
    SCATOLA_FILE_ICON_FLAGS_FORCE_THUMBNAIL_SIZE = (1<<5),
    /* uses the icon of the mount if present */
    SCATOLA_FILE_ICON_FLAGS_USE_MOUNT_ICON = (1<<6),
    /* render the mount icon as an emblem over the regular one */
    SCATOLA_FILE_ICON_FLAGS_USE_MOUNT_ICON_AS_EMBLEM = (1<<7)
} ScatolaFileIconFlags;

/* Emblems sometimes displayed for ScatolaFiles. Do not localize. */
#define SCATOLA_FILE_EMBLEM_NAME_SYMBOLIC_LINK "symbolic-link"
#define SCATOLA_FILE_EMBLEM_NAME_CANT_READ "noread"
#define SCATOLA_FILE_EMBLEM_NAME_CANT_WRITE "nowrite"
#define SCATOLA_FILE_EMBLEM_NAME_TRASH "trash"
#define SCATOLA_FILE_EMBLEM_NAME_NOTE "note"
#define SCATOLA_FILE_EMBLEM_NAME_DESKTOP "desktop"
#define SCATOLA_FILE_EMBLEM_NAME_SHARED "shared"

typedef void (*ScatolaFileCallback)          (ScatolaFile  *file,
        gpointer       callback_data);
typedef void (*ScatolaFileListCallback)      (GList         *file_list,
        gpointer       callback_data);
typedef void (*ScatolaFileOperationCallback) (ScatolaFile  *file,
        GFile         *result_location,
        GError        *error,
        gpointer       callback_data);
typedef int (*ScatolaWidthMeasureCallback)   (const char    *string,
        void	     *context);
typedef char * (*ScatolaTruncateCallback)    (const char    *string,
        int	      width,
        void	     *context);


#define SCATOLA_FILE_ATTRIBUTES_FOR_ICON (SCATOLA_FILE_ATTRIBUTE_INFO | SCATOLA_FILE_ATTRIBUTE_LINK_INFO | SCATOLA_FILE_ATTRIBUTE_THUMBNAIL)

typedef void ScatolaFileListHandle;

/* GObject requirements. */
GType                   scatola_file_get_type                          (void);

/* Getting at a single file. */
ScatolaFile *          scatola_file_get                               (GFile                          *location);
ScatolaFile *          scatola_file_get_by_uri                        (const char                     *uri);

/* Get a file only if the scatola version already exists */
ScatolaFile *          scatola_file_get_existing                      (GFile                          *location);
ScatolaFile *          scatola_file_get_existing_by_uri               (const char                     *uri);

/* Covers for g_object_ref and g_object_unref that provide two conveniences:
 * 1) Using these is type safe.
 * 2) You are allowed to call these with NULL,
 */
ScatolaFile *          scatola_file_ref                               (ScatolaFile                   *file);
void                    scatola_file_unref                             (ScatolaFile                   *file);

/* Monitor the file. */
void                    scatola_file_monitor_add                       (ScatolaFile                   *file,
        gconstpointer                   client,
        ScatolaFileAttributes          attributes);
void                    scatola_file_monitor_remove                    (ScatolaFile                   *file,
        gconstpointer                   client);

/* Waiting for data that's read asynchronously.
 * This interface currently works only for metadata, but could be expanded
 * to other attributes as well.
 */
void                    scatola_file_call_when_ready                   (ScatolaFile                   *file,
        ScatolaFileAttributes          attributes,
        ScatolaFileCallback            callback,
        gpointer                        callback_data);
void                    scatola_file_cancel_call_when_ready            (ScatolaFile                   *file,
        ScatolaFileCallback            callback,
        gpointer                        callback_data);
gboolean                scatola_file_check_if_ready                    (ScatolaFile                   *file,
        ScatolaFileAttributes          attributes);
void                    scatola_file_invalidate_attributes             (ScatolaFile                   *file,
        ScatolaFileAttributes          attributes);
void                    scatola_file_invalidate_all_attributes         (ScatolaFile                   *file);

/* Basic attributes for file objects. */
gboolean                scatola_file_contains_text                     (ScatolaFile                   *file);
gboolean                scatola_file_is_binary                         (ScatolaFile                   *file);
char *                  scatola_file_get_display_name                  (ScatolaFile                   *file);
char *                  scatola_file_get_edit_name                     (ScatolaFile                   *file);
char *                  scatola_file_get_name                          (ScatolaFile                   *file);
GFile *                 scatola_file_get_location                      (ScatolaFile                   *file);
char *			 scatola_file_get_description			 (ScatolaFile			 *file);
char *                  scatola_file_get_uri                           (ScatolaFile                   *file);
char *                  scatola_file_get_uri_scheme                    (ScatolaFile                   *file);
ScatolaFile *          scatola_file_get_parent                        (ScatolaFile                   *file);
GFile *                 scatola_file_get_parent_location               (ScatolaFile                   *file);
char *                  scatola_file_get_parent_uri                    (ScatolaFile                   *file);
char *                  scatola_file_get_parent_uri_for_display        (ScatolaFile                   *file);
gboolean                scatola_file_can_get_size                      (ScatolaFile                   *file);
goffset                 scatola_file_get_size                          (ScatolaFile                   *file);
time_t                  scatola_file_get_mtime                         (ScatolaFile                   *file);
GFileType               scatola_file_get_file_type                     (ScatolaFile                   *file);
char *                  scatola_file_get_mime_type                     (ScatolaFile                   *file);
gboolean                scatola_file_is_mime_type                      (ScatolaFile                   *file,
        const char                     *mime_type);
gboolean                scatola_file_is_launchable                     (ScatolaFile                   *file);
gboolean                scatola_file_is_symbolic_link                  (ScatolaFile                   *file);
gboolean                scatola_file_is_mountpoint                     (ScatolaFile                   *file);
GMount *                scatola_file_get_mount                         (ScatolaFile                   *file);
char *                  scatola_file_get_volume_free_space             (ScatolaFile                   *file);
char *                  scatola_file_get_volume_name                   (ScatolaFile                   *file);
char *                  scatola_file_get_symbolic_link_target_path     (ScatolaFile                   *file);
char *                  scatola_file_get_symbolic_link_target_uri      (ScatolaFile                   *file);
gboolean                scatola_file_is_broken_symbolic_link           (ScatolaFile                   *file);
gboolean                scatola_file_is_scatola_link                  (ScatolaFile                   *file);
gboolean                scatola_file_is_executable                     (ScatolaFile                   *file);
gboolean                scatola_file_is_directory                      (ScatolaFile                   *file);
gboolean                scatola_file_is_user_special_directory         (ScatolaFile                   *file,
        GUserDirectory                 special_directory);
gboolean		scatola_file_is_archive			(ScatolaFile			*file);
gboolean                scatola_file_is_in_trash                       (ScatolaFile                   *file);
gboolean                scatola_file_is_in_desktop                     (ScatolaFile                   *file);
gboolean		scatola_file_is_home				(ScatolaFile                   *file);
gboolean                scatola_file_is_desktop_directory              (ScatolaFile                   *file);
GError *                scatola_file_get_file_info_error               (ScatolaFile                   *file);
gboolean                scatola_file_get_directory_item_count          (ScatolaFile                   *file,
        guint                          *count,
        gboolean                       *count_unreadable);
void                    scatola_file_recompute_deep_counts             (ScatolaFile                   *file);
ScatolaRequestStatus   scatola_file_get_deep_counts                   (ScatolaFile                   *file,
        guint                          *directory_count,
        guint                          *file_count,
        guint                          *unreadable_directory_count,
        goffset               *total_size,
        gboolean                        force);
gboolean                scatola_file_should_show_thumbnail             (ScatolaFile                   *file);
gboolean                scatola_file_should_show_directory_item_count  (ScatolaFile                   *file);
gboolean                scatola_file_should_show_type                  (ScatolaFile                   *file);
GList *                 scatola_file_get_keywords                      (ScatolaFile                   *file);
void                    scatola_file_set_keywords                      (ScatolaFile                   *file,
        GList                          *keywords);
GList *                 scatola_file_get_emblem_icons                  (ScatolaFile                   *file,
        char                          **exclude);
GList *                 scatola_file_get_emblem_pixbufs                (ScatolaFile                   *file,
        int                             size,
        gboolean                        force_size,
        char                          **exclude);
char *                  scatola_file_get_top_left_text                 (ScatolaFile                   *file);
char *                  scatola_file_peek_top_left_text                (ScatolaFile                   *file,
        gboolean                        need_large_text,
        gboolean                       *got_top_left_text);

void                    scatola_file_set_attributes                    (ScatolaFile                   *file,
        GFileInfo                      *attributes,
        ScatolaFileOperationCallback   callback,
        gpointer                        callback_data);
GFilesystemPreviewType  scatola_file_get_filesystem_use_preview        (ScatolaFile *file);

char *                  scatola_file_get_filesystem_id                 (ScatolaFile                   *file);

ScatolaFile *          scatola_file_get_trash_original_file           (ScatolaFile                   *file);

/* Permissions. */
gboolean                scatola_file_can_get_permissions               (ScatolaFile                   *file);
gboolean                scatola_file_can_set_permissions               (ScatolaFile                   *file);
guint                   scatola_file_get_permissions                   (ScatolaFile                   *file);
gboolean                scatola_file_can_get_owner                     (ScatolaFile                   *file);
gboolean                scatola_file_can_set_owner                     (ScatolaFile                   *file);
gboolean                scatola_file_can_get_group                     (ScatolaFile                   *file);
gboolean                scatola_file_can_set_group                     (ScatolaFile                   *file);
char *                  scatola_file_get_owner_name                    (ScatolaFile                   *file);
char *                  scatola_file_get_group_name                    (ScatolaFile                   *file);
GList *                 scatola_get_user_names                         (void);
GList *                 scatola_get_all_group_names                    (void);
GList *                 scatola_file_get_settable_group_names          (ScatolaFile                   *file);
gboolean                scatola_file_can_get_selinux_context           (ScatolaFile                   *file);
char *                  scatola_file_get_selinux_context               (ScatolaFile                   *file);

/* "Capabilities". */
gboolean                scatola_file_can_read                          (ScatolaFile                   *file);
gboolean                scatola_file_can_write                         (ScatolaFile                   *file);
gboolean                scatola_file_can_execute                       (ScatolaFile                   *file);
gboolean                scatola_file_can_rename                        (ScatolaFile                   *file);
gboolean                scatola_file_can_delete                        (ScatolaFile                   *file);
gboolean                scatola_file_can_trash                         (ScatolaFile                   *file);

gboolean                scatola_file_can_mount                         (ScatolaFile                   *file);
gboolean                scatola_file_can_unmount                       (ScatolaFile                   *file);
gboolean                scatola_file_can_eject                         (ScatolaFile                   *file);
gboolean                scatola_file_can_start                         (ScatolaFile                   *file);
gboolean                scatola_file_can_start_degraded                (ScatolaFile                   *file);
gboolean                scatola_file_can_stop                          (ScatolaFile                   *file);
GDriveStartStopType     scatola_file_get_start_stop_type               (ScatolaFile                   *file);
gboolean                scatola_file_can_poll_for_media                (ScatolaFile                   *file);
gboolean                scatola_file_is_media_check_automatic          (ScatolaFile                   *file);

void                    scatola_file_mount                             (ScatolaFile                   *file,
        GMountOperation                *mount_op,
        GCancellable                   *cancellable,
        ScatolaFileOperationCallback   callback,
        gpointer                        callback_data);
void                    scatola_file_unmount                           (ScatolaFile                   *file,
        GMountOperation                *mount_op,
        GCancellable                   *cancellable,
        ScatolaFileOperationCallback   callback,
        gpointer                        callback_data);
void                    scatola_file_eject                             (ScatolaFile                   *file,
        GMountOperation                *mount_op,
        GCancellable                   *cancellable,
        ScatolaFileOperationCallback   callback,
        gpointer                        callback_data);

void                    scatola_file_start                             (ScatolaFile                   *file,
        GMountOperation                *start_op,
        GCancellable                   *cancellable,
        ScatolaFileOperationCallback   callback,
        gpointer                        callback_data);
void                    scatola_file_stop                              (ScatolaFile                   *file,
        GMountOperation                *mount_op,
        GCancellable                   *cancellable,
        ScatolaFileOperationCallback   callback,
        gpointer                        callback_data);
void                    scatola_file_poll_for_media                    (ScatolaFile                   *file);

/* Basic operations for file objects. */
void                    scatola_file_set_owner                         (ScatolaFile                   *file,
        const char                     *user_name_or_id,
        ScatolaFileOperationCallback   callback,
        gpointer                        callback_data);
void                    scatola_file_set_group                         (ScatolaFile                   *file,
        const char                     *group_name_or_id,
        ScatolaFileOperationCallback   callback,
        gpointer                        callback_data);
void                    scatola_file_set_permissions                   (ScatolaFile                   *file,
        guint32                         permissions,
        ScatolaFileOperationCallback   callback,
        gpointer                        callback_data);
void                    scatola_file_rename                            (ScatolaFile                   *file,
        const char                     *new_name,
        ScatolaFileOperationCallback   callback,
        gpointer                        callback_data);
void                    scatola_file_cancel                            (ScatolaFile                   *file,
        ScatolaFileOperationCallback   callback,
        gpointer                        callback_data);

/* Return true if this file has already been deleted.
 * This object will be unref'd after sending the files_removed signal,
 * but it could hang around longer if someone ref'd it.
 */
gboolean                scatola_file_is_gone                           (ScatolaFile                   *file);

/* Return true if this file is not confirmed to have ever really
 * existed. This is true when the ScatolaFile object has been created, but no I/O
 * has yet confirmed the existence of a file by that name.
 */
gboolean                scatola_file_is_not_yet_confirmed              (ScatolaFile                   *file);

/* Simple getting and setting top-level metadata. */
char *                  scatola_file_get_metadata                      (ScatolaFile                   *file,
        const char                     *key,
        const char                     *default_metadata);
GList *                 scatola_file_get_metadata_list                 (ScatolaFile                   *file,
        const char                     *key);
void                    scatola_file_set_metadata                      (ScatolaFile                   *file,
        const char                     *key,
        const char                     *default_metadata,
        const char                     *metadata);
void                    scatola_file_set_metadata_list                 (ScatolaFile                   *file,
        const char                     *key,
        GList                          *list);

/* Covers for common data types. */
gboolean                scatola_file_get_boolean_metadata              (ScatolaFile                   *file,
        const char                     *key,
        gboolean                        default_metadata);
void                    scatola_file_set_boolean_metadata              (ScatolaFile                   *file,
        const char                     *key,
        gboolean                        default_metadata,
        gboolean                        metadata);
int                     scatola_file_get_integer_metadata              (ScatolaFile                   *file,
        const char                     *key,
        int                             default_metadata);
void                    scatola_file_set_integer_metadata              (ScatolaFile                   *file,
        const char                     *key,
        int                             default_metadata,
        int                             metadata);

#define UNDEFINED_TIME ((time_t) (-1))

time_t                  scatola_file_get_time_metadata                 (ScatolaFile                  *file,
        const char                    *key);
void                    scatola_file_set_time_metadata                 (ScatolaFile                  *file,
        const char                    *key,
        time_t                         time);


/* Attributes for file objects as user-displayable strings. */
char *                  scatola_file_get_string_attribute              (ScatolaFile                   *file,
        const char                     *attribute_name);
char *                  scatola_file_get_string_attribute_q            (ScatolaFile                   *file,
        GQuark                          attribute_q);
char *                  scatola_file_get_string_attribute_with_default (ScatolaFile                   *file,
        const char                     *attribute_name);
char *                  scatola_file_get_string_attribute_with_default_q (ScatolaFile                  *file,
        GQuark                          attribute_q);
char *			scatola_file_fit_modified_date_as_string	(ScatolaFile 			*file,
        int				 width,
        ScatolaWidthMeasureCallback    measure_callback,
        ScatolaTruncateCallback	 truncate_callback,
        void				*measure_truncate_context);

/* Matching with another URI. */
gboolean                scatola_file_matches_uri                       (ScatolaFile                   *file,
        const char                     *uri);

/* Is the file local? */
gboolean                scatola_file_is_local                          (ScatolaFile                   *file);

/* Comparing two file objects for sorting */
ScatolaFileSortType    scatola_file_get_default_sort_type             (ScatolaFile                   *file,
        gboolean                       *reversed);
const gchar *           scatola_file_get_default_sort_attribute        (ScatolaFile                   *file,
        gboolean                       *reversed);

int                     scatola_file_compare_for_sort                  (ScatolaFile                   *file_1,
        ScatolaFile                   *file_2,
        ScatolaFileSortType            sort_type,
        gboolean			 directories_first,
        gboolean		  	 reversed);
int                     scatola_file_compare_for_sort_by_attribute     (ScatolaFile                   *file_1,
        ScatolaFile                   *file_2,
        const char                     *attribute,
        gboolean                        directories_first,
        gboolean                        reversed);
int                     scatola_file_compare_for_sort_by_attribute_q   (ScatolaFile                   *file_1,
        ScatolaFile                   *file_2,
        GQuark                          attribute,
        gboolean                        directories_first,
        gboolean                        reversed);
gboolean                scatola_file_is_date_sort_attribute_q          (GQuark                          attribute);

int                     scatola_file_compare_display_name              (ScatolaFile                   *file_1,
        const char                     *pattern);
int                     scatola_file_compare_location                  (ScatolaFile                    *file_1,
        ScatolaFile                    *file_2);

/* filtering functions for use by various directory views */
gboolean                scatola_file_is_hidden_file                    (ScatolaFile                   *file);
gboolean                scatola_file_should_show                       (ScatolaFile                   *file,
        gboolean                        show_hidden,
        gboolean                        show_foreign);
GList                  *scatola_file_list_filter_hidden                (GList                          *files,
        gboolean                        show_hidden);


/* Get the URI that's used when activating the file.
 * Getting this can require reading the contents of the file.
 */
gboolean                scatola_file_is_launcher                       (ScatolaFile                   *file);
gboolean                scatola_file_is_foreign_link                   (ScatolaFile                   *file);
gboolean                scatola_file_is_trusted_link                   (ScatolaFile                   *file);
gboolean                scatola_file_has_activation_uri                (ScatolaFile                   *file);
char *                  scatola_file_get_activation_uri                (ScatolaFile                   *file);
GFile *                 scatola_file_get_activation_location           (ScatolaFile                   *file);

char *                  scatola_file_get_drop_target_uri               (ScatolaFile                   *file);

/* Get custom icon (if specified by metadata or link contents) */
char *                  scatola_file_get_custom_icon                   (ScatolaFile                   *file);


GIcon *                 scatola_file_get_gicon                         (ScatolaFile                   *file,
        ScatolaFileIconFlags           flags);
ScatolaIconInfo *      scatola_file_get_icon                          (ScatolaFile                   *file,
        int                             size,
        ScatolaFileIconFlags           flags);
GdkPixbuf *             scatola_file_get_icon_pixbuf                   (ScatolaFile                   *file,
        int                             size,
        gboolean                        force_size,
        ScatolaFileIconFlags           flags);

gboolean                scatola_file_has_open_window                   (ScatolaFile                   *file);
void                    scatola_file_set_has_open_window               (ScatolaFile                   *file,
        gboolean                        has_open_window);

/* Thumbnailing handling */
gboolean                scatola_file_is_thumbnailing                   (ScatolaFile                   *file);

/* Convenience functions for dealing with a list of ScatolaFile objects that each have a ref.
 * These are just convenient names for functions that work on lists of GtkObject *.
 */
GList *                 scatola_file_list_ref                          (GList                          *file_list);
void                    scatola_file_list_unref                        (GList                          *file_list);
void                    scatola_file_list_free                         (GList                          *file_list);
GList *                 scatola_file_list_copy                         (GList                          *file_list);
GList *			scatola_file_list_sort_by_display_name		(GList				*file_list);
void                    scatola_file_list_call_when_ready              (GList                          *file_list,
        ScatolaFileAttributes          attributes,
        ScatolaFileListHandle        **handle,
        ScatolaFileListCallback        callback,
        gpointer                        callback_data);
void                    scatola_file_list_cancel_call_when_ready       (ScatolaFileListHandle         *handle);

/* Debugging */
void                    scatola_file_dump                              (ScatolaFile                   *file);

typedef struct ScatolaFileDetails ScatolaFileDetails;

struct ScatolaFile
{
    GObject parent_slot;
    ScatolaFileDetails *details;
};

/* This is actually a "protected" type, but it must be here so we can
 * compile the get_date function pointer declaration below.
 */
typedef enum
{
    SCATOLA_DATE_TYPE_MODIFIED,
    SCATOLA_DATE_TYPE_CHANGED,
    SCATOLA_DATE_TYPE_ACCESSED,
    SCATOLA_DATE_TYPE_PERMISSIONS_CHANGED,
    SCATOLA_DATE_TYPE_TRASHED
} ScatolaDateType;

typedef struct
{
    GObjectClass parent_slot;

    /* Subclasses can set this to something other than G_FILE_TYPE_UNKNOWN and
       it will be used as the default file type. This is useful when creating
       a "virtual" ScatolaFile subclass that you can't actually get real
       information about. For exaple ScatolaDesktopDirectoryFile. */
    GFileType default_file_type;

    /* Called when the file notices any change. */
    void                  (* changed)                (ScatolaFile *file);

    /* Called periodically while directory deep count is being computed. */
    void                  (* updated_deep_count_in_progress) (ScatolaFile *file);

    /* Virtual functions (mainly used for trash directory). */
    void                  (* monitor_add)            (ScatolaFile           *file,
            gconstpointer           client,
            ScatolaFileAttributes  attributes);
    void                  (* monitor_remove)         (ScatolaFile           *file,
            gconstpointer           client);
    void                  (* call_when_ready)        (ScatolaFile           *file,
            ScatolaFileAttributes  attributes,
            ScatolaFileCallback    callback,
            gpointer                callback_data);
    void                  (* cancel_call_when_ready) (ScatolaFile           *file,
            ScatolaFileCallback    callback,
            gpointer                callback_data);
    gboolean              (* check_if_ready)         (ScatolaFile           *file,
            ScatolaFileAttributes  attributes);
    gboolean              (* get_item_count)         (ScatolaFile           *file,
            guint                  *count,
            gboolean               *count_unreadable);
    ScatolaRequestStatus (* get_deep_counts)        (ScatolaFile           *file,
            guint                  *directory_count,
            guint                  *file_count,
            guint                  *unreadable_directory_count,
            goffset       *total_size);
    gboolean              (* get_date)               (ScatolaFile           *file,
            ScatolaDateType        type,
            time_t                 *date);
    char *                (* get_where_string)       (ScatolaFile           *file);

    void                  (* set_metadata)           (ScatolaFile           *file,
            const char             *key,
            const char             *value);
    void                  (* set_metadata_as_list)   (ScatolaFile           *file,
            const char             *key,
            char                  **value);

    void                  (* mount)                  (ScatolaFile                   *file,
            GMountOperation                *mount_op,
            GCancellable                   *cancellable,
            ScatolaFileOperationCallback   callback,
            gpointer                        callback_data);
    void                 (* unmount)                 (ScatolaFile                   *file,
            GMountOperation                *mount_op,
            GCancellable                   *cancellable,
            ScatolaFileOperationCallback   callback,
            gpointer                        callback_data);
    void                 (* eject)                   (ScatolaFile                   *file,
            GMountOperation                *mount_op,
            GCancellable                   *cancellable,
            ScatolaFileOperationCallback   callback,
            gpointer                        callback_data);

    void                  (* start)                  (ScatolaFile                   *file,
            GMountOperation                *start_op,
            GCancellable                   *cancellable,
            ScatolaFileOperationCallback   callback,
            gpointer                        callback_data);
    void                 (* stop)                    (ScatolaFile                   *file,
            GMountOperation                *mount_op,
            GCancellable                   *cancellable,
            ScatolaFileOperationCallback   callback,
            gpointer                        callback_data);

    void                 (* poll_for_media)          (ScatolaFile                   *file);
} ScatolaFileClass;

#endif /* SCATOLA_FILE_H */
