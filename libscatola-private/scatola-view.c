/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*-

   scatola-view.c: Interface for scatola views

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
#include "scatola-view.h"

enum
{
    TITLE_CHANGED,
    ZOOM_LEVEL_CHANGED,
    LAST_SIGNAL
};

static guint scatola_view_signals[LAST_SIGNAL] = { 0 };

static void
scatola_view_base_init (gpointer g_class)
{
    static gboolean initialized = FALSE;

    if (! initialized)
    {
        scatola_view_signals[TITLE_CHANGED] =
            g_signal_new ("title_changed",
                          SCATOLA_TYPE_VIEW,
                          G_SIGNAL_RUN_LAST,
                          G_STRUCT_OFFSET (ScatolaViewIface, title_changed),
                          NULL, NULL,
                          g_cclosure_marshal_VOID__VOID,
                          G_TYPE_NONE, 0);

        scatola_view_signals[ZOOM_LEVEL_CHANGED] =
            g_signal_new ("zoom_level_changed",
                          SCATOLA_TYPE_VIEW,
                          G_SIGNAL_RUN_LAST,
                          G_STRUCT_OFFSET (ScatolaViewIface, zoom_level_changed),
                          NULL, NULL,
                          g_cclosure_marshal_VOID__VOID,
                          G_TYPE_NONE, 0);

        initialized = TRUE;
    }
}

GType
scatola_view_get_type (void)
{
    static GType type = 0;

    if (!type)
    {
        const GTypeInfo info =
        {
            sizeof (ScatolaViewIface),
            scatola_view_base_init,
            NULL,
            NULL,
            NULL,
            NULL,
            0,
            0,
            NULL
        };

        type = g_type_register_static (G_TYPE_INTERFACE,
                                       "ScatolaView",
                                       &info, 0);
        g_type_interface_add_prerequisite (type, G_TYPE_OBJECT);
    }

    return type;
}

const char *
scatola_view_get_view_id (ScatolaView *view)
{
    g_return_val_if_fail (SCATOLA_IS_VIEW (view), NULL);

    return (* SCATOLA_VIEW_GET_IFACE (view)->get_view_id) (view);
}

GtkWidget *
scatola_view_get_widget (ScatolaView *view)
{
    g_return_val_if_fail (SCATOLA_IS_VIEW (view), NULL);

    return (* SCATOLA_VIEW_GET_IFACE (view)->get_widget) (view);
}

void
scatola_view_load_location (ScatolaView *view,
                         const char   *location_uri)
{
    g_return_if_fail (SCATOLA_IS_VIEW (view));
    g_return_if_fail (location_uri != NULL);

    (* SCATOLA_VIEW_GET_IFACE (view)->load_location) (view,
            location_uri);
}

void
scatola_view_stop_loading (ScatolaView *view)
{
    g_return_if_fail (SCATOLA_IS_VIEW (view));

    (* SCATOLA_VIEW_GET_IFACE (view)->stop_loading) (view);
}

int
scatola_view_get_selection_count (ScatolaView *view)
{
    g_return_val_if_fail (SCATOLA_IS_VIEW (view), 0);

    return (* SCATOLA_VIEW_GET_IFACE (view)->get_selection_count) (view);
}

GList *
scatola_view_get_selection (ScatolaView *view)
{
    g_return_val_if_fail (SCATOLA_IS_VIEW (view), NULL);

    return (* SCATOLA_VIEW_GET_IFACE (view)->get_selection) (view);
}

void
scatola_view_set_selection (ScatolaView *view,
                         GList        *list)
{
    g_return_if_fail (SCATOLA_IS_VIEW (view));

    (* SCATOLA_VIEW_GET_IFACE (view)->set_selection) (view,
            list);
}

void
scatola_view_set_is_active (ScatolaView *view,
                         gboolean is_active)
{
    g_return_if_fail (SCATOLA_IS_VIEW (view));

    (* SCATOLA_VIEW_GET_IFACE (view)->set_is_active) (view,
            is_active);
}

void
scatola_view_invert_selection (ScatolaView *view)
{
    g_return_if_fail (SCATOLA_IS_VIEW (view));

    (* SCATOLA_VIEW_GET_IFACE (view)->invert_selection) (view);
}

char *
scatola_view_get_first_visible_file (ScatolaView *view)
{
    g_return_val_if_fail (SCATOLA_IS_VIEW (view), NULL);

    return (* SCATOLA_VIEW_GET_IFACE (view)->get_first_visible_file) (view);
}

void
scatola_view_scroll_to_file (ScatolaView *view,
                          const char   *uri)
{
    g_return_if_fail (SCATOLA_IS_VIEW (view));

    (* SCATOLA_VIEW_GET_IFACE (view)->scroll_to_file) (view, uri);
}

