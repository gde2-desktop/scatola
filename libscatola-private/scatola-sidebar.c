/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*-

   scatola-sidebar.c: Interface for scatola sidebar plugins

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
#include "scatola-sidebar.h"

enum
{
    TAB_ICON_CHANGED,
    ZOOM_PARAMETERS_CHANGED,
    ZOOM_LEVEL_CHANGED,
    LAST_SIGNAL
};

static guint scatola_sidebar_signals[LAST_SIGNAL] = { 0 };

static void
scatola_sidebar_base_init (gpointer g_class)
{
    static gboolean initialized = FALSE;

    if (! initialized)
    {
        scatola_sidebar_signals[TAB_ICON_CHANGED] =
            g_signal_new ("tab_icon_changed",
                          SCATOLA_TYPE_SIDEBAR,
                          G_SIGNAL_RUN_LAST,
                          G_STRUCT_OFFSET (ScatolaSidebarIface, tab_icon_changed),
                          NULL, NULL,
                          g_cclosure_marshal_VOID__VOID,
                          G_TYPE_NONE, 0);

        initialized = TRUE;
    }
}

GType
scatola_sidebar_get_type (void)
{
    static GType type = 0;

    if (!type)
    {
        const GTypeInfo info =
        {
            sizeof (ScatolaSidebarIface),
            scatola_sidebar_base_init,
            NULL,
            NULL,
            NULL,
            NULL,
            0,
            0,
            NULL
        };

        type = g_type_register_static (G_TYPE_INTERFACE,
                                       "ScatolaSidebar",
                                       &info, 0);
        g_type_interface_add_prerequisite (type, GTK_TYPE_WIDGET);
    }

    return type;
}


const char *
scatola_sidebar_get_sidebar_id (ScatolaSidebar *sidebar)
{
    g_return_val_if_fail (SCATOLA_IS_SIDEBAR (sidebar), NULL);

    return (* SCATOLA_SIDEBAR_GET_IFACE (sidebar)->get_sidebar_id) (sidebar);
}

char *
scatola_sidebar_get_tab_label (ScatolaSidebar *sidebar)
{
    g_return_val_if_fail (SCATOLA_IS_SIDEBAR (sidebar), NULL);

    return (* SCATOLA_SIDEBAR_GET_IFACE (sidebar)->get_tab_label) (sidebar);
}

char *
scatola_sidebar_get_tab_tooltip (ScatolaSidebar *sidebar)
{
    g_return_val_if_fail (SCATOLA_IS_SIDEBAR (sidebar), NULL);

    return (* SCATOLA_SIDEBAR_GET_IFACE (sidebar)->get_tab_tooltip) (sidebar);
}

GdkPixbuf *
scatola_sidebar_get_tab_icon (ScatolaSidebar *sidebar)
{
    g_return_val_if_fail (SCATOLA_IS_SIDEBAR (sidebar), NULL);

    return (* SCATOLA_SIDEBAR_GET_IFACE (sidebar)->get_tab_icon) (sidebar);
}

void
scatola_sidebar_is_visible_changed (ScatolaSidebar *sidebar,
                                 gboolean         is_visible)
{
    g_return_if_fail (SCATOLA_IS_SIDEBAR (sidebar));

    (* SCATOLA_SIDEBAR_GET_IFACE (sidebar)->is_visible_changed) (sidebar,
            is_visible);
}
