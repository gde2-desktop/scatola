/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*-

   scatola-sidebar-provider.h: register and create ScatolaSidebars

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

#ifndef SCATOLA_SIDEBAR_PROVIDER_H
#define SCATOLA_SIDEBAR_PROVIDER_H

#include <libscatola-private/scatola-sidebar.h>
#include <libscatola-private/scatola-window-info.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SCATOLA_TYPE_SIDEBAR_PROVIDER           (scatola_sidebar_provider_get_type ())
#define SCATOLA_SIDEBAR_PROVIDER(obj)           (G_TYPE_CHECK_INSTANCE_CAST ((obj), SCATOLA_TYPE_SIDEBAR_PROVIDER, ScatolaSidebarProvider))
#define SCATOLA_IS_SIDEBAR_PROVIDER(obj)        (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SCATOLA_TYPE_SIDEBAR_PROVIDER))
#define SCATOLA_SIDEBAR_PROVIDER_GET_IFACE(obj) (G_TYPE_INSTANCE_GET_INTERFACE ((obj), SCATOLA_TYPE_SIDEBAR_PROVIDER, ScatolaSidebarProviderIface))

    typedef struct _ScatolaSidebarProvider       ScatolaSidebarProvider;
    typedef struct _ScatolaSidebarProviderIface  ScatolaSidebarProviderIface;

    struct _ScatolaSidebarProviderIface
    {
        GTypeInterface g_iface;

        ScatolaSidebar * (*create) (ScatolaSidebarProvider *provider,
                                 ScatolaWindowInfo *window);
    };

    /* Interface Functions */
    GType                   scatola_sidebar_provider_get_type  (void);
    ScatolaSidebar *       scatola_sidebar_provider_create (ScatolaSidebarProvider *provider,
            ScatolaWindowInfo  *window);
    GList *                 scatola_list_sidebar_providers (void);

#ifdef __cplusplus
}
#endif

#endif /* SCATOLA_SIDEBAR_PROVIDER_H */
