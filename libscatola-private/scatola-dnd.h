/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/* scatola-dnd.h - Common Drag & drop handling code shared by the icon container
   and the list view.

   Copyright (C) 2000 Eazel, Inc.

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

   Authors: Pavel Cisler <pavel@eazel.com>,
	    Ettore Perazzoli <ettore@gnu.org>
*/

#ifndef SCATOLA_DND_H
#define SCATOLA_DND_H

#include <gtk/gtk.h>
#include <libscatola-private/scatola-window-slot-info.h>

/* Drag & Drop target names. */
#define SCATOLA_ICON_DND_GDE2_ICON_LIST_TYPE	"x-special/gde2-icon-list"
#define SCATOLA_ICON_DND_URI_LIST_TYPE		"text/uri-list"
#define SCATOLA_ICON_DND_NETSCAPE_URL_TYPE	"_NETSCAPE_URL"
#define SCATOLA_ICON_DND_COLOR_TYPE		"application/x-color"
#define SCATOLA_ICON_DND_BGIMAGE_TYPE		"property/bgimage"
#define SCATOLA_ICON_DND_KEYWORD_TYPE		"property/keyword"
#define SCATOLA_ICON_DND_RESET_BACKGROUND_TYPE "x-special/gde2-reset-background"
#define SCATOLA_ICON_DND_ROOTWINDOW_DROP_TYPE	"application/x-rootwindow-drop"
#define SCATOLA_ICON_DND_XDNDDIRECTSAVE_TYPE	"XdndDirectSave0" /* XDS Protocol Type */
#define SCATOLA_ICON_DND_RAW_TYPE	"application/octet-stream"

/* Item of the drag selection list */
typedef struct
{
    char *uri;
    gboolean got_icon_position;
    int icon_x, icon_y;
    int icon_width, icon_height;
} ScatolaDragSelectionItem;

/* Standard Drag & Drop types. */
typedef enum
{
    SCATOLA_ICON_DND_GDE2_ICON_LIST,
    SCATOLA_ICON_DND_URI_LIST,
    SCATOLA_ICON_DND_NETSCAPE_URL,
    SCATOLA_ICON_DND_COLOR,
    SCATOLA_ICON_DND_BGIMAGE,
    SCATOLA_ICON_DND_KEYWORD,
    SCATOLA_ICON_DND_TEXT,
    SCATOLA_ICON_DND_RESET_BACKGROUND,
    SCATOLA_ICON_DND_XDNDDIRECTSAVE,
    SCATOLA_ICON_DND_RAW,
    SCATOLA_ICON_DND_ROOTWINDOW_DROP
} ScatolaIconDndTargetType;

typedef enum
{
    SCATOLA_DND_ACTION_FIRST = GDK_ACTION_ASK << 1,
    SCATOLA_DND_ACTION_SET_AS_BACKGROUND = SCATOLA_DND_ACTION_FIRST << 0,
    SCATOLA_DND_ACTION_SET_AS_FOLDER_BACKGROUND = SCATOLA_DND_ACTION_FIRST << 1,
    SCATOLA_DND_ACTION_SET_AS_GLOBAL_BACKGROUND = SCATOLA_DND_ACTION_FIRST << 2
} ScatolaDndAction;

/* drag&drop-related information. */
typedef struct
{
    GtkTargetList *target_list;

    /* Stuff saved at "receive data" time needed later in the drag. */
    gboolean got_drop_data_type;
    ScatolaIconDndTargetType data_type;
    GtkSelectionData *selection_data;
    char *direct_save_uri;

    /* Start of the drag, in window coordinates. */
    int start_x, start_y;

    /* List of ScatolaDragSelectionItems, representing items being dragged, or NULL
     * if data about them has not been received from the source yet.
     */
    GList *selection_list;

    /* has the drop occured ? */
    gboolean drop_occured;

    /* whether or not need to clean up the previous dnd data */
    gboolean need_to_destroy;

    /* autoscrolling during dragging */
    int auto_scroll_timeout_id;
    gboolean waiting_to_autoscroll;
    gint64 start_auto_scroll_in;

} ScatolaDragInfo;

