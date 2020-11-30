/*
 *  scatola-property-page-provider.h - Interface for Scatola extensions
 *                                      that provide property pages.
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

/* This interface is implemented by Scatola extensions that want to
 * add property page to property dialogs.  Extensions are called when
 * Scatola needs property pages for a selection.  They are passed a
 * list of ScatolaFileInfo objects for which information should
 * be displayed  */

#ifndef SCATOLA_PROPERTY_PAGE_PROVIDER_H
#define SCATOLA_PROPERTY_PAGE_PROVIDER_H

#include <glib-object.h>
#include "scatola-extension-types.h"
#include "scatola-file-info.h"
#include "scatola-property-page.h"

G_BEGIN_DECLS

#define SCATOLA_TYPE_PROPERTY_PAGE_PROVIDER           (scatola_property_page_provider_get_type ())
#define SCATOLA_PROPERTY_PAGE_PROVIDER(obj)           (G_TYPE_CHECK_INSTANCE_CAST ((obj), SCATOLA_TYPE_PROPERTY_PAGE_PROVIDER, ScatolaPropertyPageProvider))
#define SCATOLA_IS_PROPERTY_PAGE_PROVIDER(obj)        (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SCATOLA_TYPE_PROPERTY_PAGE_PROVIDER))
#define SCATOLA_PROPERTY_PAGE_PROVIDER_GET_IFACE(obj) (G_TYPE_INSTANCE_GET_INTERFACE ((obj), SCATOLA_TYPE_PROPERTY_PAGE_PROVIDER, ScatolaPropertyPageProviderIface))

typedef struct _ScatolaPropertyPageProvider       ScatolaPropertyPageProvider;
typedef struct _ScatolaPropertyPageProviderIface  ScatolaPropertyPageProviderIface;

/**
 * ScatolaPropertyPageProviderIface:
 * @g_iface: The parent interface.
 * @get_pages: Returns a #GList of #ScatolaPropertyPage.
 *   See scatola_property_page_provider_get_pages() for details.
 *
 * Interface for extensions to provide additional property pages.
 */

struct _ScatolaPropertyPageProviderIface {
    GTypeInterface g_iface;

    GList *(*get_pages) (ScatolaPropertyPageProvider *provider,
                         GList                    *files);
};

/* Interface Functions */
GType  scatola_property_page_provider_get_type  (void);
GList *scatola_property_page_provider_get_pages (ScatolaPropertyPageProvider *provider,
                                              GList                    *files);

G_END_DECLS

#endif
