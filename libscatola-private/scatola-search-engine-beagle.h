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

#ifndef SCATOLA_SEARCH_ENGINE_BEAGLE_H
#define SCATOLA_SEARCH_ENGINE_BEAGLE_H

#include <libscatola-private/scatola-search-engine.h>

#define SCATOLA_TYPE_SEARCH_ENGINE_BEAGLE		(scatola_search_engine_beagle_get_type ())
#define SCATOLA_SEARCH_ENGINE_BEAGLE(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), SCATOLA_TYPE_SEARCH_ENGINE_BEAGLE, ScatolaSearchEngineBeagle))
#define SCATOLA_SEARCH_ENGINE_BEAGLE_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST ((klass), SCATOLA_TYPE_SEARCH_ENGINE_BEAGLE, ScatolaSearchEngineBeagleClass))
#define SCATOLA_IS_SEARCH_ENGINE_BEAGLE(obj)		(G_TYPE_CHECK_INSTANCE_TYPE ((obj), SCATOLA_TYPE_SEARCH_ENGINE_BEAGLE))
#define SCATOLA_IS_SEARCH_ENGINE_BEAGLE_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE ((klass), SCATOLA_TYPE_SEARCH_ENGINE_BEAGLE))
#define SCATOLA_SEARCH_ENGINE_BEAGLE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), SCATOLA_TYPE_SEARCH_ENGINE_BEAGLE, ScatolaSearchEngineBeagleClass))

typedef struct ScatolaSearchEngineBeagleDetails ScatolaSearchEngineBeagleDetails;

typedef struct ScatolaSearchEngineBeagle
{
    ScatolaSearchEngine parent;
    ScatolaSearchEngineBeagleDetails *details;
} ScatolaSearchEngineBeagle;

typedef struct
{
    ScatolaSearchEngineClass parent_class;
} ScatolaSearchEngineBeagleClass;

GType          scatola_search_engine_beagle_get_type  (void);

ScatolaSearchEngine* scatola_search_engine_beagle_new       (void);

#endif /* SCATOLA_SEARCH_ENGINE_BEAGLE_H */
