/* -*- Mode: C; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 8 -*- */

/*
 *  Scatola
 *
 *  Copyright (C) 1999, 2000 Red Hat, Inc.
 *  Copyright (C) 1999, 2000, 2001 Eazel, Inc.
 *  Copyright (C) 2003 Ximian, Inc.
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
 */
/* scatola-window.h: Interface of the main window object */

#ifndef SCATOLA_SPATIAL_WINDOW_H
#define SCATOLA_SPATIAL_WINDOW_H

#include "scatola-window.h"
#include "scatola-window-private.h"

#define SCATOLA_TYPE_SPATIAL_WINDOW scatola_spatial_window_get_type()
#define SCATOLA_SPATIAL_WINDOW(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), SCATOLA_TYPE_SPATIAL_WINDOW, ScatolaSpatialWindow))
#define SCATOLA_SPATIAL_WINDOW_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), SCATOLA_TYPE_SPATIAL_WINDOW, ScatolaSpatialWindowClass))
#define SCATOLA_IS_SPATIAL_WINDOW(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SCATOLA_TYPE_SPATIAL_WINDOW))
#define SCATOLA_IS_SPATIAL_WINDOW_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), SCATOLA_TYPE_SPATIAL_WINDOW))
#define SCATOLA_SPATIAL_WINDOW_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), SCATOLA_TYPE_SPATIAL_WINDOW, ScatolaSpatialWindowClass))

#ifndef SCATOLA_SPATIAL_WINDOW_DEFINED
#define SCATOLA_SPATIAL_WINDOW_DEFINED
typedef struct _ScatolaSpatialWindow        ScatolaSpatialWindow;
#endif
typedef struct _ScatolaSpatialWindowClass   ScatolaSpatialWindowClass;
typedef struct _ScatolaSpatialWindowDetails ScatolaSpatialWindowDetails;

struct _ScatolaSpatialWindow
{
    ScatolaWindow parent_object;

    ScatolaSpatialWindowDetails *details;
};

struct _ScatolaSpatialWindowClass
{
    ScatolaWindowClass parent_spot;
};


GType            scatola_spatial_window_get_type			(void);
void             scatola_spatial_window_set_location_button		(ScatolaSpatialWindow *window,
        GFile                 *location);

#endif
