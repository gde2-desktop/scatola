/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*-

   scatola-search-directory.h: Subclass of ScatolaDirectory to implement
   a virtual directory consisting of the search directory and the search
   icons

   Copyright (C) 2005 Novell, Inc

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
*/

#ifndef SCATOLA_SEARCH_DIRECTORY_H
#define SCATOLA_SEARCH_DIRECTORY_H

#include <libscatola-private/scatola-directory.h>
#include <libscatola-private/scatola-query.h>

#define SCATOLA_TYPE_SEARCH_DIRECTORY scatola_search_directory_get_type()
#define SCATOLA_SEARCH_DIRECTORY(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), SCATOLA_TYPE_SEARCH_DIRECTORY, ScatolaSearchDirectory))
#define SCATOLA_SEARCH_DIRECTORY_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), SCATOLA_TYPE_SEARCH_DIRECTORY, ScatolaSearchDirectoryClass))
#define SCATOLA_IS_SEARCH_DIRECTORY(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SCATOLA_TYPE_SEARCH_DIRECTORY))
#define SCATOLA_IS_SEARCH_DIRECTORY_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), SCATOLA_TYPE_SEARCH_DIRECTORY))
#define SCATOLA_SEARCH_DIRECTORY_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), SCATOLA_TYPE_SEARCH_DIRECTORY, ScatolaSearchDirectoryClass))

typedef struct ScatolaSearchDirectoryDetails ScatolaSearchDirectoryDetails;

typedef struct
{
    ScatolaDirectory parent_slot;
    ScatolaSearchDirectoryDetails *details;
} ScatolaSearchDirectory;

typedef struct
{
    ScatolaDirectoryClass parent_slot;
} ScatolaSearchDirectoryClass;

GType   scatola_search_directory_get_type             (void);

char   *scatola_search_directory_generate_new_uri     (void);

ScatolaSearchDirectory *scatola_search_directory_new_from_saved_search (const char *uri);

gboolean       scatola_search_directory_is_saved_search (ScatolaSearchDirectory *search);
gboolean       scatola_search_directory_is_modified     (ScatolaSearchDirectory *search);
gboolean       scatola_search_directory_is_indexed      (ScatolaSearchDirectory *search);
void           scatola_search_directory_save_search     (ScatolaSearchDirectory *search);
void           scatola_search_directory_save_to_file    (ScatolaSearchDirectory *search,
        const char              *save_file_uri);

ScatolaQuery *scatola_search_directory_get_query       (ScatolaSearchDirectory *search);
void           scatola_search_directory_set_query       (ScatolaSearchDirectory *search,
        ScatolaQuery           *query);

#endif /* SCATOLA_SEARCH_DIRECTORY_H */
