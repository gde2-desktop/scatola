/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/*
 * Scatola
 *
 * Copyright (C) 2003 Ximian, Inc.
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
 */

#ifndef SCATOLA_LOCATION_DIALOG_H
#define SCATOLA_LOCATION_DIALOG_H

#include <gtk/gtk.h>
#include "scatola-window.h"

#define SCATOLA_TYPE_LOCATION_DIALOG         (scatola_location_dialog_get_type ())
#define SCATOLA_LOCATION_DIALOG(obj)         (G_TYPE_CHECK_INSTANCE_CAST ((obj), SCATOLA_TYPE_LOCATION_DIALOG, ScatolaLocationDialog))
#define SCATOLA_LOCATION_DIALOG_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), SCATOLA_TYPE_LOCATION_DIALOG, ScatolaLocationDialogClass))
#define SCATOLA_IS_LOCATION_DIALOG(obj)      (G_TYPE_INSTANCE_CHECK_TYPE ((obj), SCATOLA_TYPE_LOCATION_DIALOG)

typedef struct _ScatolaLocationDialog        ScatolaLocationDialog;
typedef struct _ScatolaLocationDialogClass   ScatolaLocationDialogClass;
typedef struct _ScatolaLocationDialogDetails ScatolaLocationDialogDetails;

struct _ScatolaLocationDialog
{
    GtkDialog parent;
    ScatolaLocationDialogDetails *details;
};

struct _ScatolaLocationDialogClass
{
    GtkDialogClass parent_class;
};

GType      scatola_location_dialog_get_type     (void);
GtkWidget* scatola_location_dialog_new          (ScatolaWindow         *window);
void       scatola_location_dialog_set_location (ScatolaLocationDialog *dialog,
        const char             *location);

#endif /* SCATOLA_LOCATION_DIALOG_H */
