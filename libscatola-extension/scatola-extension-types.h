/*
 *  scatola-info-provider.h - Type definitions for Scatola extensions
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
 *  Author: Dave Camp <dave@ximian.com>
 *
 */

/* This interface is implemented by Scatola extensions that want to
 * provide information about files.  Extensions are called when Scatola
 * needs information about a file.  They are passed a ScatolaFileInfo
 * object which should be filled with relevant information */

#ifndef SCATOLA_EXTENSION_TYPES_H
#define SCATOLA_EXTENSION_TYPES_H

#include <glib-object.h>

G_BEGIN_DECLS

#define SCATOLA_TYPE_OPERATION_RESULT (scatola_operation_result_get_type ())

/**
 * ScatolaOperationHandle:
 *
 * Handle for asynchronous interfaces. These are opaque handles that must
 * be unique within an extension object. These are returned by operations
 * that return SCATOLA_OPERATION_IN_PROGRESS.
 */
typedef struct _ScatolaOperationHandle ScatolaOperationHandle;

/**
 * ScatolaOperationResult:
 * @SCATOLA_OPERATION_COMPLETE: the operation succeeded, and the extension
 *  is done with the request.
 * @SCATOLA_OPERATION_FAILED: the operation failed.
 * @SCATOLA_OPERATION_IN_PROGRESS: the extension has begin an async operation.
 *  When this value is returned, the extension must set the handle parameter
 *  and call the callback closure when the operation is complete.
 */
typedef enum {
    /* Returned if the call succeeded, and the extension is done
     * with the request */
    SCATOLA_OPERATION_COMPLETE,

    /* Returned if the call failed */
    SCATOLA_OPERATION_FAILED,

    /* Returned if the extension has begun an async operation.
     * If this is returned, the extension must set the handle
     * parameter and call the callback closure when the
     * operation is complete. */
    SCATOLA_OPERATION_IN_PROGRESS
} ScatolaOperationResult;

GType scatola_operation_result_get_type (void);

/**
 * SECTION:scatola-extension-types
 * @title: ScatolaModule
 * @short_description: Initialize an extension
 * @include: libscatola-extension/scatola-extension-types.h
 *
 * Methods that each extension implements.
 */

void scatola_module_initialize  (GTypeModule  *module);
void scatola_module_shutdown    (void);
void scatola_module_list_types  (const GType **types,
                              int          *num_types);
void scatola_module_list_pyfiles (GList      **pyfiles);

G_END_DECLS

#endif
