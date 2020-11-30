/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*-

   scatola-window-pane.h: Scatola window pane

   Copyright (C) 2008 Free Software Foundation, Inc.

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

   Author: Holger Berndt <berndth@gmx.de>
*/

#ifndef SCATOLA_WINDOW_PANE_H
#define SCATOLA_WINDOW_PANE_H

#include "scatola-window.h"

#define SCATOLA_TYPE_WINDOW_PANE	 (scatola_window_pane_get_type())
#define SCATOLA_WINDOW_PANE_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST((k), SCATOLA_TYPE_WINDOW_PANE, ScatolaWindowPaneClass))
#define SCATOLA_WINDOW_PANE(obj)	 (G_TYPE_CHECK_INSTANCE_CAST ((obj), SCATOLA_TYPE_WINDOW_PANE, ScatolaWindowPane))
#define SCATOLA_IS_WINDOW_PANE(obj)      (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SCATOLA_TYPE_WINDOW_PANE))
#define SCATOLA_IS_WINDOW_PANE_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE ((k), SCATOLA_TYPE_WINDOW_PANE))
#define SCATOLA_WINDOW_PANE_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), SCATOLA_TYPE_WINDOW_PANE, ScatolaWindowPaneClass))

typedef struct _ScatolaWindowPaneClass ScatolaWindowPaneClass;

struct _ScatolaWindowPaneClass
{
    GObjectClass parent_class;

    void (*show) (ScatolaWindowPane *pane);
    void (*set_active) (ScatolaWindowPane *pane,
                        gboolean is_active);
    void (*sync_search_widgets) (ScatolaWindowPane *pane);
    void (*sync_location_widgets) (ScatolaWindowPane *pane);
};

/* A ScatolaWindowPane is a layer between a slot and a window.
 * Each slot is contained in one pane, and each pane can contain
 * one or more slots. It also supports the notion of an "active slot".
 * On the other hand, each pane is contained in a window, while each
 * window can contain one or multiple panes. Likewise, the window has
 * the notion of an "active pane".
 *
 * A spatial window has only one pane, which contains a single slot.
 * A navigation window may have one or more panes.
 */
struct _ScatolaWindowPane
{
    GObject parent;

    /* hosting window */
    ScatolaWindow *window;
    gboolean visible;

    /* available slots, and active slot.
     * Both of them may never be NULL. */
    GList *slots;
    GList *active_slots;
    ScatolaWindowSlot *active_slot;

    /* whether or not this pane is active */
    gboolean is_active;
};

GType scatola_window_pane_get_type (void);
ScatolaWindowPane *scatola_window_pane_new (ScatolaWindow *window);


void scatola_window_pane_show (ScatolaWindowPane *pane);
void scatola_window_pane_zoom_in (ScatolaWindowPane *pane);
void scatola_window_pane_zoom_to_level (ScatolaWindowPane *pane, ScatolaZoomLevel level);
void scatola_window_pane_zoom_out (ScatolaWindowPane *pane);
void scatola_window_pane_zoom_to_default (ScatolaWindowPane *pane);
void scatola_window_pane_sync_location_widgets (ScatolaWindowPane *pane);
void scatola_window_pane_sync_search_widgets  (ScatolaWindowPane *pane);
void scatola_window_pane_set_active (ScatolaWindowPane *pane, gboolean is_active);
void scatola_window_pane_slot_close (ScatolaWindowPane *pane, ScatolaWindowSlot *slot);

ScatolaWindowSlot* scatola_window_pane_get_slot_for_content_box (ScatolaWindowPane *pane, GtkWidget *content_box);
void scatola_window_pane_switch_to (ScatolaWindowPane *pane);
void scatola_window_pane_grab_focus (ScatolaWindowPane *pane);


#endif /* SCATOLA_WINDOW_PANE_H */
