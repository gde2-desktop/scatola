/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/*  scatola-side-pane.c
 *
 *  Copyright (C) 2002 Ximian, Inc.
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
 *  Author: Dave Camp <dave@ximian.com>
 */

#ifndef SCATOLA_SIDE_PANE_H
#define SCATOLA_SIDE_PANE_H

#include <gtk/gtk.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SCATOLA_TYPE_SIDE_PANE scatola_side_pane_get_type()
#define SCATOLA_SIDE_PANE(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), SCATOLA_TYPE_SIDE_PANE, ScatolaSidePane))
#define SCATOLA_SIDE_PANE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), SCATOLA_TYPE_SIDE_PANE, ScatolaSidePaneClass))
#define SCATOLA_IS_SIDE_PANE(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SCATOLA_TYPE_SIDE_PANE))
#define SCATOLA_IS_SIDE_PANE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), SCATOLA_TYPE_SIDE_PANE))
#define SCATOLA_SIDE_PANE_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), SCATOLA_TYPE_SIDE_PANE, ScatolaSidePaneClass))

    typedef struct _ScatolaSidePaneDetails ScatolaSidePaneDetails;

    typedef struct
    {
#if GTK_CHECK_VERSION (3, 0, 0)
        GtkBox parent;
#else
        GtkVBox parent;
#endif
        ScatolaSidePaneDetails *details;
    } ScatolaSidePane;

    typedef struct
    {
#if GTK_CHECK_VERSION (3, 0, 0)
        GtkBoxClass parent_slot;
#else
        GtkVBoxClass parent_slot;
#endif

        void (*close_requested) (ScatolaSidePane *side_pane);
        void (*switch_page) (ScatolaSidePane *side_pane,
                             GtkWidget *child);
    } ScatolaSidePaneClass;

    GType                  scatola_side_pane_get_type        (void);
    ScatolaSidePane      *scatola_side_pane_new             (void);
    void                   scatola_side_pane_add_panel       (ScatolaSidePane *side_pane,
            GtkWidget        *widget,
            const char       *title,
            const char       *tooltip);
    void                   scatola_side_pane_remove_panel    (ScatolaSidePane *side_pane,
            GtkWidget        *widget);
    void                   scatola_side_pane_show_panel      (ScatolaSidePane *side_pane,
            GtkWidget        *widget);
    void                   scatola_side_pane_set_panel_image (ScatolaSidePane *side_pane,
            GtkWidget        *widget,
            GdkPixbuf        *pixbuf);
    GtkWidget             *scatola_side_pane_get_current_panel (ScatolaSidePane *side_pane);
    GtkWidget             *scatola_side_pane_get_title        (ScatolaSidePane *side_pane);

#ifdef __cplusplus
}
#endif

#endif /* SCATOLA_SIDE_PANE_H */
