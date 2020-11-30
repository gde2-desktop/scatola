/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*-*/

/* scatola-metadata.c - metadata utils
 *
 * Copyright (C) 2009 Red Hatl, Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#include <config.h>
#include "scatola-metadata.h"
#include <glib.h>

static char *used_metadata_names[] =
{
    SCATOLA_METADATA_KEY_DEFAULT_VIEW,
    SCATOLA_METADATA_KEY_LOCATION_BACKGROUND_COLOR,
    SCATOLA_METADATA_KEY_LOCATION_BACKGROUND_IMAGE,
    SCATOLA_METADATA_KEY_ICON_VIEW_ZOOM_LEVEL,
    SCATOLA_METADATA_KEY_ICON_VIEW_AUTO_LAYOUT,
    SCATOLA_METADATA_KEY_ICON_VIEW_TIGHTER_LAYOUT,
    SCATOLA_METADATA_KEY_ICON_VIEW_SORT_BY,
    SCATOLA_METADATA_KEY_ICON_VIEW_SORT_REVERSED,
    SCATOLA_METADATA_KEY_ICON_VIEW_KEEP_ALIGNED,
    SCATOLA_METADATA_KEY_ICON_VIEW_LAYOUT_TIMESTAMP,
    SCATOLA_METADATA_KEY_LIST_VIEW_ZOOM_LEVEL,
    SCATOLA_METADATA_KEY_LIST_VIEW_SORT_COLUMN,
    SCATOLA_METADATA_KEY_LIST_VIEW_SORT_REVERSED,
    SCATOLA_METADATA_KEY_LIST_VIEW_VISIBLE_COLUMNS,
    SCATOLA_METADATA_KEY_LIST_VIEW_COLUMN_ORDER,
    SCATOLA_METADATA_KEY_COMPACT_VIEW_ZOOM_LEVEL,
    SCATOLA_METADATA_KEY_WINDOW_GEOMETRY,
    SCATOLA_METADATA_KEY_WINDOW_SCROLL_POSITION,
    SCATOLA_METADATA_KEY_WINDOW_SHOW_HIDDEN_FILES,
    SCATOLA_METADATA_KEY_WINDOW_MAXIMIZED,
    SCATOLA_METADATA_KEY_WINDOW_STICKY,
    SCATOLA_METADATA_KEY_WINDOW_KEEP_ABOVE,
    SCATOLA_METADATA_KEY_SIDEBAR_BACKGROUND_COLOR,
    SCATOLA_METADATA_KEY_SIDEBAR_BACKGROUND_IMAGE,
    SCATOLA_METADATA_KEY_SIDEBAR_BUTTONS,
    SCATOLA_METADATA_KEY_ANNOTATION,
    SCATOLA_METADATA_KEY_ICON_POSITION,
    SCATOLA_METADATA_KEY_ICON_POSITION_TIMESTAMP,
    SCATOLA_METADATA_KEY_ICON_SCALE,
    SCATOLA_METADATA_KEY_CUSTOM_ICON,
    SCATOLA_METADATA_KEY_SCREEN,
    SCATOLA_METADATA_KEY_EMBLEMS,
    NULL
};

guint
scatola_metadata_get_id (const char *metadata)
{
    static GHashTable *hash;
    int i;

    if (hash == NULL)
    {
        hash = g_hash_table_new (g_str_hash, g_str_equal);
        for (i = 0; used_metadata_names[i] != NULL; i++)
            g_hash_table_insert (hash,
                                 used_metadata_names[i],
                                 GINT_TO_POINTER (i + 1));
    }

    return GPOINTER_TO_INT (g_hash_table_lookup (hash, metadata));
}
