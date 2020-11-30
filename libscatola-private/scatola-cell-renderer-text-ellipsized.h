/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*-

   scatola-cell-renderer-text-ellipsized.c: Cell renderer for text which
   will use pango ellipsization but deactivate it temporarily for the size
   calculation to get the size based on the actual text length.

   Copyright (C) 2007 Martin Wehner

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public
   License along with this program; if not, write to the
   Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
   Boston, MA 02110-1301, USA.

   Author: Martin Wehner <martin.wehner@gmail.com>
*/

#ifndef SCATOLA_CELL_RENDERER_TEXT_ELLIPSIZED_H
#define SCATOLA_CELL_RENDERER_TEXT_ELLIPSIZED_H

#include <gtk/gtk.h>

#define SCATOLA_TYPE_CELL_RENDERER_TEXT_ELLIPSIZED scatola_cell_renderer_text_ellipsized_get_type()
#define SCATOLA_CELL_RENDERER_TEXT_ELLIPSIZED(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), SCATOLA_TYPE_CELL_RENDERER_TEXT_ELLIPSIZED, ScatolaCellRendererTextEllipsized))
#define SCATOLA_CELL_RENDERER_TEXT_ELLIPSIZED_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), SCATOLA_TYPE_CELL_RENDERER_TEXT_ELLIPSIZED, ScatolaCellRendererTextEllipsizedClass))
#define SCATOLA_IS_CELL_RENDERER_TEXT_ELLIPSIZED(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SCATOLA_TYPE_CELL_RENDERER_TEXT_ELLIPSIZED))
#define SCATOLA_IS_CELL_RENDERER_TEXT_ELLIPSIZED_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), SCATOLA_TYPE_CELL_RENDERER_TEXT_ELLIPSIZED))
#define SCATOLA_CELL_RENDERER_TEXT_ELLIPSIZED_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), SCATOLA_TYPE_CELL_RENDERER_TEXT_ELLIPSIZED, ScatolaCellRendererTextEllipsizedClass))


typedef struct _ScatolaCellRendererTextEllipsized ScatolaCellRendererTextEllipsized;
typedef struct _ScatolaCellRendererTextEllipsizedClass ScatolaCellRendererTextEllipsizedClass;

struct _ScatolaCellRendererTextEllipsized
{
    GtkCellRendererText parent;
};

struct _ScatolaCellRendererTextEllipsizedClass
{
    GtkCellRendererTextClass parent_class;
};

GType		 scatola_cell_renderer_text_ellipsized_get_type (void);
GtkCellRenderer *scatola_cell_renderer_text_ellipsized_new      (void);

#endif /* SCATOLA_CELL_RENDERER_TEXT_ELLIPSIZED_H */
