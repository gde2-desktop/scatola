/* -*- Mode: C; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 8 -*- */

/*
 *  Scatola
 *
 *  Copyright (C) 1999, 2000 Red Hat, Inc.
 *  Copyright (C) 1999, 2000, 2001 Eazel, Inc.
 *
 *  Scatola is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License as
 *  published by the Free Software Foundation; either version 2 of the
 *  License, or (at your option) any later version.
 *
 *  Scatola is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 *  Authors: Elliot Lee <sopwith@redhat.com>
 *           Darin Adler <darin@bentspoon.com>
 *
 */

#ifndef SCATOLA_WINDOW_PRIVATE_H
#define SCATOLA_WINDOW_PRIVATE_H

#include "scatola-window.h"
#include "scatola-window-slot.h"
#include "scatola-window-pane.h"
#include "scatola-spatial-window.h"
#include "scatola-navigation-window.h"
#include "scatola-bookmark-list.h"

#include <libscatola-private/scatola-directory.h>

struct _ScatolaNavigationWindowPane;

/* FIXME bugzilla.gnome.org 42575: Migrate more fields into here. */
struct ScatolaWindowDetails
{
#if GTK_CHECK_VERSION (3, 0, 0)
    GtkWidget *grid;
#else
    GtkWidget *table;
#endif
    GtkWidget *statusbar;
    GtkWidget *menubar;

    GtkUIManager *ui_manager;
    GtkActionGroup *main_action_group; /* owned by ui_manager */
    guint help_message_cid;

    /* Menus. */
    guint extensions_menu_merge_id;
    GtkActionGroup *extensions_menu_action_group;

    GtkActionGroup *bookmarks_action_group;
    guint bookmarks_merge_id;
    ScatolaBookmarkList *bookmark_list;

    ScatolaWindowShowHiddenFilesMode show_hidden_files_mode;

    /* View As menu */
    GList *short_list_viewers;
    char *extra_viewer;

    /* View As choices */
    GtkActionGroup *view_as_action_group; /* owned by ui_manager */
    GtkRadioAction *view_as_radio_action;
    GtkRadioAction *extra_viewer_radio_action;
    guint short_list_merge_id;
    guint extra_viewer_merge_id;

    /* Ensures that we do not react on signals of a
     * view that is re-used as new view when its loading
     * is cancelled
     */
    gboolean temporarily_ignore_view_signals;

    /* available panes, and active pane.
     * Both of them may never be NULL.
     */
    GList *panes;
    ScatolaWindowPane *active_pane;

    /* So we can tell which window initiated
     * an unmount operation.
     */
    gboolean initiated_unmount;
};

struct _ScatolaNavigationWindowDetails
{
    GtkWidget *content_paned;
    GtkWidget *content_box;
    GtkActionGroup *navigation_action_group; /* owned by ui_manager */

    GtkSizeGroup *header_size_group;

    /* Side Pane */
    int side_pane_width;
    ScatolaSidebar *current_side_panel;

    /* Menus */
    GtkActionGroup *go_menu_action_group;
    guint refresh_go_menu_idle_id;
    guint go_menu_merge_id;

    /* Toolbar */
    GtkWidget *toolbar;

    guint extensions_toolbar_merge_id;
    GtkActionGroup *extensions_toolbar_action_group;

    /* spinner */
    gboolean    spinner_active;
    GtkWidget  *spinner;

    /* focus widget before the location bar has been shown temporarily */
    GtkWidget *last_focus_widget;

    /* split view */
    GtkWidget *split_view_hpane;
};

#define SCATOLA_MENU_PATH_BACK_ITEM			"/menu/Go/Back"
#define SCATOLA_MENU_PATH_FORWARD_ITEM			"/menu/Go/Forward"
#define SCATOLA_MENU_PATH_UP_ITEM			"/menu/Go/Up"

#define SCATOLA_MENU_PATH_RELOAD_ITEM			"/menu/View/Reload"
#define SCATOLA_MENU_PATH_ZOOM_IN_ITEM			"/menu/View/Zoom Items Placeholder/Zoom In"
#define SCATOLA_MENU_PATH_ZOOM_OUT_ITEM		"/menu/View/Zoom Items Placeholder/Zoom Out"
#define SCATOLA_MENU_PATH_ZOOM_NORMAL_ITEM		"/menu/View/Zoom Items Placeholder/Zoom Normal"

#define SCATOLA_COMMAND_BACK				"/commands/Back"
#define SCATOLA_COMMAND_FORWARD			"/commands/Forward"
#define SCATOLA_COMMAND_UP				"/commands/Up"

#define SCATOLA_COMMAND_RELOAD				"/commands/Reload"
#define SCATOLA_COMMAND_BURN_CD			"/commands/Burn CD"
#define SCATOLA_COMMAND_STOP				"/commands/Stop"
#define SCATOLA_COMMAND_ZOOM_IN			"/commands/Zoom In"
#define SCATOLA_COMMAND_ZOOM_OUT			"/commands/Zoom Out"
#define SCATOLA_COMMAND_ZOOM_NORMAL			"/commands/Zoom Normal"

/* window geometry */
/* Min values are very small, and a Scatola window at this tiny size is *almost*
 * completely unusable. However, if all the extra bits (sidebar, location bar, etc)
 * are turned off, you can see an icon or two at this size. See bug 5946.
 */

#define SCATOLA_SPATIAL_WINDOW_MIN_WIDTH			100
#define SCATOLA_SPATIAL_WINDOW_MIN_HEIGHT			100
#define SCATOLA_SPATIAL_WINDOW_DEFAULT_WIDTH			500
#define SCATOLA_SPATIAL_WINDOW_DEFAULT_HEIGHT			300

