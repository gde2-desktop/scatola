/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */
/*
 * Copyright (C) 2005 Red Hat, Inc
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
 * Author: Alexander Larsson <alexl@redhat.com>
 *
 */

#ifndef SCATOLA_SEARCH_ENGINE_SIMPLE_H
#define SCATOLA_SEARCH_ENGINE_SIMPLE_H

#include <libscatola-private/scatola-search-engine.h>

#define SCATOLA_TYPE_SEARCH_ENGINE_SIMPLE		(scatola_search_engine_simple_get_type ())
#define SCATOLA_SEARCH_ENGINE_SIMPLE(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), SCATOLA_TYPE_SEARCH_ENGINE_SIMPLE, ScatolaSearchEngineSimple))
#define SCATOLA_SEARCH_ENGINE_SIMPLE_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST ((klass), SCATOLA_TYPE_SEARCH_ENGINE_SIMPLE, ScatolaSearchEngineSimpleClass))
#define SCATOLA_IS_SEARCH_ENGINE_SIMPLE(obj)		(G_TYPE_CHECK_INSTANCE_TYPE ((obj), SCATOLA_TYPE_SEARCH_ENGINE_SIMPLE))
#define SCATOLA_IS_SEARCH_ENGINE_SIMPLE_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE ((klass), SCATOLA_TYPE_SEARCH_ENGINE_SIMPLE))
#define SCATOLA_SEARCH_ENGINE_SIMPLE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), SCATOLA_TYPE_SEARCH_ENGINE_SIMPLE, ScatolaSearchEngineSimpleClass))

typedef struct ScatolaSearchEngineSimpleDetails ScatolaSearchEngineSimpleDetails;

typedef struct ScatolaSearchEngineSimple
{
    ScatolaSearchEngine parent;
    ScatolaSearchEngineSimpleDetails *details;
} ScatolaSearchEngineSimple;

typedef struct
{
    ScatolaSearchEngineClass parent_class;
} ScatolaSearchEngineSimpleClass;

GType          scatola_search_engine_simple_get_type  (void);

ScatolaSearchEngine* scatola_search_engine_simple_new       (void);

#endif /* SCATOLA_SEARCH_ENGINE_SIMPLE_H */
