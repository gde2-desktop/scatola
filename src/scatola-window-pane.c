/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*-

   scatola-window-pane.c: Scatola window pane

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

#include "scatola-window-pane.h"
#include "scatola-window-private.h"
#include "scatola-navigation-window-pane.h"
#include "scatola-window-manage-views.h"
#include <eel/eel-gtk-macros.h>

static void scatola_window_pane_init       (ScatolaWindowPane *pane);
static void scatola_window_pane_class_init (ScatolaWindowPaneClass *class);
static void scatola_window_pane_dispose    (GObject *object);

G_DEFINE_TYPE (ScatolaWindowPane,
               scatola_window_pane,
               G_TYPE_OBJECT)
#define parent_class scatola_window_pane_parent_class


static inline ScatolaWindowSlot *
get_first_inactive_slot (ScatolaWindowPane *pane)
{
    GList *l;
    ScatolaWindowSlot *slot;

    for (l = pane->slots; l != NULL; l = l->next)
    {
        slot = SCATOLA_WINDOW_SLOT (l->data);
        if (slot != pane->active_slot)
        {
            return slot;
        }
    }

    return NULL;
}

void
scatola_window_pane_show (ScatolaWindowPane *pane)
{
    pane->visible = TRUE;
    EEL_CALL_METHOD (SCATOLA_WINDOW_PANE_CLASS, pane,
                     show, (pane));
}

void
scatola_window_pane_zoom_in (ScatolaWindowPane *pane)
{
    ScatolaWindowSlot *slot;

    g_assert (pane != NULL);

    scatola_window_set_active_pane (pane->window, pane);

    slot = pane->active_slot;
    if (slot->content_view != NULL)
    {
        scatola_view_bump_zoom_level (slot->content_view, 1);
    }
}

void
scatola_window_pane_zoom_to_level (ScatolaWindowPane *pane,
                                ScatolaZoomLevel level)
{
    ScatolaWindowSlot *slot;

    g_assert (pane != NULL);

    scatola_window_set_active_pane (pane->window, pane);

    slot = pane->active_slot;
    if (slot->content_view != NULL)
    {
        scatola_view_zoom_to_level (slot->content_view, level);
    }
}

void
scatola_window_pane_zoom_out (ScatolaWindowPane *pane)
{
    ScatolaWindowSlot *slot;

    g_assert (pane != NULL);

    scatola_window_set_active_pane (pane->window, pane);

    slot = pane->active_slot;
    if (slot->content_view != NULL)
    {
        scatola_view_bump_zoom_level (slot->content_view, -1);
    }
}

void
scatola_window_pane_zoom_to_default (ScatolaWindowPane *pane)
{
    ScatolaWindowSlot *slot;

    g_assert (pane != NULL);

    scatola_window_set_active_pane (pane->window, pane);

    slot = pane->active_slot;
    if (slot->content_view != NULL)
    {
        scatola_view_restore_default_zoom_level (slot->content_view);
    }
}

void
scatola_window_pane_slot_close (ScatolaWindowPane *pane, ScatolaWindowSlot *slot)
{
    ScatolaWindowSlot *next_slot;

    if (pane->window)
    {
        ScatolaWindow *window;
        window = pane->window;
        if (pane->active_slot == slot)
        {
            g_assert (pane->active_slots != NULL);
            g_assert (pane->active_slots->data == slot);

            next_slot = NULL;
            if (pane->active_slots->next != NULL)
            {
                next_slot = SCATOLA_WINDOW_SLOT (pane->active_slots->next->data);
            }

            if (next_slot == NULL)
            {
                next_slot = get_first_inactive_slot (SCATOLA_WINDOW_PANE (pane));
            }

            scatola_window_set_active_slot (window, next_slot);
        }
        scatola_window_close_slot (slot);

        /* If that was the last slot in the active pane, close the pane or even the whole window. */
        if (window->details->active_pane->slots == NULL)
        {
            ScatolaWindowPane *next_pane;
            next_pane = scatola_window_get_next_pane (window);

            /* If next_pane is non-NULL, we have more than one pane available. In this
             * case, close the current pane and switch to the next one. If there is
             * no next pane, close the window. */
            if(next_pane)
            {
                scatola_window_close_pane (pane);
                scatola_window_pane_switch_to (next_pane);
                if (SCATOLA_IS_NAVIGATION_WINDOW (window))
                {
                    scatola_navigation_window_update_show_hide_menu_items (SCATOLA_NAVIGATION_WINDOW (window));
                }
            }
            else
            {
                scatola_window_close (window);
            }
        }
    }
}

