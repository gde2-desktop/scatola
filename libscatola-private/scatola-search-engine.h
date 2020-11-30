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

#ifndef SCATOLA_SEARCH_ENGINE_H
#define SCATOLA_SEARCH_ENGINE_H

#include <glib-object.h>
#include <libscatola-private/scatola-query.h>

#define SCATOLA_TYPE_SEARCH_ENGINE		(scatola_search_engine_get_type ())
#define SCATOLA_SEARCH_ENGINE(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), SCATOLA_TYPE_SEARCH_ENGINE, ScatolaSearchEngine))
#define SCATOLA_SEARCH_ENGINE_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST ((klass), SCATOLA_TYPE_SEARCH_ENGINE, ScatolaSearchEngineClass))
#define SCATOLA_IS_SEARCH_ENGINE(obj)		(G_TYPE_CHECK_INSTANCE_TYPE ((obj), SCATOLA_TYPE_SEARCH_ENGINE))
#define SCATOLA_IS_SEARCH_ENGINE_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE ((klass), SCATOLA_TYPE_SEARCH_ENGINE))
#define SCATOLA_SEARCH_ENGINE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), SCATOLA_TYPE_SEARCH_ENGINE, ScatolaSearchEngineClass))

typedef struct ScatolaSearchEngineDetails ScatolaSearchEngineDetails;

typedef struct ScatolaSearchEngine
{
    GObject parent;
    ScatolaSearchEngineDetails *details;
} ScatolaSearchEngine;

typedef struct
{
    GObjectClass parent_class;

    /* VTable */
    void (*set_query) (ScatolaSearchEngine *engine, ScatolaQuery *query);
    void (*start) (ScatolaSearchEngine *engine);
    void (*stop) (ScatolaSearchEngine *engine);
    gboolean (*is_indexed) (ScatolaSearchEngine *engine);

    /* Signals */
    void (*hits_added) (ScatolaSearchEngine *engine, GList *hits);
    void (*hits_subtracted) (ScatolaSearchEngine *engine, GList *hits);
    void (*finished) (ScatolaSearchEngine *engine);
    void (*error) (ScatolaSearchEngine *engine, const char *error_message);
} ScatolaSearchEngineClass;

GType          scatola_search_engine_get_type  (void);
gboolean       scatola_search_engine_enabled (void);

ScatolaSearchEngine* scatola_search_engine_new       (void);

void           scatola_search_engine_set_query (ScatolaSearchEngine *engine, ScatolaQuery *query);
void	       scatola_search_engine_start (ScatolaSearchEngine *engine);
void	       scatola_search_engine_stop (ScatolaSearchEngine *engine);
gboolean       scatola_search_engine_is_indexed (ScatolaSearchEngine *engine);

void	       scatola_search_engine_hits_added (ScatolaSearchEngine *engine, GList *hits);
void	       scatola_search_engine_hits_subtracted (ScatolaSearchEngine *engine, GList *hits);
void	       scatola_search_engine_finished (ScatolaSearchEngine *engine);
void	       scatola_search_engine_error (ScatolaSearchEngine *engine, const char *error_message);

#endif /* SCATOLA_SEARCH_ENGINE_H */
