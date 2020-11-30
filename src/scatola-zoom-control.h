/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/*
 * Scatola
 *
 * Copyright (C) 2000 Eazel, Inc.
 *
 * Scatola is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Scatola is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * Author: Andy Hertzfeld <andy@eazel.com>
 *
 * This is the header file for the zoom control on the location bar
 *
 */

#ifndef SCATOLA_ZOOM_CONTROL_H
#define SCATOLA_ZOOM_CONTROL_H

#include <gtk/gtk.h>
#include <libscatola-private/scatola-icon-info.h> /* For ScatolaZoomLevel */

#define SCATOLA_TYPE_ZOOM_CONTROL scatola_zoom_control_get_type()
#define SCATOLA_ZOOM_CONTROL(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), SCATOLA_TYPE_ZOOM_CONTROL, ScatolaZoomControl))
#define SCATOLA_ZOOM_CONTROL_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), SCATOLA_TYPE_ZOOM_CONTROL, ScatolaZoomControlClass))
#define SCATOLA_IS_ZOOM_CONTROL(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SCATOLA_TYPE_ZOOM_CONTROL))
#define SCATOLA_IS_ZOOM_CONTROL_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), SCATOLA_TYPE_ZOOM_CONTROL))
#define SCATOLA_ZOOM_CONTROL_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), SCATOLA_TYPE_ZOOM_CONTROL, ScatolaZoomControlClass))

typedef struct ScatolaZoomControl ScatolaZoomControl;
typedef struct ScatolaZoomControlClass ScatolaZoomControlClass;
typedef struct ScatolaZoomControlDetails ScatolaZoomControlDetails;

struct ScatolaZoomControl
{
#if GTK_CHECK_VERSION (3, 0, 0)
    GtkBox parent;
#else
    GtkHBox parent;
#endif
    ScatolaZoomControlDetails *details;
};

struct ScatolaZoomControlClass
{
#if GTK_CHECK_VERSION (3, 0, 0)
    GtkBoxClass parent_class;
#else
    GtkHBoxClass parent_class;
#endif

    void (*zoom_in)		(ScatolaZoomControl *control);
    void (*zoom_out) 	(ScatolaZoomControl *control);
    void (*zoom_to_level) 	(ScatolaZoomControl *control,
                             ScatolaZoomLevel zoom_level);
    void (*zoom_to_default)	(ScatolaZoomControl *control);

    /* Action signal for keybindings, do not connect to this */
    void (*change_value)    (ScatolaZoomControl *control,
                             GtkScrollType scroll);
};

GType             scatola_zoom_control_get_type           (void);
GtkWidget *       scatola_zoom_control_new                (void);
void              scatola_zoom_control_set_zoom_level     (ScatolaZoomControl *zoom_control,
        ScatolaZoomLevel    zoom_level);
void              scatola_zoom_control_set_parameters     (ScatolaZoomControl *zoom_control,
        ScatolaZoomLevel    min_zoom_level,
        ScatolaZoomLevel    max_zoom_level,
        gboolean             has_min_zoom_level,
        gboolean             has_max_zoom_level,
        GList               *zoom_levels);
ScatolaZoomLevel scatola_zoom_control_get_zoom_level     (ScatolaZoomControl *zoom_control);
ScatolaZoomLevel scatola_zoom_control_get_min_zoom_level (ScatolaZoomControl *zoom_control);
ScatolaZoomLevel scatola_zoom_control_get_max_zoom_level (ScatolaZoomControl *zoom_control);
gboolean          scatola_zoom_control_has_min_zoom_level (ScatolaZoomControl *zoom_control);
gboolean          scatola_zoom_control_has_max_zoom_level (ScatolaZoomControl *zoom_control);
gboolean          scatola_zoom_control_can_zoom_in        (ScatolaZoomControl *zoom_control);
gboolean          scatola_zoom_control_can_zoom_out       (ScatolaZoomControl *zoom_control);

void              scatola_zoom_control_set_active_appearance (ScatolaZoomControl *zoom_control, gboolean is_active);

#endif /* SCATOLA_ZOOM_CONTROL_H */
