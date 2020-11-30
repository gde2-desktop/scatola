/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/* scatola-global-preferences.h - Scatola specific preference keys and
                                   functions.

   Copyright (C) 1999, 2000, 2001 Eazel, Inc.

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with this program; see the file COPYING.LIB.  If not,
   write to the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
   Boston, MA 02110-1301, USA.

   Authors: Ramiro Estrugo <ramiro@eazel.com>
*/

#ifndef SCATOLA_GLOBAL_PREFERENCES_H
#define SCATOLA_GLOBAL_PREFERENCES_H

#include <gio/gio.h>

G_BEGIN_DECLS

/* Whether exit when last window destroyed */
#define SCATOLA_PREFERENCES_EXIT_WITH_LAST_WINDOW		"exit-with-last-window"

/* Desktop Background options */
#define SCATOLA_PREFERENCES_BACKGROUND_SET			"background-set"
#define SCATOLA_PREFERENCES_BACKGROUND_COLOR		"background-color"
#define SCATOLA_PREFERENCES_BACKGROUND_URI			"background-uri"

/* Side Pane Background options */
#define SCATOLA_PREFERENCES_SIDE_PANE_BACKGROUND_SET	"side-pane-background-set"
#define SCATOLA_PREFERENCES_SIDE_PANE_BACKGROUND_COLOR	"side-pane-background-color"
#define SCATOLA_PREFERENCES_SIDE_PANE_BACKGROUND_URI	"side-pane-background-uri"

/* Automount options */
#define SCATOLA_PREFERENCES_MEDIA_AUTOMOUNT		"automount"
#define SCATOLA_PREFERENCES_MEDIA_AUTOMOUNT_OPEN		"automount-open"

/* Autorun options */
#define SCATOLA_PREFERENCES_MEDIA_AUTORUN_NEVER			"autorun-never"
#define SCATOLA_PREFERENCES_MEDIA_AUTORUN_X_CONTENT_START_APP	"autorun-x-content-start-app"
#define SCATOLA_PREFERENCES_MEDIA_AUTORUN_X_CONTENT_IGNORE		"autorun-x-content-ignore"
#define SCATOLA_PREFERENCES_MEDIA_AUTORUN_X_CONTENT_OPEN_FOLDER	"autorun-x-content-open-folder"

/* Trash options */
#define SCATOLA_PREFERENCES_CONFIRM_TRASH			"confirm-trash"
#define SCATOLA_PREFERENCES_ENABLE_DELETE			"enable-delete"

/* Desktop options */
#define SCATOLA_PREFERENCES_DESKTOP_IS_HOME_DIR		"desktop-is-home-dir"

/* Display  */
#define SCATOLA_PREFERENCES_SHOW_HIDDEN_FILES  		"show-hidden-files"
#define SCATOLA_PREFERENCES_SHOW_ADVANCED_PERMISSIONS	"show-advanced-permissions"
#define SCATOLA_PREFERENCES_DATE_FORMAT			"date-format"
#define SCATOLA_PREFERENCES_USE_IEC_UNITS			"use-iec-units"

/* Mouse */
#define SCATOLA_PREFERENCES_MOUSE_USE_EXTRA_BUTTONS 	"mouse-use-extra-buttons"
#define SCATOLA_PREFERENCES_MOUSE_FORWARD_BUTTON		"mouse-forward-button"
#define SCATOLA_PREFERENCES_MOUSE_BACK_BUTTON		"mouse-back-button"

typedef enum
{
    SCATOLA_DATE_FORMAT_LOCALE,
    SCATOLA_DATE_FORMAT_ISO,
    SCATOLA_DATE_FORMAT_INFORMAL
} ScatolaDateFormat;

typedef enum
{
    SCATOLA_NEW_TAB_POSITION_AFTER_CURRENT_TAB,
    SCATOLA_NEW_TAB_POSITION_END,
} ScatolaNewTabPosition;

/* Sidebar panels  */
#define SCATOLA_PREFERENCES_TREE_SHOW_ONLY_DIRECTORIES         "show-only-directories"

/* Single/Double click preference  */
#define SCATOLA_PREFERENCES_CLICK_POLICY			"click-policy"

