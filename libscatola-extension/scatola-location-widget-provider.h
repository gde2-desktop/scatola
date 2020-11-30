/*
 *  scatola-info-provider.h - Interface for Scatola extensions that
 *                             provide info about files.
 *
 *  Copyright (C) 2003 Novell, Inc.
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
 *  Author:  Dave Camp <dave@ximian.com>
 *           Alexander Larsson <alexl@redhat.com>
 *
 */

/* This interface is implemented by Scatola extensions that want to
 * provide extra location widgets for a particular location.
 * Extensions are called when Scatola displays a location.
 */

#ifndef SCATOLA_LOCATION_WIDGET_PROVIDER_H
#define SCATOLA_LOCATION_WIDGET_PROVIDER_H

#include <glib-object.h>
#include <gtk/gtk.h>
#include "scatola-extension-types.h"

G_BEGIN_DECLS

#define SCATOLA_TYPE_LOCATION_WIDGET_PROVIDER           (scatola_location_widget_provider_get_type ())
#define SCATOLA_LOCATION_WIDGET_PROVIDER(obj)           (G_TYPE_CHECK_INSTANCE_CAST ((obj), SCATOLA_TYPE_LOCATION_WIDGET_PROVIDER, ScatolaLocationWidgetProvider))
#define SCATOLA_IS_LOCATION_WIDGET_PROVIDER(obj)        (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SCATOLA_TYPE_LOCATION_WIDGET_PROVIDER))
#define SCATOLA_LOCATION_WIDGET_PROVIDER_GET_IFACE(obj) (G_TYPE_INSTANCE_GET_INTERFACE ((obj), SCATOLA_TYPE_LOCATION_WIDGET_PROVIDER, ScatolaLocationWidgetProviderIface))

typedef struct _ScatolaLocationWidgetProvider       ScatolaLocationWidgetProvider;
typedef struct _ScatolaLocationWidgetProviderIface  ScatolaLocationWidgetProviderIface;

/**
 * ScatolaLocationWidgetProviderIface:
 * @g_iface: The parent interface.
 * @get_widget: Returns a #GtkWidget.
 *   See scatola_location_widget_provider_get_widget() for details.
 *
 * Interface for extensions to provide additional location widgets.
 */
struct _ScatolaLocationWidgetProviderIface {
    GTypeInterface g_iface;

    GtkWidget *(*get_widget) (ScatolaLocationWidgetProvider *provider,
                              const char                 *uri,
                              GtkWidget                  *window);
};

/* Interface Functions */
GType      scatola_location_widget_provider_get_type   (void);
GtkWidget *scatola_location_widget_provider_get_widget (ScatolaLocationWidgetProvider *provider,
                                                     const char                 *uri,
                                                     GtkWidget                  *window);
G_END_DECLS

#endif