typedef struct
{
    /* NB: the following elements are managed by us */
    gboolean have_data;
    gboolean have_valid_data;

    gboolean drop_occured;

    unsigned int info;
    union
    {
        GList *selection_list;
        GList *uri_list;
        char *netscape_url;
    } data;

    /* NB: the following elements are managed by the caller of
     *   scatola_drag_slot_proxy_init() */

    /* a fixed location, or NULL to use slot's location */
    GFile *target_location;
    /* a fixed slot, or NULL to use the window's active slot */
    ScatolaWindowSlotInfo *target_slot;
} ScatolaDragSlotProxyInfo;

typedef void		(* ScatolaDragEachSelectedItemDataGet)	(const char *url,
        int x, int y, int w, int h,
        gpointer data);
typedef void		(* ScatolaDragEachSelectedItemIterator)	(ScatolaDragEachSelectedItemDataGet iteratee,
        gpointer iterator_context,
        gpointer data);

void			    scatola_drag_init				(ScatolaDragInfo		      *drag_info,
        const GtkTargetEntry		      *drag_types,
        int				       drag_type_count,
        gboolean			       add_text_targets);
void			    scatola_drag_finalize			(ScatolaDragInfo		      *drag_info);
ScatolaDragSelectionItem  *scatola_drag_selection_item_new		(void);
void			    scatola_drag_destroy_selection_list	(GList				      *selection_list);
GList			   *scatola_drag_build_selection_list		(GtkSelectionData		      *data);

GList *			    scatola_drag_uri_list_from_selection_list	(const GList			      *selection_list);

GList *			    scatola_drag_uri_list_from_array		(const char			     **uris);

gboolean		    scatola_drag_items_local			(const char			      *target_uri,
        const GList			      *selection_list);
gboolean		    scatola_drag_uris_local			(const char			      *target_uri,
        const GList			      *source_uri_list);
gboolean		    scatola_drag_items_on_desktop		(const GList			      *selection_list);
void			    scatola_drag_default_drop_action_for_icons (GdkDragContext			      *context,
        const char			      *target_uri,
        const GList			      *items,
        int				      *action);
GdkDragAction		    scatola_drag_default_drop_action_for_netscape_url (GdkDragContext			     *context);
GdkDragAction		    scatola_drag_default_drop_action_for_uri_list     (GdkDragContext			     *context,
        const char			     *target_uri_string);
gboolean		    scatola_drag_drag_data_get			(GtkWidget			      *widget,
        GdkDragContext			      *context,
        GtkSelectionData		      *selection_data,
        guint				       info,
        guint32			       time,
        gpointer			       container_context,
        ScatolaDragEachSelectedItemIterator  each_selected_item_iterator);
int			    scatola_drag_modifier_based_action		(int				       default_action,
        int				       non_default_action);

GdkDragAction		    scatola_drag_drop_action_ask		(GtkWidget			      *widget,
        GdkDragAction			       possible_actions);
GdkDragAction		    scatola_drag_drop_background_ask		(GtkWidget			      *widget,
        GdkDragAction			       possible_actions);

gboolean		    scatola_drag_autoscroll_in_scroll_region	(GtkWidget			      *widget);
void			    scatola_drag_autoscroll_calculate_delta	(GtkWidget			      *widget,
        float				      *x_scroll_delta,
        float				      *y_scroll_delta);
void			    scatola_drag_autoscroll_start		(ScatolaDragInfo		      *drag_info,
        GtkWidget			      *widget,
        GSourceFunc			       callback,
        gpointer			       user_data);
void			    scatola_drag_autoscroll_stop		(ScatolaDragInfo		      *drag_info);

gboolean		    scatola_drag_selection_includes_special_link (GList			      *selection_list);

void                        scatola_drag_slot_proxy_init               (GtkWidget *widget,
        ScatolaDragSlotProxyInfo *drag_info);

#endif
