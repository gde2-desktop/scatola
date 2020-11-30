/*
 *  scatola-menu-provider.h - Interface for Scatola extensions that
 *                             provide context menu items.
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
 * add context menu entries to files.  Extensions are called when
 * Scatola constructs the context menu for a file.  They are passed a
 * list of ScatolaFileInfo objects which holds the current selection */

#ifndef SCATOLA_MENU_PROVIDER_H
#define SCATOLA_MENU_PROVIDER_H

#include <glib-object.h>
#include <gtk/gtk.h>
#include "scatola-extension-types.h"
#include "scatola-file-info.h"
#include "scatola-menu.h"

G_BEGIN_DECLS

#define SCATOLA_TYPE_MENU_PROVIDER           (scatola_menu_provider_get_type ())
#define SCATOLA_MENU_PROVIDER(obj)           (G_TYPE_CHECK_INSTANCE_CAST ((obj), SCATOLA_TYPE_MENU_PROVIDER, ScatolaMenuProvider))
#define SCATOLA_IS_MENU_PROVIDER(obj)        (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SCATOLA_TYPE_MENU_PROVIDER))
#define SCATOLA_MENU_PROVIDER_GET_IFACE(obj) (G_TYPE_INSTANCE_GET_INTERFACE ((obj), SCATOLA_TYPE_MENU_PROVIDER, ScatolaMenuProviderIface))

typedef struct _ScatolaMenuProvider       ScatolaMenuProvider;
typedef struct _ScatolaMenuProviderIface  ScatolaMenuProviderIface;

/**
 * ScatolaMenuProviderIface:
 * @g_iface: The parent interface.
 * @get_file_items: Returns a #GList of #ScatolaMenuItem.
 *   See scatola_menu_provider_get_file_items() for details.
 * @get_background_items: Returns a #GList of #ScatolaMenuItem.
 *   See scatola_menu_provider_get_background_items() for details.
 * @get_toolbar_items: Returns a #GList of #ScatolaMenuItem.
 *   See scatola_menu_provider_get_toolbar_items() for details.
 *
 * Interface for extensions to provide additional menu items.
 */

struct _ScatolaMenuProviderIface {
    GTypeInterface g_iface;

    GList *(*get_file_items)       (ScatolaMenuProvider *provider,
                                    GtkWidget        *window,
                                    GList            *files);
    GList *(*get_background_items) (ScatolaMenuProvider *provider,
                                    GtkWidget        *window,
                                    ScatolaFileInfo     *current_folder);
    GList *(*get_toolbar_items)    (ScatolaMenuProvider *provider,
                                    GtkWidget        *window,
                                    ScatolaFileInfo     *current_folder);
};

/* Interface Functions */
GType  scatola_menu_provider_get_type             (void);
GList *scatola_menu_provider_get_file_items       (ScatolaMenuProvider *provider,
                                                GtkWidget        *window,
                                                GList            *files);
GList *scatola_menu_provider_get_background_items (ScatolaMenuProvider *provider,
                                                GtkWidget        *window,
                                                ScatolaFileInfo     *current_folder);
GList *scatola_menu_provider_get_toolbar_items    (ScatolaMenuProvider *provider,
                                                GtkWidget        *window,
                                                ScatolaFileInfo     *current_folder);

/* This function emit a signal to inform scatola that its item list has changed. */
void   scatola_menu_provider_emit_items_updated_signal (ScatolaMenuProvider *provider);

G_END_DECLS

#endif
