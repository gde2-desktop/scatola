/*
 *  scatola-file-info.h - Information about a file
 *
 *  Copyright (C) 2003 Novell, Inc.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public
 *  License along with this library; if not, write to the Free
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

/* ScatolaFileInfo is an interface to the ScatolaFile object.  It
 * provides access to the asynchronous data in the ScatolaFile.
 * Extensions are passed objects of this type for operations. */

#ifndef SCATOLA_FILE_INFO_H
#define SCATOLA_FILE_INFO_H

#include <glib-object.h>
#include <gio/gio.h>

G_BEGIN_DECLS

#define SCATOLA_TYPE_FILE_INFO           (scatola_file_info_get_type ())
#define SCATOLA_FILE_INFO(obj)           (G_TYPE_CHECK_INSTANCE_CAST ((obj), SCATOLA_TYPE_FILE_INFO, ScatolaFileInfo))
#define SCATOLA_IS_FILE_INFO(obj)        (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SCATOLA_TYPE_FILE_INFO))
#define SCATOLA_FILE_INFO_GET_IFACE(obj) (G_TYPE_INSTANCE_GET_INTERFACE ((obj), SCATOLA_TYPE_FILE_INFO, ScatolaFileInfoIface))

#ifndef SCATOLA_FILE_DEFINED
#define SCATOLA_FILE_DEFINED
/* Using ScatolaFile for the vtable to make implementing this in
 * ScatolaFile easier */
typedef struct ScatolaFile            ScatolaFile;
#endif

typedef ScatolaFile                   ScatolaFileInfo;
typedef struct _ScatolaFileInfoIface  ScatolaFileInfoIface;

/**
 * ScatolaFileInfoIface:
 * @g_iface: The parent interface.
 * @is_gone: Returns whether the file info is gone.
 *   See scatola_file_info_is_gone() for details.
 * @get_name: Returns the file name as a string.
 *   See scatola_file_info_get_name() for details.
 * @get_uri: Returns the file URI as a string.
 *   See scatola_file_info_get_uri() for details.
 * @get_parent_uri: Returns the file parent URI as a string.
 *   See scatola_file_info_get_parent_uri() for details.
 * @get_uri_scheme: Returns the file URI scheme as a string.
 *   See scatola_file_info_get_uri_scheme() for details.
 * @get_mime_type: Returns the file mime type as a string.
 *   See scatola_file_info_get_mime_type() for details.
 * @is_mime_type: Returns whether the file is the given mime type.
 *   See scatola_file_info_is_mime_type() for details.
 * @is_directory: Returns whether the file is a directory.
 *   See scatola_file_info_is_directory() for details.
 * @add_emblem: Adds an emblem to this file.
 *   See scatola_file_info_add_emblem() for details.
 * @get_string_attribute: Returns the specified file attribute as a string.
 *   See scatola_file_info_get_string_attribute() for details.
 * @add_string_attribute: Sets the specified string file attribute value.
 *   See scatola_file_info_add_string_attribute() for details.
 * @invalidate_extension_info: Invalidates information of the file provided by extensions.
 *   See scatola_file_info_invalidate_extension_info() for details.
 * @get_activation_uri: Returns the file activation URI as a string.
 *   See scatola_file_info_get_activation_uri() for details.
 * @get_file_type: Returns the file type.
 *   See scatola_file_info_get_file_type() for details.
 * @get_location: Returns the file location as a #GFile.
 *   See scatola_file_info_get_location() for details.
 * @get_parent_location: Returns the file parent location as a #GFile.
 *   See scatola_file_info_get_parent_location() for details.
 * @get_parent_info: Returns the file parent #ScatolaFileInfo.
 *   See scatola_file_info_get_parent_info() for details.
 * @get_mount: Returns the file mount as a #GMount.
 *   See scatola_file_info_get_mount() for details.
 * @can_write: Returns whether the file is writable.
 *   See scatola_file_info_can_write() for details.
 *
 * Interface for extensions to get and modify information
 * about file objects.
 */

struct _ScatolaFileInfoIface {
    GTypeInterface g_iface;

