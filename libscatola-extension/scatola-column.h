/*
 *  scatola-column.h - Info columns exported by
 *                      ScatolaColumnProvider objects.
 *
 *  Copyright (C) 2003 Novell, Inc.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public
 *  License along with this library; if not, write to the Free
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 *  Author:  Dave Camp <dave@ximian.com>
 *
 */

#ifndef SCATOLA_COLUMN_H
#define SCATOLA_COLUMN_H

#include <glib-object.h>
#include "scatola-extension-types.h"

G_BEGIN_DECLS

#define SCATOLA_TYPE_COLUMN            (scatola_column_get_type())
#define SCATOLA_COLUMN(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), SCATOLA_TYPE_COLUMN, ScatolaColumn))
#define SCATOLA_COLUMN_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), SCATOLA_TYPE_COLUMN, ScatolaColumnClass))
#define SCATOLA_INFO_IS_COLUMN(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SCATOLA_TYPE_COLUMN))
#define SCATOLA_INFO_IS_COLUMN_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((obj), SCATOLA_TYPE_COLUMN))
#define SCATOLA_COLUMN_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj), SCATOLA_TYPE_COLUMN, ScatolaColumnClass))

typedef struct _ScatolaColumn        ScatolaColumn;
typedef struct _ScatolaColumnDetails ScatolaColumnDetails;
typedef struct _ScatolaColumnClass   ScatolaColumnClass;

struct _ScatolaColumn {
    GObject parent;

    ScatolaColumnDetails *details;
};

struct _ScatolaColumnClass {
    GObjectClass parent;
};

GType       scatola_column_get_type  (void);
ScatolaColumn *scatola_column_new       (const char *name,
                                   const char *attribute,
                                   const char *label,
                                   const char *description);

/* ScatolaColumn has the following properties:
 *   name (string)        - the identifier for the column
 *   attribute (string)   - the file attribute to be displayed in the
 *                          column
 *   label (string)       - the user-visible label for the column
 *   description (string) - a user-visible description of the column
 *   xalign (float)       - x-alignment of the column
 */

G_END_DECLS

#endif
