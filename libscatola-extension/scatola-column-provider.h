/*
 *  scatola-column-provider.h - Interface for Scatola extensions that
 *                               provide column descriptions.
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
 * add columns to the list view and details to the icon view.
 * Extensions are asked for a list of columns to display.  Each
 * returned column refers to a string attribute which can be filled in
 * by ScatolaInfoProvider */

#ifndef SCATOLA_COLUMN_PROVIDER_H
#define SCATOLA_COLUMN_PROVIDER_H

#include <glib-object.h>
#include "scatola-extension-types.h"
#include "scatola-column.h"

G_BEGIN_DECLS

#define SCATOLA_TYPE_COLUMN_PROVIDER           (scatola_column_provider_get_type ())
#define SCATOLA_COLUMN_PROVIDER(obj)           (G_TYPE_CHECK_INSTANCE_CAST ((obj), SCATOLA_TYPE_COLUMN_PROVIDER, ScatolaColumnProvider))
#define SCATOLA_IS_COLUMN_PROVIDER(obj)        (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SCATOLA_TYPE_COLUMN_PROVIDER))
#define SCATOLA_COLUMN_PROVIDER_GET_IFACE(obj) (G_TYPE_INSTANCE_GET_INTERFACE ((obj), SCATOLA_TYPE_COLUMN_PROVIDER, ScatolaColumnProviderIface))

typedef struct _ScatolaColumnProvider       ScatolaColumnProvider;
typedef struct _ScatolaColumnProviderIface  ScatolaColumnProviderIface;

/**
 * ScatolaColumnProviderIface:
 * @g_iface: The parent interface.
 * @get_columns: Returns a #GList of #ScatolaColumn.
 *   See scatola_column_provider_get_columns() for details.
 *
 * Interface for extensions to provide additional list view columns.
 */

struct _ScatolaColumnProviderIface {
    GTypeInterface g_iface;

    GList *(*get_columns) (ScatolaColumnProvider *provider);
};

/* Interface Functions */
GType  scatola_column_provider_get_type    (void);
GList *scatola_column_provider_get_columns (ScatolaColumnProvider *provider);

G_END_DECLS

#endif
