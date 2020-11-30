/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */
/*
 * Copyright (C) 2005 Red Hat, Inc.
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
 * Author: Alexander Larsson <alexl@redhat.com>
 *
 */

#ifndef SCATOLA_QUERY_EDITOR_H
#define SCATOLA_QUERY_EDITOR_H

#include <gtk/gtk.h>
#include <libscatola-private/scatola-query.h>
#include <libscatola-private/scatola-window-info.h>
#include <scatola-search-bar.h>

#define SCATOLA_TYPE_QUERY_EDITOR scatola_query_editor_get_type()
#define SCATOLA_QUERY_EDITOR(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), SCATOLA_TYPE_QUERY_EDITOR, ScatolaQueryEditor))
#define SCATOLA_QUERY_EDITOR_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), SCATOLA_TYPE_QUERY_EDITOR, ScatolaQueryEditorClass))
#define SCATOLA_IS_QUERY_EDITOR(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SCATOLA_TYPE_QUERY_EDITOR))
#define SCATOLA_IS_QUERY_EDITOR_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), SCATOLA_TYPE_QUERY_EDITOR))
#define SCATOLA_QUERY_EDITOR_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), SCATOLA_TYPE_QUERY_EDITOR, ScatolaQueryEditorClass))

typedef struct ScatolaQueryEditorDetails ScatolaQueryEditorDetails;

typedef struct ScatolaQueryEditor
{
#if GTK_CHECK_VERSION (3, 0, 0)
    GtkBox parent;
#else
    GtkVBox parent;
#endif
    ScatolaQueryEditorDetails *details;
} ScatolaQueryEditor;

typedef struct
{
#if GTK_CHECK_VERSION (3, 0, 0)
    GtkBoxClass parent_class;
#else
    GtkVBoxClass parent_class;
#endif

    void (* changed) (ScatolaQueryEditor  *editor,
                      ScatolaQuery        *query,
                      gboolean              reload);
    void (* cancel)   (ScatolaQueryEditor *editor);
} ScatolaQueryEditorClass;

GType      scatola_query_editor_get_type     	   (void);
GtkWidget* scatola_query_editor_new          	   (gboolean start_hidden,
        gboolean is_indexed);
GtkWidget* scatola_query_editor_new_with_bar      (gboolean start_hidden,
        gboolean is_indexed,
        gboolean start_attached,
        ScatolaSearchBar *bar,
        ScatolaWindowSlot *slot);
void       scatola_query_editor_set_default_query (ScatolaQueryEditor *editor);

void	   scatola_query_editor_grab_focus (ScatolaQueryEditor *editor);
void       scatola_query_editor_clear_query (ScatolaQueryEditor *editor);

ScatolaQuery *scatola_query_editor_get_query   (ScatolaQueryEditor *editor);
void           scatola_query_editor_set_query   (ScatolaQueryEditor *editor,
        ScatolaQuery       *query);
void           scatola_query_editor_set_visible (ScatolaQueryEditor *editor,
        gboolean             visible);

#endif /* SCATOLA_QUERY_EDITOR_H */
