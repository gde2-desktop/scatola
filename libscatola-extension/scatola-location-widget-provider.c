/*
 *  scatola-location-widget-provider.c - Interface for Scatola
                 extensions that provide extra widgets for a location
 *
 *  Copyright (C) 2005 Red Hat, Inc.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public
 *  License along with this library; if not, write to the Free
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 *  Author:  Alexander Larsson <alexl@redhat.com>
 *
 */

#include <config.h>
#include "scatola-location-widget-provider.h"

#include <glib-object.h>

/**
 * SECTION:scatola-location-widget-provider
 * @title: ScatolaLocationWidgetProvider
 * @short_description: Interface to provide additional location widgets
 * @include: libscatola-extension/scatola-location-widget-provider.h
 *
 * #ScatolaLocationWidgetProvider allows extension to provide additional location
 * widgets in the file manager views.
 */

static void
scatola_location_widget_provider_base_init (gpointer g_class)
{
}

GType
scatola_location_widget_provider_get_type (void)
{
    static GType type = 0;

    if (!type) {
        const GTypeInfo info = {
            sizeof (ScatolaLocationWidgetProviderIface),
            scatola_location_widget_provider_base_init,
            NULL,
            NULL,
            NULL,
            NULL,
            0,
            0,
            NULL
        };

        type = g_type_register_static (G_TYPE_INTERFACE,
                                       "ScatolaLocationWidgetProvider",
                                       &info, 0);
        g_type_interface_add_prerequisite (type, G_TYPE_OBJECT);
    }

    return type;
}

/**
 * scatola_location_widget_provider_get_widget:
 * @provider: a #ScatolaLocationWidgetProvider
 * @uri: the URI of the location
 * @window: parent #GtkWindow
 *
 * Returns: (transfer none): the location widget for @provider at @uri
 */
GtkWidget *
scatola_location_widget_provider_get_widget (ScatolaLocationWidgetProvider *provider,
                                          const char                 *uri,
                                          GtkWidget                  *window)
{
    g_return_val_if_fail (SCATOLA_IS_LOCATION_WIDGET_PROVIDER (provider), NULL);
    g_return_val_if_fail (SCATOLA_LOCATION_WIDGET_PROVIDER_GET_IFACE (provider)->get_widget != NULL, NULL);

    return SCATOLA_LOCATION_WIDGET_PROVIDER_GET_IFACE (provider)->get_widget
           (provider, uri, window);

}
