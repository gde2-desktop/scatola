/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*-

   scatola-search-directory-file.h: Subclass of ScatolaFile to implement the
   the case of the search directory

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

#ifndef SCATOLA_SEARCH_DIRECTORY_FILE_H
#define SCATOLA_SEARCH_DIRECTORY_FILE_H

#include <libscatola-private/scatola-file.h>

#define SCATOLA_TYPE_SEARCH_DIRECTORY_FILE scatola_search_directory_file_get_type()
#define SCATOLA_SEARCH_DIRECTORY_FILE(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), SCATOLA_TYPE_SEARCH_DIRECTORY_FILE, ScatolaSearchDirectoryFile))
#define SCATOLA_SEARCH_DIRECTORY_FILE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), SCATOLA_TYPE_SEARCH_DIRECTORY_FILE, ScatolaSearchDirectoryFileClass))
#define SCATOLA_IS_SEARCH_DIRECTORY_FILE(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SCATOLA_TYPE_SEARCH_DIRECTORY_FILE))
#define SCATOLA_IS_SEARCH_DIRECTORY_FILE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), SCATOLA_TYPE_SEARCH_DIRECTORY_FILE))
#define SCATOLA_SEARCH_DIRECTORY_FILE_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), SCATOLA_TYPE_SEARCH_DIRECTORY_FILE, ScatolaSearchDirectoryFileClass))

typedef struct ScatolaSearchDirectoryFileDetails ScatolaSearchDirectoryFileDetails;

typedef struct
{
    ScatolaFile parent_slot;
    ScatolaSearchDirectoryFileDetails *details;
} ScatolaSearchDirectoryFile;

typedef struct
{
    ScatolaFileClass parent_slot;
} ScatolaSearchDirectoryFileClass;

GType   scatola_search_directory_file_get_type (void);
void    scatola_search_directory_file_update_display_name (ScatolaSearchDirectoryFile *search_file);

#endif /* SCATOLA_SEARCH_DIRECTORY_FILE_H */
