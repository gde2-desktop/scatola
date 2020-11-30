/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*-

   scatola-navigation-window-slot.h: Scatola navigation window slot

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

   Author: Christian Neumair <cneumair@gnome.org>
*/

#ifndef SCATOLA_NAVIGATION_WINDOW_SLOT_H
#define SCATOLA_NAVIGATION_WINDOW_SLOT_H

#include "scatola-window-slot.h"

typedef struct ScatolaNavigationWindowSlot ScatolaNavigationWindowSlot;
typedef struct ScatolaNavigationWindowSlotClass ScatolaNavigationWindowSlotClass;


#define SCATOLA_TYPE_NAVIGATION_WINDOW_SLOT         (scatola_navigation_window_slot_get_type())
#define SCATOLA_NAVIGATION_WINDOW_SLOT_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST((k), SCATOLA_NAVIGATION_WINDOW_SLOT_CLASS, ScatolaNavigationWindowSlotClass))
#define SCATOLA_NAVIGATION_WINDOW_SLOT(obj)         (G_TYPE_CHECK_INSTANCE_CAST ((obj), SCATOLA_TYPE_NAVIGATION_WINDOW_SLOT, ScatolaNavigationWindowSlot))
#define SCATOLA_IS_NAVIGATION_WINDOW_SLOT(obj)      (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SCATOLA_TYPE_NAVIGATION_WINDOW_SLOT))
#define SCATOLA_IS_NAVIGATION_WINDOW_SLOT_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE ((k), SCATOLA_TYPE_NAVIGATION_WINDOW_SLOT))
#define SCATOLA_NAVIGATION_WINDOW_SLOT_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), SCATOLA_TYPE_NAVIGATION_WINDOW_SLOT, ScatolaNavigationWindowSlotClass))

typedef enum
{
    SCATOLA_BAR_PATH,
    SCATOLA_BAR_NAVIGATION,
    SCATOLA_BAR_SEARCH
} ScatolaBarMode;

struct ScatolaNavigationWindowSlot
{
    ScatolaWindowSlot parent;

    ScatolaBarMode bar_mode;
    GtkTreeModel *viewer_model;
    int num_viewers;

    /* Back/Forward chain, and history list.
     * The data in these lists are ScatolaBookmark pointers.
     */
    GList *back_list, *forward_list;

    /* Current views stuff */
    GList *sidebar_panels;
};

struct ScatolaNavigationWindowSlotClass
{
    ScatolaWindowSlotClass parent;
};

GType scatola_navigation_window_slot_get_type (void);

gboolean scatola_navigation_window_slot_should_close_with_mount (ScatolaNavigationWindowSlot *slot,
        GMount *mount);

void scatola_navigation_window_slot_clear_forward_list (ScatolaNavigationWindowSlot *slot);
void scatola_navigation_window_slot_clear_back_list    (ScatolaNavigationWindowSlot *slot);

#endif /* SCATOLA_NAVIGATION_WINDOW_SLOT_H */
