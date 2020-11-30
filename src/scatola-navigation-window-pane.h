/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*-

   scatola-navigation-window-pane.h: Scatola navigation window pane

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

#ifndef SCATOLA_NAVIGATION_WINDOW_PANE_H
#define SCATOLA_NAVIGATION_WINDOW_PANE_H

#include "scatola-window-pane.h"
#include "scatola-navigation-window-slot.h"

#define SCATOLA_TYPE_NAVIGATION_WINDOW_PANE     (scatola_navigation_window_pane_get_type())
#define SCATOLA_NAVIGATION_WINDOW_PANE_CLASS(k) (G_TYPE_CHECK_CLASS_CAST((k), SCATOLA_TYPE_NAVIGATION_WINDOW_PANE, ScatolaNavigationWindowPaneClass))
#define SCATOLA_NAVIGATION_WINDOW_PANE(obj)     (G_TYPE_CHECK_INSTANCE_CAST ((obj), SCATOLA_TYPE_NAVIGATION_WINDOW_PANE, ScatolaNavigationWindowPane))
#define SCATOLA_IS_NAVIGATION_WINDOW_PANE(obj)  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SCATOLA_TYPE_NAVIGATION_WINDOW_PANE))
#define SCATOLA_IS_NAVIGATION_WINDOW_PANE_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE ((k), SCATOLA_TYPE_NAVIGATION_WINDOW_PANE))
#define SCATOLA_NAVIGATION_WINDOW_PANE_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), SCATOLA_TYPE_NAVIGATION_WINDOW_PANE, ScatolaNavigationWindowPaneClass))

typedef struct _ScatolaNavigationWindowPaneClass ScatolaNavigationWindowPaneClass;
typedef struct _ScatolaNavigationWindowPane      ScatolaNavigationWindowPane;

struct _ScatolaNavigationWindowPaneClass
{
    ScatolaWindowPaneClass parent_class;
};

struct _ScatolaNavigationWindowPane
{
    ScatolaWindowPane parent;

    GtkWidget *widget;

    /* location bar */
    GtkWidget *location_bar;
    GtkWidget *location_button;
    GtkWidget *navigation_bar;
    GtkWidget *path_bar;
    GtkWidget *search_bar;

    gboolean temporary_navigation_bar;
    gboolean temporary_location_bar;
    gboolean temporary_search_bar;

    /* notebook */
    GtkWidget *notebook;

    /* split view */
    GtkWidget *split_view_hpane;
};

GType    scatola_navigation_window_pane_get_type (void);

ScatolaNavigationWindowPane* scatola_navigation_window_pane_new (ScatolaWindow *window);

/* location bar */
void     scatola_navigation_window_pane_setup             (ScatolaNavigationWindowPane *pane);

void     scatola_navigation_window_pane_hide_location_bar (ScatolaNavigationWindowPane *pane, gboolean save_preference);
void     scatola_navigation_window_pane_show_location_bar (ScatolaNavigationWindowPane *pane, gboolean save_preference);
gboolean scatola_navigation_window_pane_location_bar_showing (ScatolaNavigationWindowPane *pane);
void     scatola_navigation_window_pane_hide_path_bar (ScatolaNavigationWindowPane *pane);
void     scatola_navigation_window_pane_show_path_bar (ScatolaNavigationWindowPane *pane);
gboolean scatola_navigation_window_pane_path_bar_showing (ScatolaNavigationWindowPane *pane);
gboolean scatola_navigation_window_pane_search_bar_showing (ScatolaNavigationWindowPane *pane);
void     scatola_navigation_window_pane_set_bar_mode  (ScatolaNavigationWindowPane *pane, ScatolaBarMode mode);
void     scatola_navigation_window_pane_show_location_bar_temporarily (ScatolaNavigationWindowPane *pane);
void     scatola_navigation_window_pane_show_navigation_bar_temporarily (ScatolaNavigationWindowPane *pane);
void     scatola_navigation_window_pane_always_use_location_entry (ScatolaNavigationWindowPane *pane, gboolean use_entry);
gboolean scatola_navigation_window_pane_hide_temporary_bars (ScatolaNavigationWindowPane *pane);
/* notebook */
void     scatola_navigation_window_pane_add_slot_in_tab (ScatolaNavigationWindowPane *pane, ScatolaWindowSlot *slot, ScatolaWindowOpenSlotFlags flags);
void     scatola_navigation_window_pane_remove_page (ScatolaNavigationWindowPane *pane, int page_num);

#endif /* SCATOLA_NAVIGATION_WINDOW_PANE_H */
