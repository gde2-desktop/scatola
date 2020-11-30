/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*-

   scatola-window-info.c: Interface for scatola window

   Copyright (C) 2004 Red Hat Inc.

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

   Author: Alexander Larsson <alexl@redhat.com>
*/

#include <config.h>
#include "scatola-window-info.h"

enum
{
    LOADING_URI,
    SELECTION_CHANGED,
    TITLE_CHANGED,
    HIDDEN_FILES_MODE_CHANGED,
    LAST_SIGNAL
};

static guint scatola_window_info_signals[LAST_SIGNAL] = { 0 };

static void
scatola_window_info_base_init (gpointer g_class)
{
    static gboolean initialized = FALSE;

    if (! initialized)
    {
        scatola_window_info_signals[LOADING_URI] =
            g_signal_new ("loading_uri",
                          SCATOLA_TYPE_WINDOW_INFO,
                          G_SIGNAL_RUN_LAST,
                          G_STRUCT_OFFSET (ScatolaWindowInfoIface, loading_uri),
                          NULL, NULL,
                          g_cclosure_marshal_VOID__STRING,
                          G_TYPE_NONE, 1,
                          G_TYPE_STRING);

        scatola_window_info_signals[SELECTION_CHANGED] =
            g_signal_new ("selection_changed",
                          SCATOLA_TYPE_WINDOW_INFO,
                          G_SIGNAL_RUN_LAST,
                          G_STRUCT_OFFSET (ScatolaWindowInfoIface, selection_changed),
                          NULL, NULL,
                          g_cclosure_marshal_VOID__VOID,
                          G_TYPE_NONE, 0);

        scatola_window_info_signals[TITLE_CHANGED] =
            g_signal_new ("title_changed",
                          SCATOLA_TYPE_WINDOW_INFO,
                          G_SIGNAL_RUN_LAST,
                          G_STRUCT_OFFSET (ScatolaWindowInfoIface, title_changed),
                          NULL, NULL,
                          g_cclosure_marshal_VOID__STRING,
                          G_TYPE_NONE, 1,
                          G_TYPE_STRING);

        scatola_window_info_signals[HIDDEN_FILES_MODE_CHANGED] =
            g_signal_new ("hidden_files_mode_changed",
                          SCATOLA_TYPE_WINDOW_INFO,
                          G_SIGNAL_RUN_LAST,
                          G_STRUCT_OFFSET (ScatolaWindowInfoIface, hidden_files_mode_changed),
                          NULL, NULL,
                          g_cclosure_marshal_VOID__VOID,
                          G_TYPE_NONE, 0);

        initialized = TRUE;
    }
}

GType
scatola_window_info_get_type (void)
{
    static GType type = 0;

    if (!type)
    {
        const GTypeInfo info =
        {
            sizeof (ScatolaWindowInfoIface),
            scatola_window_info_base_init,
            NULL,
            NULL,
            NULL,
            NULL,
            0,
            0,
            NULL
        };

        type = g_type_register_static (G_TYPE_INTERFACE,
                                       "ScatolaWindowInfo",
                                       &info, 0);
        g_type_interface_add_prerequisite (type, G_TYPE_OBJECT);
    }

    return type;
}

void
scatola_window_info_report_load_underway (ScatolaWindowInfo      *window,
                                       ScatolaView            *view)
{
    g_return_if_fail (SCATOLA_IS_WINDOW_INFO (window));
    g_return_if_fail (SCATOLA_IS_VIEW (view));

    (* SCATOLA_WINDOW_INFO_GET_IFACE (window)->report_load_underway) (window,
            view);
}

void
scatola_window_info_report_load_complete (ScatolaWindowInfo      *window,
                                       ScatolaView            *view)
{
    g_return_if_fail (SCATOLA_IS_WINDOW_INFO (window));
    g_return_if_fail (SCATOLA_IS_VIEW (view));

    (* SCATOLA_WINDOW_INFO_GET_IFACE (window)->report_load_complete) (window,
            view);
}

void
scatola_window_info_report_view_failed (ScatolaWindowInfo      *window,
                                     ScatolaView            *view)
{
    g_return_if_fail (SCATOLA_IS_WINDOW_INFO (window));
    g_return_if_fail (SCATOLA_IS_VIEW (view));

    (* SCATOLA_WINDOW_INFO_GET_IFACE (window)->report_view_failed) (window,
            view);
}

void
scatola_window_info_report_selection_changed (ScatolaWindowInfo      *window)
{
    g_return_if_fail (SCATOLA_IS_WINDOW_INFO (window));

    (* SCATOLA_WINDOW_INFO_GET_IFACE (window)->report_selection_changed) (window);
}

void
scatola_window_info_view_visible (ScatolaWindowInfo      *window,
                               ScatolaView            *view)
{
    g_return_if_fail (SCATOLA_IS_WINDOW_INFO (window));

    (* SCATOLA_WINDOW_INFO_GET_IFACE (window)->view_visible) (window, view);
}