    gboolean      (*is_gone)              (ScatolaFileInfo *file);

    char         *(*get_name)             (ScatolaFileInfo *file);
    char         *(*get_uri)              (ScatolaFileInfo *file);
    char         *(*get_parent_uri)       (ScatolaFileInfo *file);
    char         *(*get_uri_scheme)       (ScatolaFileInfo *file);

    char         *(*get_mime_type)        (ScatolaFileInfo *file);
    gboolean      (*is_mime_type)         (ScatolaFileInfo *file,
                                           const char   *mime_Type);
    gboolean      (*is_directory)         (ScatolaFileInfo *file);

    void          (*add_emblem)           (ScatolaFileInfo *file,
                                           const char   *emblem_name);
    char         *(*get_string_attribute) (ScatolaFileInfo *file,
                                           const char   *attribute_name);
    void          (*add_string_attribute) (ScatolaFileInfo *file,
                                           const char   *attribute_name,
                                           const char   *value);
    void          (*invalidate_extension_info) (ScatolaFileInfo *file);

    char         *(*get_activation_uri)   (ScatolaFileInfo *file);

    GFileType     (*get_file_type)        (ScatolaFileInfo *file);
    GFile        *(*get_location)         (ScatolaFileInfo *file);
    GFile        *(*get_parent_location)  (ScatolaFileInfo *file);
    ScatolaFileInfo *(*get_parent_info)      (ScatolaFileInfo *file);
    GMount       *(*get_mount)            (ScatolaFileInfo *file);
    gboolean      (*can_write)            (ScatolaFileInfo *file);
};

GList       *scatola_file_info_list_copy             (GList        *files);
void         scatola_file_info_list_free             (GList        *files);
GType        scatola_file_info_get_type              (void);

/* Return true if the file has been deleted */
gboolean     scatola_file_info_is_gone               (ScatolaFileInfo *file);

/* Name and Location */
GFileType    scatola_file_info_get_file_type         (ScatolaFileInfo *file);
GFile        *scatola_file_info_get_location         (ScatolaFileInfo *file);
char         *scatola_file_info_get_name             (ScatolaFileInfo *file);
char         *scatola_file_info_get_uri              (ScatolaFileInfo *file);
char         *scatola_file_info_get_activation_uri   (ScatolaFileInfo *file);
GFile        *scatola_file_info_get_parent_location  (ScatolaFileInfo *file);
char         *scatola_file_info_get_parent_uri       (ScatolaFileInfo *file);
GMount       *scatola_file_info_get_mount            (ScatolaFileInfo *file);
char         *scatola_file_info_get_uri_scheme       (ScatolaFileInfo *file);
/* It's not safe to call this recursively multiple times, as it works
 * only for files already cached by Scatola.
 */
ScatolaFileInfo *scatola_file_info_get_parent_info      (ScatolaFileInfo *file);

/* File Type */
char         *scatola_file_info_get_mime_type        (ScatolaFileInfo *file);
gboolean      scatola_file_info_is_mime_type         (ScatolaFileInfo *file,
                                                   const char   *mime_type);
gboolean      scatola_file_info_is_directory         (ScatolaFileInfo *file);
gboolean      scatola_file_info_can_write            (ScatolaFileInfo *file);


/* Modifying the ScatolaFileInfo */
void          scatola_file_info_add_emblem           (ScatolaFileInfo *file,
                                                   const char   *emblem_name);
char         *scatola_file_info_get_string_attribute (ScatolaFileInfo *file,
                                                   const char   *attribute_name);
void          scatola_file_info_add_string_attribute (ScatolaFileInfo *file,
                                                   const char   *attribute_name,
                                                   const char   *value);

/* Invalidating file info */
void          scatola_file_info_invalidate_extension_info (ScatolaFileInfo *file);

ScatolaFileInfo *scatola_file_info_lookup                (GFile       *location);
ScatolaFileInfo *scatola_file_info_create                (GFile       *location);
ScatolaFileInfo *scatola_file_info_lookup_for_uri        (const char  *uri);
ScatolaFileInfo *scatola_file_info_create_for_uri        (const char  *uri);

G_END_DECLS

#endif
