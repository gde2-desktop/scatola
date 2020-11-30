/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*-

   scatola-window-slot-info.c: Interface for scatola window slots

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
#include "scatola-window-slot-info.h"

enum
{
    ACTIVE,
    INACTIVE,
    LAST_SIGNAL
};

static guint scatola_window_slot_info_signals[LAST_SIGNAL] = { 0 };

static void
scatola_window_slot_info_base_init (gpointer g_class)
{
    static gboolean initialized = FALSE;

    if (!initialized)
    {
        scatola_window_slot_info_signals[ACTIVE] =
            g_signal_new ("active",
                          SCATOLA_TYPE_WINDOW_SLOT_INFO,
                          G_SIGNAL_RUN_LAST,
                          G_STRUCT_OFFSET (ScatolaWindowSlotInfoIface, active),
                          NULL, NULL,
                          g_cclosure_marshal_VOID__VOID,
                          G_TYPE_NONE, 0);

        scatola_window_slot_info_signals[INACTIVE] =
            g_signal_new ("inactive",
                          SCATOLA_TYPE_WINDOW_SLOT_INFO,
                          G_SIGNAL_RUN_LAST,
                          G_STRUCT_OFFSET (ScatolaWindowSlotInfoIface, inactive),
                          NULL, NULL,
                          g_cclosure_marshal_VOID__VOID,
                          G_TYPE_NONE, 0);

        initialized = TRUE;
    }
}

GType
scatola_window_slot_info_get_type (void)
{
    static GType type = 0;

    if (!type)
    {
        const GTypeInfo info =
        {
            sizeof (ScatolaWindowSlotInfoIface),
            scatola_window_slot_info_base_init,
            NULL,
            NULL,
            NULL,
            NULL,
            0,
            0,
            NULL
        };

        type = g_type_register_static (G_TYPE_INTERFACE,
                                       "ScatolaWindowSlotInfo",
                                       &info, 0);
        g_type_interface_add_prerequisite (type, G_TYPE_OBJECT);
    }

    return type;
}

void
scatola_window_slot_info_set_status (ScatolaWindowSlotInfo *slot,
                                  const char             *status)
{
    g_assert (SCATOLA_IS_WINDOW_SLOT_INFO (slot));

    (* SCATOLA_WINDOW_SLOT_INFO_GET_IFACE (slot)->set_status) (slot,
            status);
}

void
scatola_window_slot_info_make_hosting_pane_active (ScatolaWindowSlotInfo *slot)
{
    g_assert (SCATOLA_IS_WINDOW_SLOT_INFO (slot));
    (* SCATOLA_WINDOW_SLOT_INFO_GET_IFACE (slot)->make_hosting_pane_active) (slot);
}

void
scatola_window_slot_info_open_location_full (ScatolaWindowSlotInfo  *slot,
                                     GFile                   *location,
                                     ScatolaWindowOpenMode       mode,
                                     ScatolaWindowOpenFlags      flags,
                                     GList                   *selection,
                                     ScatolaWindowGoToCallback   callback,
                                     gpointer user_data)
{
    g_assert (SCATOLA_IS_WINDOW_SLOT_INFO (slot));

    (* SCATOLA_WINDOW_SLOT_INFO_GET_IFACE (slot)->open_location) (slot,
            location,
            mode,
            flags,
            selection,
            callback,
            user_data);
}

char *
scatola_window_slot_info_get_title (ScatolaWindowSlotInfo *slot)
{
    g_assert (SCATOLA_IS_WINDOW_SLOT_INFO (slot));

    return (* SCATOLA_WINDOW_SLOT_INFO_GET_IFACE (slot)->get_title) (slot);
}

char *
scatola_window_slot_info_get_current_location (ScatolaWindowSlotInfo *slot)
{
    g_assert (SCATOLA_IS_WINDOW_SLOT_INFO (slot));

    return (* SCATOLA_WINDOW_SLOT_INFO_GET_IFACE (slot)->get_current_location) (slot);
}

ScatolaView *
scatola_window_slot_info_get_current_view (ScatolaWindowSlotInfo *slot)
{
    g_assert (SCATOLA_IS_WINDOW_SLOT_INFO (slot));

    return (* SCATOLA_WINDOW_SLOT_INFO_GET_IFACE (slot)->get_current_view) (slot);
}

ScatolaWindowInfo *
scatola_window_slot_info_get_window (ScatolaWindowSlotInfo *slot)
{
    g_assert (SCATOLA_IS_WINDOW_SLOT_INFO (slot));

    return (* SCATOLA_WINDOW_SLOT_INFO_GET_IFACE (slot)->get_window) (slot);
}

