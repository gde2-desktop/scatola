/* -*- Mode: C; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 8 -*- */

/*
 *  Scatola
 *
 *  Copyright (C) 2009 Red Hat, Inc.
 *
 *  Scatola is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License as
 *  published by the Free Software Foundation; either version 2 of the
 *  License, or (at your option) any later version.
 *
 *  Scatola is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 *  Authors: Alexander Larsson <alexl@redhat.com>
 *
 */

#ifndef SCATOLA_ZOOM_ACTION_H
#define SCATOLA_ZOOM_ACTION_H

#include <gtk/gtk.h>

#define SCATOLA_TYPE_ZOOM_ACTION            (scatola_zoom_action_get_type ())
#define SCATOLA_ZOOM_ACTION(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), SCATOLA_TYPE_ZOOM_ACTION, ScatolaZoomAction))
#define SCATOLA_ZOOM_ACTION_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), SCATOLA_TYPE_ZOOM_ACTION, ScatolaZoomActionClass))
#define SCATOLA_IS_ZOOM_ACTION(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SCATOLA_TYPE_ZOOM_ACTION))
#define SCATOLA_IS_ZOOM_ACTION_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((obj), SCATOLA_TYPE_ZOOM_ACTION))
#define SCATOLA_ZOOM_ACTION_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj), SCATOLA_TYPE_ZOOM_ACTION, ScatolaZoomActionClass))

typedef struct _ScatolaZoomAction       ScatolaZoomAction;
typedef struct _ScatolaZoomActionClass  ScatolaZoomActionClass;
typedef struct ScatolaZoomActionPrivate ScatolaZoomActionPrivate;

struct _ScatolaZoomAction
{
    GtkAction parent;

    /*< private >*/
    ScatolaZoomActionPrivate *priv;
};

struct _ScatolaZoomActionClass
{
    GtkActionClass parent_class;
};

GType    scatola_zoom_action_get_type   (void);

#endif
