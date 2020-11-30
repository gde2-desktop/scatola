/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */
/*
 * Copyright (C) 2005 Novell, Inc.
 *
 * Scatola is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * Scatola is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; see the file COPYING.  If not,
 * write to the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 * Author: Anders Carlsson <andersca@imendio.com>
 *
 */

#ifndef SCATOLA_QUERY_H
#define SCATOLA_QUERY_H

#include <glib-object.h>

#define SCATOLA_TYPE_QUERY		(scatola_query_get_type ())
#define SCATOLA_QUERY(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), SCATOLA_TYPE_QUERY, ScatolaQuery))
#define SCATOLA_QUERY_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST ((klass), SCATOLA_TYPE_QUERY, ScatolaQueryClass))
#define SCATOLA_IS_QUERY(obj)		(G_TYPE_CHECK_INSTANCE_TYPE ((obj), SCATOLA_TYPE_QUERY))
#define SCATOLA_IS_QUERY_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE ((klass), SCATOLA_TYPE_QUERY))
#define SCATOLA_QUERY_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), SCATOLA_TYPE_QUERY, ScatolaQueryClass))

typedef struct ScatolaQueryDetails ScatolaQueryDetails;

typedef struct ScatolaQuery
{
    GObject parent;
    ScatolaQueryDetails *details;
} ScatolaQuery;

typedef struct
{
    GObjectClass parent_class;
} ScatolaQueryClass;

GType          scatola_query_get_type (void);
gboolean       scatola_query_enabled  (void);

ScatolaQuery* scatola_query_new      (void);

char *         scatola_query_get_text           (ScatolaQuery *query);
void           scatola_query_set_text           (ScatolaQuery *query, const char *text);

char *         scatola_query_get_location       (ScatolaQuery *query);
void           scatola_query_set_location       (ScatolaQuery *query, const char *uri);

GList *        scatola_query_get_mime_types     (ScatolaQuery *query);
void           scatola_query_set_mime_types     (ScatolaQuery *query, GList *mime_types);
void           scatola_query_add_mime_type      (ScatolaQuery *query, const char *mime_type);

char *         scatola_query_to_readable_string (ScatolaQuery *query);
ScatolaQuery *scatola_query_load               (char *file);
gboolean       scatola_query_save               (ScatolaQuery *query, char *file);

#endif /* SCATOLA_QUERY_H */