void
scatola_window_info_close (ScatolaWindowInfo      *window)
{
    g_return_if_fail (SCATOLA_IS_WINDOW_INFO (window));

    (* SCATOLA_WINDOW_INFO_GET_IFACE (window)->close_window) (window);
}

void
scatola_window_info_push_status (ScatolaWindowInfo      *window,
                              const char              *status)
{
    g_return_if_fail (SCATOLA_IS_WINDOW_INFO (window));

    (* SCATOLA_WINDOW_INFO_GET_IFACE (window)->push_status) (window,
            status);
}

ScatolaWindowType
scatola_window_info_get_window_type (ScatolaWindowInfo *window)
{
    g_return_val_if_fail (SCATOLA_IS_WINDOW_INFO (window), SCATOLA_WINDOW_SPATIAL);

    return (* SCATOLA_WINDOW_INFO_GET_IFACE (window)->get_window_type) (window);
}

char *
scatola_window_info_get_title (ScatolaWindowInfo *window)
{
    g_return_val_if_fail (SCATOLA_IS_WINDOW_INFO (window), NULL);

    return (* SCATOLA_WINDOW_INFO_GET_IFACE (window)->get_title) (window);
}

GList *
scatola_window_info_get_history (ScatolaWindowInfo *window)
{
    g_return_val_if_fail (SCATOLA_IS_WINDOW_INFO (window), NULL);

    return (* SCATOLA_WINDOW_INFO_GET_IFACE (window)->get_history) (window);
}

char *
scatola_window_info_get_current_location (ScatolaWindowInfo *window)
{
    g_return_val_if_fail (SCATOLA_IS_WINDOW_INFO (window), NULL);

    return (* SCATOLA_WINDOW_INFO_GET_IFACE (window)->get_current_location) (window);
}

int
scatola_window_info_get_selection_count (ScatolaWindowInfo *window)
{
    g_return_val_if_fail (SCATOLA_IS_WINDOW_INFO (window), 0);

    return (* SCATOLA_WINDOW_INFO_GET_IFACE (window)->get_selection_count) (window);
}

GList *
scatola_window_info_get_selection (ScatolaWindowInfo *window)
{
    g_return_val_if_fail (SCATOLA_IS_WINDOW_INFO (window), NULL);

    return (* SCATOLA_WINDOW_INFO_GET_IFACE (window)->get_selection) (window);
}

ScatolaWindowShowHiddenFilesMode
scatola_window_info_get_hidden_files_mode (ScatolaWindowInfo *window)
{
    g_return_val_if_fail (SCATOLA_IS_WINDOW_INFO (window), SCATOLA_WINDOW_SHOW_HIDDEN_FILES_DEFAULT);

    return (* SCATOLA_WINDOW_INFO_GET_IFACE (window)->get_hidden_files_mode) (window);
}

void
scatola_window_info_set_hidden_files_mode (ScatolaWindowInfo *window,
                                        ScatolaWindowShowHiddenFilesMode  mode)
{
    g_return_if_fail (SCATOLA_IS_WINDOW_INFO (window));

    (* SCATOLA_WINDOW_INFO_GET_IFACE (window)->set_hidden_files_mode) (window,
            mode);
}

GtkUIManager *
scatola_window_info_get_ui_manager (ScatolaWindowInfo *window)
{
    g_return_val_if_fail (SCATOLA_IS_WINDOW_INFO (window), NULL);

    return (* SCATOLA_WINDOW_INFO_GET_IFACE (window)->get_ui_manager) (window);
}

ScatolaWindowSlotInfo *
scatola_window_info_get_active_slot (ScatolaWindowInfo *window)
{
    g_return_val_if_fail (SCATOLA_IS_WINDOW_INFO (window), NULL);

    return (* SCATOLA_WINDOW_INFO_GET_IFACE (window)->get_active_slot) (window);
}

ScatolaWindowSlotInfo *
scatola_window_info_get_extra_slot (ScatolaWindowInfo *window)
{
    g_return_val_if_fail (SCATOLA_IS_WINDOW_INFO (window), NULL);

    return (* SCATOLA_WINDOW_INFO_GET_IFACE (window)->get_extra_slot) (window);
}

gboolean
scatola_window_info_get_initiated_unmount (ScatolaWindowInfo *window)
{
    g_return_val_if_fail (SCATOLA_IS_WINDOW_INFO (window), FALSE);

    return (* SCATOLA_WINDOW_INFO_GET_IFACE (window)->get_initiated_unmount) (window);
}

void
scatola_window_info_set_initiated_unmount (ScatolaWindowInfo *window, gboolean initiated_unmount)
{
    g_return_if_fail (SCATOLA_IS_WINDOW_INFO (window));

    (* SCATOLA_WINDOW_INFO_GET_IFACE (window)->set_initiated_unmount) (window,
            initiated_unmount);

}
