/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/* scatola-column-choose.h - A column chooser widget

   Copyright (C) 2004 Novell, Inc.

   The Gde2 Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   The Gde2 Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with the Gde2 Library; see the column COPYING.LIB.  If not,
   write to the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
   Boston, MA 02110-1301, USA.

   Authors: Dave Camp <dave@ximian.com>
*/

#ifndef SCATOLA_COLUMN_CHOOSER_H
#define SCATOLA_COLUMN_CHOOSER_H

#include <gtk/gtk.h>
#include <libscatola-private/scatola-file.h>

#define SCATOLA_TYPE_COLUMN_CHOOSER scatola_column_chooser_get_type()
#define SCATOLA_COLUMN_CHOOSER(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), SCATOLA_TYPE_COLUMN_CHOOSER, ScatolaColumnChooser))
#define SCATOLA_COLUMN_CHOOSER_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), SCATOLA_TYPE_COLUMN_CHOOSER, ScatolaColumnChooserClass))
#define SCATOLA_IS_COLUMN_CHOOSER(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SCATOLA_TYPE_COLUMN_CHOOSER))
#define SCATOLA_IS_COLUMN_CHOOSER_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), SCATOLA_TYPE_COLUMN_CHOOSER))
#define SCATOLA_COLUMN_CHOOSER_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), SCATOLA_TYPE_COLUMN_CHOOSER, ScatolaColumnChooserClass))

typedef struct _ScatolaColumnChooserDetails ScatolaColumnChooserDetails;

typedef struct
{
#if GTK_CHECK_VERSION (3, 0, 0)
    GtkBox parent;
#else
    GtkHBox parent;
#endif

    ScatolaColumnChooserDetails *details;
} ScatolaColumnChooser;

typedef struct
{
#if GTK_CHECK_VERSION (3, 0, 0)
    GtkBoxClass parent_slot;
#else
    GtkHBoxClass parent_slot;
#endif

    void (*changed) (ScatolaColumnChooser *chooser);
    void (*use_default) (ScatolaColumnChooser *chooser);
} ScatolaColumnChooserClass;

GType      scatola_column_chooser_get_type            (void);
GtkWidget *scatola_column_chooser_new                 (ScatolaFile *file);
void       scatola_column_chooser_set_settings    (ScatolaColumnChooser   *chooser,
        char                   **visible_columns,
        char                   **column_order);
void       scatola_column_chooser_get_settings    (ScatolaColumnChooser *chooser,
        char                  ***visible_columns,
        char                  ***column_order);

#endif /* SCATOLA_COLUMN_CHOOSER_H */