/* Activating executable text files */
#define SCATOLA_PREFERENCES_EXECUTABLE_TEXT_ACTIVATION		"executable-text-activation"

/* Installing new packages when unknown mime type activated */
#define SCATOLA_PREFERENCES_INSTALL_MIME_ACTIVATION		"install-mime-activation"

/* Spatial or browser mode */
#define SCATOLA_PREFERENCES_ALWAYS_USE_BROWSER       		"always-use-browser"
#define SCATOLA_PREFERENCES_NEW_TAB_POSITION       		"tabs-open-position"
#define SCATOLA_PREFERENCES_ALWAYS_USE_LOCATION_ENTRY			"always-use-location-entry"

/* Which views should be displayed for new windows */
#define SCATOLA_WINDOW_STATE_START_WITH_LOCATION_BAR			"start-with-location-bar"
#define SCATOLA_WINDOW_STATE_START_WITH_STATUS_BAR				"start-with-status-bar"
#define SCATOLA_WINDOW_STATE_START_WITH_SIDEBAR		 		"start-with-sidebar"
#define SCATOLA_WINDOW_STATE_START_WITH_TOOLBAR				"start-with-toolbar"
#define SCATOLA_WINDOW_STATE_SIDE_PANE_VIEW                    "side-pane-view"
#define SCATOLA_WINDOW_STATE_GEOMETRY 	"geometry"
#define SCATOLA_WINDOW_STATE_MAXIMIZED        "maximized"
#define SCATOLA_WINDOW_STATE_SIDEBAR_WIDTH  					"sidebar-width"

/* Sorting order */
#define SCATOLA_PREFERENCES_SORT_DIRECTORIES_FIRST		        "sort-directories-first"
#define SCATOLA_PREFERENCES_DEFAULT_SORT_ORDER			        "default-sort-order"
#define SCATOLA_PREFERENCES_DEFAULT_SORT_IN_REVERSE_ORDER	    "default-sort-in-reverse-order"

/* The default folder viewer - one of the two enums below */
#define SCATOLA_PREFERENCES_DEFAULT_FOLDER_VIEWER		"default-folder-viewer"

    enum
    {
        SCATOLA_DEFAULT_FOLDER_VIEWER_ICON_VIEW,
        SCATOLA_DEFAULT_FOLDER_VIEWER_COMPACT_VIEW,
        SCATOLA_DEFAULT_FOLDER_VIEWER_LIST_VIEW,
        SCATOLA_DEFAULT_FOLDER_VIEWER_OTHER
    };

    /* These IIDs are used by the preferences code and in scatola-application.c */
#define SCATOLA_ICON_VIEW_IID		"OAFIID:Scatola_File_Manager_Icon_View"
#define SCATOLA_COMPACT_VIEW_IID	"OAFIID:Scatola_File_Manager_Compact_View"
#define SCATOLA_LIST_VIEW_IID		"OAFIID:Scatola_File_Manager_List_View"


    /* Icon View */
#define SCATOLA_PREFERENCES_ICON_VIEW_DEFAULT_USE_TIGHTER_LAYOUT	    "default-use-tighter-layout"
#define SCATOLA_PREFERENCES_ICON_VIEW_DEFAULT_ZOOM_LEVEL		        "default-zoom-level"

#define SCATOLA_PREFERENCES_ICON_VIEW_LABELS_BESIDE_ICONS      	"labels-beside-icons"



    /* Which text attributes appear beneath icon names */
#define SCATOLA_PREFERENCES_ICON_VIEW_CAPTIONS				"captions"

    /* The default size for thumbnail icons */
#define SCATOLA_PREFERENCES_ICON_VIEW_THUMBNAIL_SIZE			"thumbnail-size"

    /* ellipsization preferences */
#define SCATOLA_PREFERENCES_ICON_VIEW_TEXT_ELLIPSIS_LIMIT		"text-ellipsis-limit"
#define SCATOLA_PREFERENCES_DESKTOP_TEXT_ELLIPSIS_LIMIT		"text-ellipsis-limit"

    /* Compact View */
