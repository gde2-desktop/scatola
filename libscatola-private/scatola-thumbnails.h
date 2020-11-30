/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*-

   scatola-thumbnails.h: Thumbnail code for icon factory.

   Copyright (C) 2000 Eazel, Inc.

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

   Author: Andy Hertzfeld <andy@eazel.com>
*/

#ifndef SCATOLA_THUMBNAILS_H
#define SCATOLA_THUMBNAILS_H

#include <gdk-pixbuf/gdk-pixbuf.h>
#include <libscatola-private/scatola-file.h>

/* Returns NULL if there's no thumbnail yet. */
void       scatola_create_thumbnail                (ScatolaFile *file);
gboolean   scatola_can_thumbnail                   (ScatolaFile *file);
gboolean   scatola_can_thumbnail_internally        (ScatolaFile *file);
gboolean   scatola_thumbnail_is_mimetype_limited_by_size
(const char *mime_type);

/* Queue handling: */
void       scatola_thumbnail_remove_from_queue     (const char   *file_uri);
void       scatola_thumbnail_prioritize            (const char   *file_uri);


#endif /* SCATOLA_THUMBNAILS_H */
