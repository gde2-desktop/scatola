/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/*
 * Scatola
 *
 * Copyright (C) 2000 Eazel, Inc.
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
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * Authors: Darin Adler <darin@bentspoon.com>
 */

/* scatola-desktop-window.h
 */

#ifndef SCATOLA_DESKTOP_WINDOW_H
#define SCATOLA_DESKTOP_WINDOW_H

#include "scatola-window.h"
#include "scatola-application.h"
#include "scatola-spatial-window.h"

#define SCATOLA_TYPE_DESKTOP_WINDOW scatola_desktop_window_get_type()
#define SCATOLA_DESKTOP_WINDOW(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), SCATOLA_TYPE_DESKTOP_WINDOW, ScatolaDesktopWindow))
#define SCATOLA_DESKTOP_WINDOW_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), SCATOLA_TYPE_DESKTOP_WINDOW, ScatolaDesktopWindowClass))
#define SCATOLA_IS_DESKTOP_WINDOW(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SCATOLA_TYPE_DESKTOP_WINDOW))
#define SCATOLA_IS_DESKTOP_WINDOW_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), SCATOLA_TYPE_DESKTOP_WINDOW))
#define SCATOLA_DESKTOP_WINDOW_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), SCATOLA_TYPE_DESKTOP_WINDOW, ScatolaDesktopWindowClass))

typedef struct ScatolaDesktopWindowDetails ScatolaDesktopWindowDetails;

typedef struct
{
    ScatolaSpatialWindow parent_spot;
    ScatolaDesktopWindowDetails *details;
    gboolean affect_desktop_on_next_location_change;
} ScatolaDesktopWindow;

typedef struct
{
    ScatolaSpatialWindowClass parent_spot;
} ScatolaDesktopWindowClass;

GType                  scatola_desktop_window_get_type            (void);
ScatolaDesktopWindow *scatola_desktop_window_new                 (ScatolaApplication *application,
        GdkScreen           *screen);
void                   scatola_desktop_window_update_directory    (ScatolaDesktopWindow *window);
gboolean               scatola_desktop_window_loaded              (ScatolaDesktopWindow *window);

#endif /* SCATOLA_DESKTOP_WINDOW_H */