#define SCATOLA_PREFERENCES_COMPACT_VIEW_DEFAULT_ZOOM_LEVEL		"default-zoom-level"
#define SCATOLA_PREFERENCES_COMPACT_VIEW_ALL_COLUMNS_SAME_WIDTH	"all-columns-have-same-width"

    /* List View */
#define SCATOLA_PREFERENCES_LIST_VIEW_DEFAULT_ZOOM_LEVEL		"default-zoom-level"
#define SCATOLA_PREFERENCES_LIST_VIEW_DEFAULT_VISIBLE_COLUMNS	"default-visible-columns"
#define SCATOLA_PREFERENCES_LIST_VIEW_DEFAULT_COLUMN_ORDER		"default-column-order"

    enum
    {
        SCATOLA_CLICK_POLICY_SINGLE,
        SCATOLA_CLICK_POLICY_DOUBLE
    };

    enum
    {
        SCATOLA_EXECUTABLE_TEXT_LAUNCH,
        SCATOLA_EXECUTABLE_TEXT_DISPLAY,
        SCATOLA_EXECUTABLE_TEXT_ASK
    };

    typedef enum
    {
        SCATOLA_SPEED_TRADEOFF_ALWAYS,
        SCATOLA_SPEED_TRADEOFF_LOCAL_ONLY,
        SCATOLA_SPEED_TRADEOFF_NEVER
    } ScatolaSpeedTradeoffValue;

#define SCATOLA_PREFERENCES_SHOW_TEXT_IN_ICONS		    "show-icon-text"
#define SCATOLA_PREFERENCES_SHOW_DIRECTORY_ITEM_COUNTS "show-directory-item-counts"
#define SCATOLA_PREFERENCES_SHOW_IMAGE_FILE_THUMBNAILS	"show-image-thumbnails"
#define SCATOLA_PREFERENCES_IMAGE_FILE_THUMBNAIL_LIMIT	"thumbnail-limit"
#define SCATOLA_PREFERENCES_PREVIEW_SOUND		        "preview-sound"

    typedef enum
    {
        SCATOLA_COMPLEX_SEARCH_BAR,
        SCATOLA_SIMPLE_SEARCH_BAR
    } ScatolaSearchBarMode;

#define SCATOLA_PREFERENCES_DESKTOP_FONT                  "font"
#define SCATOLA_PREFERENCES_DESKTOP_HOME_VISIBLE          "home-icon-visible"
#define SCATOLA_PREFERENCES_DESKTOP_HOME_NAME             "home-icon-name"
#define SCATOLA_PREFERENCES_DESKTOP_COMPUTER_VISIBLE      "computer-icon-visible"
#define SCATOLA_PREFERENCES_DESKTOP_COMPUTER_NAME         "computer-icon-name"
#define SCATOLA_PREFERENCES_DESKTOP_TRASH_VISIBLE         "trash-icon-visible"
#define SCATOLA_PREFERENCES_DESKTOP_TRASH_NAME            "trash-icon-name"
#define SCATOLA_PREFERENCES_DESKTOP_VOLUMES_VISIBLE       "volumes-visible"
#define SCATOLA_PREFERENCES_DESKTOP_NETWORK_VISIBLE       "network-icon-visible"
#define SCATOLA_PREFERENCES_DESKTOP_NETWORK_NAME          "network-icon-name"
#define SCATOLA_PREFERENCES_LOCKDOWN_COMMAND_LINE         "disable-command-line"
#define SCATOLA_PREFERENCES_DISABLED_EXTENSIONS           "disabled-extensions"

void scatola_global_preferences_init                      (void);
char *scatola_global_preferences_get_default_folder_viewer_preference_as_iid (void);

GSettings *scatola_preferences;
GSettings *scatola_media_preferences;
GSettings *scatola_window_state;
GSettings *scatola_icon_view_preferences;
GSettings *scatola_desktop_preferences;
GSettings *scatola_tree_sidebar_preferences;
GSettings *scatola_compact_view_preferences;
GSettings *scatola_list_view_preferences;
GSettings *scatola_extension_preferences;

GSettings *gde2_background_preferences;
GSettings *gde2_lockdown_preferences;

G_END_DECLS

#endif /* SCATOLA_GLOBAL_PREFERENCES_H */
