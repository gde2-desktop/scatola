/*
 *  scatola-property-page-provider.c - Interface for Scatola extensions
 *                                      that provide property pages for
 *                                      files.
 *
 *  Copyright (C) 2003 Novell, Inc.
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
 *
 */

#include <config.h>
#include "scatola-property-page-provider.h"

#include <glib-object.h>

/**
 * SECTION:scatola-property-page-provider
 * @title: ScatolaPropertyPageProvider
 * @short_description: Interface to provide additional property pages
 * @include: libscatola-extension/scatola-property-page-provider.h
 *
 * #ScatolaPropertyPageProvider allows extension to provide additional pages
 * for the file properties dialog.
 */

static void
scatola_property_page_provider_base_init (gpointer g_class)
{
}

GType
scatola_property_page_provider_get_type (void)
{
    static GType type = 0;

    if (!type) {
        const GTypeInfo info = {
            sizeof (ScatolaPropertyPageProviderIface),
            scatola_property_page_provider_base_init,
            NULL,
            NULL,
            NULL,
            NULL,
            0,
            0,
            NULL
        };

        type = g_type_register_static (G_TYPE_INTERFACE,
                                       "ScatolaPropertyPageProvider",
                                       &info, 0);
        g_type_interface_add_prerequisite (type, G_TYPE_OBJECT);
    }

    return type;
}

/**
 * scatola_property_page_provider_get_pages:
 * @provider: a #ScatolaPropertyPageProvider
 * @files: (element-type ScatolaFileInfo): a #GList of #ScatolaFileInfo
 *
 * This function is called by Scatola when it wants property page
 * items from the extension.
 *
 * This function is called in the main thread before a property page
 * is shown, so it should return quickly.
 *
 * Returns: (element-type ScatolaPropertyPage) (transfer full): A #GList of allocated #ScatolaPropertyPage items.
 */
GList *
scatola_property_page_provider_get_pages (ScatolaPropertyPageProvider *provider,
                                       GList *files)
{
    g_return_val_if_fail (SCATOLA_IS_PROPERTY_PAGE_PROVIDER (provider), NULL);
    g_return_val_if_fail (SCATOLA_PROPERTY_PAGE_PROVIDER_GET_IFACE (provider)->get_pages != NULL, NULL);

    return SCATOLA_PROPERTY_PAGE_PROVIDER_GET_IFACE (provider)->get_pages
           (provider, files);
}


