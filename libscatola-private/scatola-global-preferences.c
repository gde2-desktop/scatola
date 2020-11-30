/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/* scatola-global-preferences.c - Scatola specific preference keys and
                                   functions.

   Copyright (C) 1999, 2000, 2001 Eazel, Inc.

   The Gde2 Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   The Gde2 Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with the Gde2 Library; see the file COPYING.LIB.  If not,
   write to the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
   Boston, MA 02110-1301, USA.

   Authors: Ramiro Estrugo <ramiro@eazel.com>
*/

#include <config.h>
#include "scatola-global-preferences.h"

#include "scatola-file-utilities.h"
#include "scatola-file.h"
#include <eel/eel-glib-extensions.h>
#include <eel/eel-gtk-extensions.h>
#include <eel/eel-stock-dialogs.h>
#include <eel/eel-string.h>
#include <glib/gi18n.h>

/*
 * Public functions
 */
char *
scatola_global_preferences_get_default_folder_viewer_preference_as_iid (void)
{
    int preference_value;
    const char *viewer_iid;

    preference_value =
        g_settings_get_enum (scatola_preferences, SCATOLA_PREFERENCES_DEFAULT_FOLDER_VIEWER);

    if (preference_value == SCATOLA_DEFAULT_FOLDER_VIEWER_LIST_VIEW)
    {
        viewer_iid = SCATOLA_LIST_VIEW_IID;
    }
    else if (preference_value == SCATOLA_DEFAULT_FOLDER_VIEWER_COMPACT_VIEW)
    {
        viewer_iid = SCATOLA_COMPACT_VIEW_IID;
    }
    else
    {
        viewer_iid = SCATOLA_ICON_VIEW_IID;
    }

    return g_strdup (viewer_iid);
}

void
scatola_global_preferences_init (void)
{
    static gboolean initialized = FALSE;
    int i;

    if (initialized)
    {
        return;
    }

    initialized = TRUE;
    
    scatola_preferences = g_settings_new("org.gde2.scatola.preferences");
    scatola_media_preferences = g_settings_new("org.gde2.media-handling");
    scatola_window_state = g_settings_new("org.gde2.scatola.window-state");
    scatola_icon_view_preferences = g_settings_new("org.gde2.scatola.icon-view");
    scatola_compact_view_preferences = g_settings_new("org.gde2.scatola.compact-view");
    scatola_desktop_preferences = g_settings_new("org.gde2.scatola.desktop");
    scatola_tree_sidebar_preferences = g_settings_new("org.gde2.scatola.sidebar-panels.tree");
    scatola_list_view_preferences = g_settings_new("org.gde2.scatola.list-view");
    scatola_extension_preferences = g_settings_new("org.gde2.scatola.extensions");

    gde2_background_preferences = g_settings_new("org.gde2.background");
    gde2_lockdown_preferences = g_settings_new("org.gde2.lockdown");
}
