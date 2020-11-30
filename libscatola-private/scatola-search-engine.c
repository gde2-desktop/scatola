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

#include <config.h>
#include "scatola-search-engine.h"
#include "scatola-search-engine-beagle.h"
#include "scatola-search-engine-simple.h"
#include "scatola-search-engine-tracker.h"

#include <eel/eel-gtk-macros.h>

struct ScatolaSearchEngineDetails
{
    int none;
};

enum
{
    HITS_ADDED,
    HITS_SUBTRACTED,
    FINISHED,
    ERROR,
    LAST_SIGNAL
};

static guint signals[LAST_SIGNAL] = { 0 };

static void  scatola_search_engine_class_init       (ScatolaSearchEngineClass *class);
static void  scatola_search_engine_init             (ScatolaSearchEngine      *engine);

G_DEFINE_ABSTRACT_TYPE (ScatolaSearchEngine,
                        scatola_search_engine,
                        G_TYPE_OBJECT);

static GObjectClass *parent_class = NULL;

static void
finalize (GObject *object)
{
    ScatolaSearchEngine *engine;

    engine = SCATOLA_SEARCH_ENGINE (object);

    g_free (engine->details);

    EEL_CALL_PARENT (G_OBJECT_CLASS, finalize, (object));
}

static void
scatola_search_engine_class_init (ScatolaSearchEngineClass *class)
{
    GObjectClass *gobject_class;

    parent_class = g_type_class_peek_parent (class);

    gobject_class = G_OBJECT_CLASS (class);
    gobject_class->finalize = finalize;

    signals[HITS_ADDED] =
        g_signal_new ("hits-added",
                      G_TYPE_FROM_CLASS (class),
                      G_SIGNAL_RUN_LAST,
                      G_STRUCT_OFFSET (ScatolaSearchEngineClass, hits_added),
                      NULL, NULL,
                      g_cclosure_marshal_VOID__POINTER,
                      G_TYPE_NONE, 1,
                      G_TYPE_POINTER);

    signals[HITS_SUBTRACTED] =
        g_signal_new ("hits-subtracted",
                      G_TYPE_FROM_CLASS (class),
                      G_SIGNAL_RUN_LAST,
                      G_STRUCT_OFFSET (ScatolaSearchEngineClass, hits_subtracted),
                      NULL, NULL,
                      g_cclosure_marshal_VOID__POINTER,
                      G_TYPE_NONE, 1,
                      G_TYPE_POINTER);

    signals[FINISHED] =
        g_signal_new ("finished",
                      G_TYPE_FROM_CLASS (class),
                      G_SIGNAL_RUN_LAST,
                      G_STRUCT_OFFSET (ScatolaSearchEngineClass, finished),
                      NULL, NULL,
                      g_cclosure_marshal_VOID__VOID,
                      G_TYPE_NONE, 0);

    signals[ERROR] =
        g_signal_new ("error",
                      G_TYPE_FROM_CLASS (class),
                      G_SIGNAL_RUN_LAST,
                      G_STRUCT_OFFSET (ScatolaSearchEngineClass, error),
                      NULL, NULL,
                      g_cclosure_marshal_VOID__STRING,
                      G_TYPE_NONE, 1,
                      G_TYPE_STRING);

}

static void
scatola_search_engine_init (ScatolaSearchEngine *engine)
{
    engine->details = g_new0 (ScatolaSearchEngineDetails, 1);
}

ScatolaSearchEngine *
scatola_search_engine_new (void)
{
    ScatolaSearchEngine *engine;

    engine = scatola_search_engine_tracker_new ();
    if (engine)
    {
        return engine;
    }

    engine = scatola_search_engine_beagle_new ();
    if (engine)
    {
        return engine;
    }

    engine = scatola_search_engine_simple_new ();
    return engine;
}

void
scatola_search_engine_set_query (ScatolaSearchEngine *engine, ScatolaQuery *query)
{
    g_return_if_fail (SCATOLA_IS_SEARCH_ENGINE (engine));
    g_return_if_fail (SCATOLA_SEARCH_ENGINE_GET_CLASS (engine)->set_query != NULL);

    SCATOLA_SEARCH_ENGINE_GET_CLASS (engine)->set_query (engine, query);
}

void
scatola_search_engine_start (ScatolaSearchEngine *engine)
{
    g_return_if_fail (SCATOLA_IS_SEARCH_ENGINE (engine));
    g_return_if_fail (SCATOLA_SEARCH_ENGINE_GET_CLASS (engine)->start != NULL);

    SCATOLA_SEARCH_ENGINE_GET_CLASS (engine)->start (engine);
}


void
scatola_search_engine_stop (ScatolaSearchEngine *engine)
{
    g_return_if_fail (SCATOLA_IS_SEARCH_ENGINE (engine));
    g_return_if_fail (SCATOLA_SEARCH_ENGINE_GET_CLASS (engine)->stop != NULL);

    SCATOLA_SEARCH_ENGINE_GET_CLASS (engine)->stop (engine);
}

gboolean
scatola_search_engine_is_indexed (ScatolaSearchEngine *engine)
{
    g_return_val_if_fail (SCATOLA_IS_SEARCH_ENGINE (engine), FALSE);
    g_return_val_if_fail (SCATOLA_SEARCH_ENGINE_GET_CLASS (engine)->is_indexed != NULL, FALSE);

    return SCATOLA_SEARCH_ENGINE_GET_CLASS (engine)->is_indexed (engine);
}

void
scatola_search_engine_hits_added (ScatolaSearchEngine *engine, GList *hits)
{
    g_return_if_fail (SCATOLA_IS_SEARCH_ENGINE (engine));

    g_signal_emit (engine, signals[HITS_ADDED], 0, hits);
}


void
scatola_search_engine_hits_subtracted (ScatolaSearchEngine *engine, GList *hits)
{
    g_return_if_fail (SCATOLA_IS_SEARCH_ENGINE (engine));

    g_signal_emit (engine, signals[HITS_SUBTRACTED], 0, hits);
}


void
scatola_search_engine_finished (ScatolaSearchEngine *engine)
{
    g_return_if_fail (SCATOLA_IS_SEARCH_ENGINE (engine));

    g_signal_emit (engine, signals[FINISHED], 0);
}

void
scatola_search_engine_error (ScatolaSearchEngine *engine, const char *error_message)
{
    g_return_if_fail (SCATOLA_IS_SEARCH_ENGINE (engine));

    g_signal_emit (engine, signals[ERROR], 0, error_message);
}
