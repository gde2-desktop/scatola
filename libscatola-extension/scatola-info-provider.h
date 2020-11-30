/*
 *  scatola-info-provider.h - Interface for Scatola extensions that
 *                             provide info about files.
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
 * provide information about files.  Extensions are called when Scatola
 * needs information about a file.  They are passed a ScatolaFileInfo
 * object which should be filled with relevant information */

#ifndef SCATOLA_INFO_PROVIDER_H
#define SCATOLA_INFO_PROVIDER_H

#include <glib-object.h>
#include "scatola-extension-types.h"
#include "scatola-file-info.h"

G_BEGIN_DECLS

#define SCATOLA_TYPE_INFO_PROVIDER           (scatola_info_provider_get_type ())
#define SCATOLA_INFO_PROVIDER(obj)           (G_TYPE_CHECK_INSTANCE_CAST ((obj), SCATOLA_TYPE_INFO_PROVIDER, ScatolaInfoProvider))
#define SCATOLA_IS_INFO_PROVIDER(obj)        (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SCATOLA_TYPE_INFO_PROVIDER))
#define SCATOLA_INFO_PROVIDER_GET_IFACE(obj) (G_TYPE_INSTANCE_GET_INTERFACE ((obj), SCATOLA_TYPE_INFO_PROVIDER, ScatolaInfoProviderIface))

typedef struct _ScatolaInfoProvider       ScatolaInfoProvider;
typedef struct _ScatolaInfoProviderIface  ScatolaInfoProviderIface;

typedef void (*ScatolaInfoProviderUpdateComplete) (ScatolaInfoProvider    *provider,
                                                ScatolaOperationHandle *handle,
                                                ScatolaOperationResult  result,
                                                gpointer             user_data);

/**
 * ScatolaInfoProviderIface:
 * @g_iface: The parent interface.
 * @update_file_info: Returns a #ScatolaOperationResult.
 *   See scatola_info_provider_update_file_info() for details.
 * @cancel_update: Cancels a previous call to scatola_info_provider_update_file_info().
 *   See scatola_info_provider_cancel_update() for details.
 *
 * Interface for extensions to provide additional information about files.
 */

struct _ScatolaInfoProviderIface {
    GTypeInterface g_iface;

    ScatolaOperationResult (*update_file_info) (ScatolaInfoProvider     *provider,
                                             ScatolaFileInfo         *file,
                                             GClosure             *update_complete,
                                             ScatolaOperationHandle **handle);
    void                (*cancel_update)    (ScatolaInfoProvider     *provider,
                                             ScatolaOperationHandle  *handle);
};

/* Interface Functions */
GType               scatola_info_provider_get_type               (void);
ScatolaOperationResult scatola_info_provider_update_file_info       (ScatolaInfoProvider     *provider,
                                                               ScatolaFileInfo         *file,
                                                               GClosure             *update_complete,
                                                               ScatolaOperationHandle **handle);
void                scatola_info_provider_cancel_update          (ScatolaInfoProvider     *provider,
                                                               ScatolaOperationHandle  *handle);



/* Helper functions for implementations */
void                scatola_info_provider_update_complete_invoke (GClosure             *update_complete,
                                                               ScatolaInfoProvider     *provider,
                                                               ScatolaOperationHandle  *handle,
                                                               ScatolaOperationResult   result);

G_END_DECLS

#endif