static void
real_sync_location_widgets (ScatolaWindowPane *pane)
{
    ScatolaWindowSlot *slot;

    /* TODO: Would be nice with a real subclass for spatial panes */
    g_assert (SCATOLA_IS_SPATIAL_WINDOW (pane->window));

    slot = pane->active_slot;

    /* Change the location button to match the current location. */
    scatola_spatial_window_set_location_button (SCATOLA_SPATIAL_WINDOW (pane->window),
            slot->location);
}


void
scatola_window_pane_sync_location_widgets (ScatolaWindowPane *pane)
{
    EEL_CALL_METHOD (SCATOLA_WINDOW_PANE_CLASS, pane,
                     sync_location_widgets, (pane));
}

void
scatola_window_pane_sync_search_widgets (ScatolaWindowPane *pane)
{
    g_assert (SCATOLA_IS_WINDOW_PANE (pane));

    EEL_CALL_METHOD (SCATOLA_WINDOW_PANE_CLASS, pane,
                     sync_search_widgets, (pane));
}

void
scatola_window_pane_grab_focus (ScatolaWindowPane *pane)
{
    if (SCATOLA_IS_WINDOW_PANE (pane) && pane->active_slot)
    {
        scatola_view_grab_focus (pane->active_slot->content_view);
    }
}

void
scatola_window_pane_switch_to (ScatolaWindowPane *pane)
{
    scatola_window_pane_grab_focus (pane);
}

static void
scatola_window_pane_init (ScatolaWindowPane *pane)
{
    pane->slots = NULL;
    pane->active_slots = NULL;
    pane->active_slot = NULL;
    pane->is_active = FALSE;
}

void
scatola_window_pane_set_active (ScatolaWindowPane *pane, gboolean is_active)
{
    if (is_active == pane->is_active)
    {
        return;
    }

    pane->is_active = is_active;

    /* notify the current slot about its activity state (so that it can e.g. modify the bg color) */
    scatola_window_slot_is_in_active_pane (pane->active_slot, is_active);

    EEL_CALL_METHOD (SCATOLA_WINDOW_PANE_CLASS, pane,
                     set_active, (pane, is_active));
}

static void
scatola_window_pane_class_init (ScatolaWindowPaneClass *class)
{
    G_OBJECT_CLASS (class)->dispose = scatola_window_pane_dispose;
    SCATOLA_WINDOW_PANE_CLASS (class)->sync_location_widgets = real_sync_location_widgets;
}

static void
scatola_window_pane_dispose (GObject *object)
{
    ScatolaWindowPane *pane = SCATOLA_WINDOW_PANE (object);

    g_assert (pane->slots == NULL);

    pane->window = NULL;
    G_OBJECT_CLASS (parent_class)->dispose (object);
}

ScatolaWindowPane *
scatola_window_pane_new (ScatolaWindow *window)
{
    ScatolaWindowPane *pane;

    pane = g_object_new (SCATOLA_TYPE_WINDOW_PANE, NULL);
    pane->window = window;
    return pane;
}

ScatolaWindowSlot *
scatola_window_pane_get_slot_for_content_box (ScatolaWindowPane *pane,
        GtkWidget *content_box)
{
    ScatolaWindowSlot *slot;
    GList *l;

    for (l = pane->slots; l != NULL; l = l->next)
    {
        slot = SCATOLA_WINDOW_SLOT (l->data);

        if (slot->content_box == content_box)
        {
            return slot;
        }
    }
    return NULL;
}
