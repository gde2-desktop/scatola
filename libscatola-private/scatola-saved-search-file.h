/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*-

   scatola-saved-search-file.h: Subclass of ScatolaVFSFile to implement the
   the case of a Saved Search file.

   Copyright (C) 2005 Red Hat, Inc

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

   Author: Alexander Larsson
*/

#ifndef SCATOLA_SAVED_SEARCH_FILE_H
#define SCATOLA_SAVED_SEARCH_FILE_H

#include <libscatola-private/scatola-vfs-file.h>

#define SCATOLA_TYPE_SAVED_SEARCH_FILE scatola_saved_search_file_get_type()
#define SCATOLA_SAVED_SEARCH_FILE(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), SCATOLA_TYPE_SAVED_SEARCH_FILE, ScatolaSavedSearchFile))
#define SCATOLA_SAVED_SEARCH_FILE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), SCATOLA_TYPE_SAVED_SEARCH_FILE, ScatolaSavedSearchFileClass))
#define SCATOLA_IS_SAVED_SEARCH_FILE(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SCATOLA_TYPE_SAVED_SEARCH_FILE))
#define SCATOLA_IS_SAVED_SEARCH_FILE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), SCATOLA_TYPE_SAVED_SEARCH_FILE))
#define SCATOLA_SAVED_SEARCH_FILE_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), SCATOLA_TYPE_SAVED_SEARCH_FILE, ScatolaSavedSearchFileClass))


typedef struct ScatolaSavedSearchFileDetails ScatolaSavedSearchFileDetails;

typedef struct
{
    ScatolaFile parent_slot;
} ScatolaSavedSearchFile;

typedef struct
{
    ScatolaFileClass parent_slot;
} ScatolaSavedSearchFileClass;

GType   scatola_saved_search_file_get_type (void);

#endif /* SCATOLA_SAVED_SEARCH_FILE_H */
