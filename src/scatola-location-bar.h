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
 * You should have received a copy of the GNU General Public
 * License along with this program; see the file COPYING.  If not,
 * write to the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 * Author: Maciej Stachowiak <mjs@eazel.com>
 *         Ettore Perazzoli <ettore@gnu.org>
 */

/* scatola-location-bar.h - Location bar for Scatola
 */

#ifndef SCATOLA_LOCATION_BAR_H
#define SCATOLA_LOCATION_BAR_H

#include "scatola-navigation-window.h"
#include "scatola-navigation-window-pane.h"
#include <libscatola-private/scatola-entry.h>
#include <gtk/gtk.h>

#define SCATOLA_TYPE_LOCATION_BAR scatola_location_bar_get_type()
#define SCATOLA_LOCATION_BAR(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), SCATOLA_TYPE_LOCATION_BAR, ScatolaLocationBar))
#define SCATOLA_LOCATION_BAR_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), SCATOLA_TYPE_LOCATION_BAR, ScatolaLocationBarClass))
#define SCATOLA_IS_LOCATION_BAR(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SCATOLA_TYPE_LOCATION_BAR))
#define SCATOLA_IS_LOCATION_BAR_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), SCATOLA_TYPE_LOCATION_BAR))
#define SCATOLA_LOCATION_BAR_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), SCATOLA_TYPE_LOCATION_BAR, ScatolaLocationBarClass))

typedef struct ScatolaLocationBarDetails ScatolaLocationBarDetails;

typedef struct ScatolaLocationBar
{
    GtkHBox parent;
    ScatolaLocationBarDetails *details;
} ScatolaLocationBar;

typedef struct
{
    GtkHBoxClass parent_class;

    /* for GtkBindingSet */
    void         (* cancel)           (ScatolaLocationBar *bar);
} ScatolaLocationBarClass;

GType      scatola_location_bar_get_type     	(void);
GtkWidget* scatola_location_bar_new          	(ScatolaNavigationWindowPane *pane);
void       scatola_location_bar_set_active     (ScatolaLocationBar *location_bar,
        gboolean is_active);
ScatolaEntry * scatola_location_bar_get_entry (ScatolaLocationBar *location_bar);

void    scatola_location_bar_activate         (ScatolaLocationBar *bar);
void    scatola_location_bar_set_location     (ScatolaLocationBar *bar,
                                            const char      *location);

#endif /* SCATOLA_LOCATION_BAR_H */