char *
scatola_view_get_title (ScatolaView *view)
{
    g_return_val_if_fail (SCATOLA_IS_VIEW (view), NULL);

    if (SCATOLA_VIEW_GET_IFACE (view)->get_title != NULL)
    {
        return (* SCATOLA_VIEW_GET_IFACE (view)->get_title) (view);
    }
    else
    {
        return NULL;
    }
}


gboolean
scatola_view_supports_zooming (ScatolaView *view)
{
    g_return_val_if_fail (SCATOLA_IS_VIEW (view), FALSE);

    return (* SCATOLA_VIEW_GET_IFACE (view)->supports_zooming) (view);
}

void
scatola_view_bump_zoom_level (ScatolaView *view,
                           int zoom_increment)
{
    g_return_if_fail (SCATOLA_IS_VIEW (view));

    (* SCATOLA_VIEW_GET_IFACE (view)->bump_zoom_level) (view,
            zoom_increment);
}

void
scatola_view_zoom_to_level (ScatolaView      *view,
                         ScatolaZoomLevel  level)
{
    g_return_if_fail (SCATOLA_IS_VIEW (view));

    (* SCATOLA_VIEW_GET_IFACE (view)->zoom_to_level) (view,
            level);
}

void
scatola_view_restore_default_zoom_level (ScatolaView *view)
{
    g_return_if_fail (SCATOLA_IS_VIEW (view));

    (* SCATOLA_VIEW_GET_IFACE (view)->restore_default_zoom_level) (view);
}

gboolean
scatola_view_can_zoom_in (ScatolaView *view)
{
    g_return_val_if_fail (SCATOLA_IS_VIEW (view), FALSE);

    return (* SCATOLA_VIEW_GET_IFACE (view)->can_zoom_in) (view);
}

gboolean
scatola_view_can_zoom_out (ScatolaView *view)
{
    g_return_val_if_fail (SCATOLA_IS_VIEW (view), FALSE);

    return (* SCATOLA_VIEW_GET_IFACE (view)->can_zoom_out) (view);
}

ScatolaZoomLevel
scatola_view_get_zoom_level (ScatolaView *view)
{
    g_return_val_if_fail (SCATOLA_IS_VIEW (view), SCATOLA_ZOOM_LEVEL_STANDARD);

    return (* SCATOLA_VIEW_GET_IFACE (view)->get_zoom_level) (view);
}

void
scatola_view_grab_focus (ScatolaView   *view)
{
    g_return_if_fail (SCATOLA_IS_VIEW (view));

    if (SCATOLA_VIEW_GET_IFACE (view)->grab_focus != NULL)
    {
        (* SCATOLA_VIEW_GET_IFACE (view)->grab_focus) (view);
    }
}

void
scatola_view_update_menus (ScatolaView *view)
{
    g_return_if_fail (SCATOLA_IS_VIEW (view));

    if (SCATOLA_VIEW_GET_IFACE (view)->update_menus != NULL)
    {
        (* SCATOLA_VIEW_GET_IFACE (view)->update_menus) (view);
    }
}

void
scatola_view_pop_up_location_context_menu (ScatolaView   *view,
                                        GdkEventButton *event,
                                        const char     *location)
{
    g_return_if_fail (SCATOLA_IS_VIEW (view));

    if (SCATOLA_VIEW_GET_IFACE (view)->pop_up_location_context_menu != NULL)
    {
        (* SCATOLA_VIEW_GET_IFACE (view)->pop_up_location_context_menu) (view, event, location);
    }
}

void
scatola_view_drop_proxy_received_uris   (ScatolaView         *view,
                                      GList                *uris,
                                      const char           *target_location,
                                      GdkDragAction         action)
{
    g_return_if_fail (SCATOLA_IS_VIEW (view));

    if (SCATOLA_VIEW_GET_IFACE (view)->drop_proxy_received_uris != NULL)
    {
        (* SCATOLA_VIEW_GET_IFACE (view)->drop_proxy_received_uris) (view, uris, target_location, action);
    }
}

void
scatola_view_drop_proxy_received_netscape_url (ScatolaView         *view,
        const char           *source_url,
        const char           *target_location,
        GdkDragAction         action)
{
    g_return_if_fail (SCATOLA_IS_VIEW (view));

    if (SCATOLA_VIEW_GET_IFACE (view)->drop_proxy_received_netscape_url != NULL)
    {
        (* SCATOLA_VIEW_GET_IFACE (view)->drop_proxy_received_netscape_url) (view, source_url, target_location, action);
    }
}