#define SCATOLA_NAVIGATION_WINDOW_MIN_WIDTH			200
#define SCATOLA_NAVIGATION_WINDOW_MIN_HEIGHT			200
#define SCATOLA_NAVIGATION_WINDOW_DEFAULT_WIDTH		800
#define SCATOLA_NAVIGATION_WINDOW_DEFAULT_HEIGHT		550

typedef void (*ScatolaBookmarkFailedCallback) (ScatolaWindow *window,
        ScatolaBookmark *bookmark);

void               scatola_window_set_status                            (ScatolaWindow    *window,
        ScatolaWindowSlot *slot,
        const char        *status);
void               scatola_window_load_view_as_menus                    (ScatolaWindow    *window);
void               scatola_window_load_extension_menus                  (ScatolaWindow    *window);
void               scatola_window_initialize_menus                      (ScatolaWindow    *window);
void               scatola_window_finalize_menus                        (ScatolaWindow    *window);
ScatolaWindowPane *scatola_window_get_next_pane                        (ScatolaWindow *window);
void               scatola_menus_append_bookmark_to_menu                (ScatolaWindow    *window,
        ScatolaBookmark  *bookmark,
        const char        *parent_path,
        const char        *parent_id,
        guint              index_in_parent,
        GtkActionGroup    *action_group,
        guint              merge_id,
        GCallback          refresh_callback,
        ScatolaBookmarkFailedCallback failed_callback);
void               scatola_window_update_find_menu_item                 (ScatolaWindow    *window);
void               scatola_window_zoom_in                               (ScatolaWindow    *window);
void               scatola_window_zoom_out                              (ScatolaWindow    *window);
void               scatola_window_zoom_to_level                         (ScatolaWindow    *window,
        ScatolaZoomLevel  level);
void               scatola_window_zoom_to_default                       (ScatolaWindow    *window);

ScatolaWindowSlot *scatola_window_open_slot                            (ScatolaWindowPane *pane,
        ScatolaWindowOpenSlotFlags flags);
void                scatola_window_close_slot                           (ScatolaWindowSlot *slot);

ScatolaWindowSlot *scatola_window_get_slot_for_view                    (ScatolaWindow *window,
        ScatolaView   *view);

GList *              scatola_window_get_slots                           (ScatolaWindow    *window);
ScatolaWindowSlot * scatola_window_get_active_slot                     (ScatolaWindow    *window);
ScatolaWindowSlot * scatola_window_get_extra_slot                      (ScatolaWindow    *window);
void                 scatola_window_set_active_slot                     (ScatolaWindow    *window,
        ScatolaWindowSlot *slot);
void                 scatola_window_set_active_pane                     (ScatolaWindow *window,
        ScatolaWindowPane *new_pane);
ScatolaWindowPane * scatola_window_get_active_pane                     (ScatolaWindow *window);

void               scatola_send_history_list_changed                    (void);
void               scatola_remove_from_history_list_no_notify           (GFile             *location);
gboolean           scatola_add_bookmark_to_history_list                 (ScatolaBookmark  *bookmark);
gboolean           scatola_add_to_history_list_no_notify                (GFile             *location,
        const char        *name,
        gboolean           has_custom_name,
        GIcon            *icon);
GList *            scatola_get_history_list                             (void);
void               scatola_window_bookmarks_preference_changed_callback (gpointer           user_data);


/* sync window GUI with current slot. Used when changing slots,
 * and when updating the slot state.
 */
void scatola_window_sync_status           (ScatolaWindow *window);
void scatola_window_sync_allow_stop       (ScatolaWindow *window,
                                        ScatolaWindowSlot *slot);
void scatola_window_sync_title            (ScatolaWindow *window,
                                        ScatolaWindowSlot *slot);
void scatola_window_sync_zoom_widgets     (ScatolaWindow *window);

/* Navigation window menus */
void               scatola_navigation_window_initialize_actions                    (ScatolaNavigationWindow    *window);
void               scatola_navigation_window_initialize_menus                      (ScatolaNavigationWindow    *window);
void               scatola_navigation_window_remove_bookmarks_menu_callback        (ScatolaNavigationWindow    *window);

void               scatola_navigation_window_remove_bookmarks_menu_items           (ScatolaNavigationWindow    *window);
void               scatola_navigation_window_update_show_hide_menu_items           (ScatolaNavigationWindow     *window);
void               scatola_navigation_window_update_spatial_menu_item              (ScatolaNavigationWindow     *window);
void               scatola_navigation_window_remove_go_menu_callback    (ScatolaNavigationWindow    *window);
void               scatola_navigation_window_remove_go_menu_items       (ScatolaNavigationWindow    *window);

/* Navigation window toolbar */
void               scatola_navigation_window_activate_spinner                     (ScatolaNavigationWindow    *window);
void               scatola_navigation_window_initialize_toolbars                   (ScatolaNavigationWindow    *window);
void               scatola_navigation_window_load_extension_toolbar_items          (ScatolaNavigationWindow    *window);
void               scatola_navigation_window_set_spinner_active                   (ScatolaNavigationWindow    *window,
        gboolean                     active);
void               scatola_navigation_window_go_back                               (ScatolaNavigationWindow    *window);
void               scatola_navigation_window_go_forward                            (ScatolaNavigationWindow    *window);
void               scatola_window_close_pane                                       (ScatolaWindowPane *pane);
void               scatola_navigation_window_update_split_view_actions_sensitivity (ScatolaNavigationWindow    *window);

#endif /* SCATOLA_WINDOW_PRIVATE_H */
