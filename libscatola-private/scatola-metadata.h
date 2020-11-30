/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*-

   scatola-metadata.h: #defines and other metadata-related info

   Copyright (C) 2000 Eazel, Inc.

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public
   License along with this program; if not, write to the
   Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
   Boston, MA 02110-1301, USA.

   Author: John Sullivan <sullivan@eazel.com>
*/

#ifndef SCATOLA_METADATA_H
#define SCATOLA_METADATA_H

/* Keys for getting/setting Scatola metadata. All metadata used in Scatola
 * should define its key here, so we can keep track of the whole set easily.
 * Any updates here needs to be added in scatola-metadata.c too.
 */

#include <glib.h>

/* Per-file */

#define SCATOLA_METADATA_KEY_DEFAULT_VIEW		 	"scatola-default-view"

#define SCATOLA_METADATA_KEY_LOCATION_BACKGROUND_COLOR 	"folder-background-color"
#define SCATOLA_METADATA_KEY_LOCATION_BACKGROUND_IMAGE 	"folder-background-image"

#define SCATOLA_METADATA_KEY_ICON_VIEW_ZOOM_LEVEL       	"scatola-icon-view-zoom-level"
#define SCATOLA_METADATA_KEY_ICON_VIEW_AUTO_LAYOUT      	"scatola-icon-view-auto-layout"
#define SCATOLA_METADATA_KEY_ICON_VIEW_TIGHTER_LAYOUT      	"scatola-icon-view-tighter-layout"
#define SCATOLA_METADATA_KEY_ICON_VIEW_SORT_BY          	"scatola-icon-view-sort-by"
#define SCATOLA_METADATA_KEY_ICON_VIEW_SORT_REVERSED    	"scatola-icon-view-sort-reversed"
#define SCATOLA_METADATA_KEY_ICON_VIEW_KEEP_ALIGNED            "scatola-icon-view-keep-aligned"
#define SCATOLA_METADATA_KEY_ICON_VIEW_LAYOUT_TIMESTAMP	"scatola-icon-view-layout-timestamp"

#define SCATOLA_METADATA_KEY_LIST_VIEW_ZOOM_LEVEL       	"scatola-list-view-zoom-level"
#define SCATOLA_METADATA_KEY_LIST_VIEW_SORT_COLUMN      	"scatola-list-view-sort-column"
#define SCATOLA_METADATA_KEY_LIST_VIEW_SORT_REVERSED    	"scatola-list-view-sort-reversed"
#define SCATOLA_METADATA_KEY_LIST_VIEW_VISIBLE_COLUMNS    	"scatola-list-view-visible-columns"
#define SCATOLA_METADATA_KEY_LIST_VIEW_COLUMN_ORDER    	"scatola-list-view-column-order"

#define SCATOLA_METADATA_KEY_COMPACT_VIEW_ZOOM_LEVEL		"scatola-compact-view-zoom-level"

#define SCATOLA_METADATA_KEY_WINDOW_GEOMETRY			"scatola-window-geometry"
#define SCATOLA_METADATA_KEY_WINDOW_SCROLL_POSITION		"scatola-window-scroll-position"
#define SCATOLA_METADATA_KEY_WINDOW_SHOW_HIDDEN_FILES		"scatola-window-show-hidden-files"
#define SCATOLA_METADATA_KEY_WINDOW_MAXIMIZED			"scatola-window-maximized"
#define SCATOLA_METADATA_KEY_WINDOW_STICKY			"scatola-window-sticky"
#define SCATOLA_METADATA_KEY_WINDOW_KEEP_ABOVE			"scatola-window-keep-above"

#define SCATOLA_METADATA_KEY_SIDEBAR_BACKGROUND_COLOR   	"scatola-sidebar-background-color"
#define SCATOLA_METADATA_KEY_SIDEBAR_BACKGROUND_IMAGE   	"scatola-sidebar-background-image"
#define SCATOLA_METADATA_KEY_SIDEBAR_BUTTONS			"scatola-sidebar-buttons"

#define SCATOLA_METADATA_KEY_ICON_POSITION              	"scatola-icon-position"
#define SCATOLA_METADATA_KEY_ICON_POSITION_TIMESTAMP		"scatola-icon-position-timestamp"
#define SCATOLA_METADATA_KEY_ANNOTATION                 	"annotation"
#define SCATOLA_METADATA_KEY_ICON_SCALE                 	"icon-scale"
#define SCATOLA_METADATA_KEY_CUSTOM_ICON                	"custom-icon"
#define SCATOLA_METADATA_KEY_SCREEN				"screen"
#define SCATOLA_METADATA_KEY_EMBLEMS				"emblems"

guint scatola_metadata_get_id (const char *metadata);

#endif /* SCATOLA_METADATA_H */
