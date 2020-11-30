/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/* scatola-file-utilities.h - interface for file manipulation routines.

   Copyright (C) 1999, 2000, 2001 Eazel, Inc.

   The Gde2 Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   The Gde2 Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with the Gde2 Library; see the file COPYING.LIB.  If not,
   write to the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
   Boston, MA 02110-1301, USA.

   Authors: John Sullivan <sullivan@eazel.com>
*/

#ifndef SCATOLA_FILE_UTILITIES_H
#define SCATOLA_FILE_UTILITIES_H

#include <gio/gio.h>
#include <gtk/gtk.h>

#define SCATOLA_SAVED_SEARCH_EXTENSION ".savedSearch"
#define SCATOLA_SAVED_SEARCH_MIMETYPE "application/x-gde2-saved-search"

/* These functions all return something something that needs to be
 * freed with g_free, is not NULL, and is guaranteed to exist.
 */
char *   scatola_get_xdg_dir                        (const char *type);
char *   scatola_get_user_directory                 (void);
char *   scatola_get_desktop_directory              (void);
GFile *  scatola_get_desktop_location               (void);
char *   scatola_get_desktop_directory_uri          (void);
char *   scatola_get_home_directory_uri             (void);
gboolean scatola_is_desktop_directory_file          (GFile *dir,
        const char *filename);
gboolean scatola_is_root_directory                  (GFile *dir);
gboolean scatola_is_desktop_directory               (GFile *dir);
gboolean scatola_is_home_directory                  (GFile *dir);
gboolean scatola_is_home_directory_file             (GFile *dir,
        const char *filename);
gboolean scatola_is_in_system_dir                   (GFile *location);
char *   scatola_get_pixmap_directory               (void);

gboolean scatola_should_use_templates_directory     (void);
char *   scatola_get_templates_directory            (void);
char *   scatola_get_templates_directory_uri        (void);
void     scatola_create_templates_directory         (void);

char *	 scatola_compute_title_for_location	     (GFile *file);

/* A version of gde2's gde2_pixmap_file that works for the scatola prefix.
 * Otherwise similar to gde2_pixmap_file in that it checks to see if the file
 * exists and returns NULL if it doesn't.
 */
/* FIXME bugzilla.gnome.org 42425:
 * We might not need this once we get on gde2-libs 2.0 which handles
 * gde2_pixmap_file better, using GDE2_PATH.
 */
char *   scatola_pixmap_file                        (const char *partial_path);

/* Locate a file in either the uers directory or the datadir. */
char *   scatola_get_data_file_path                 (const char *partial_path);

gboolean scatola_is_engrampa_installed              (void);

/* Inhibit/Uninhibit GDE2 Power Manager */
int    scatola_inhibit_power_manager                (const char *message) G_GNUC_WARN_UNUSED_RESULT;
void     scatola_uninhibit_power_manager            (int cookie);

/* Return an allocated file name that is guranteed to be unique, but
 * tries to make the name readable to users.
 * This isn't race-free, so don't use for security-related things
 */
char *   scatola_ensure_unique_file_name            (const char *directory_uri,
        const char *base_name,
        const char *extension);

GFile *  scatola_find_existing_uri_in_hierarchy     (GFile *location);

char * scatola_get_accel_map_file (void);

GHashTable * scatola_trashed_files_get_original_directories (GList *files,
        GList **unhandled_files);
void scatola_restore_files_from_trash (GList *files,
                                    GtkWindow *parent_window);

#endif /* SCATOLA_FILE_UTILITIES_H */
