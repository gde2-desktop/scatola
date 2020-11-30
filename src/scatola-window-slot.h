/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*-

   scatola-window-slot.h: Scatola window slot

   Copyright (C) 2008 Free Software Foundation, Inc.

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

   Author: Christian Neumair <cneumair@gnome.org>
*/

#ifndef SCATOLA_WINDOW_SLOT_H
#define SCATOLA_WINDOW_SLOT_H

#include "scatola-window-pane.h"
#include "scatola-query-editor.h"
#include <glib/gi18n.h>

#define SCATOLA_TYPE_WINDOW_SLOT	 (scatola_window_slot_get_type())
#define SCATOLA_WINDOW_SLOT_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST((k), SCATOLA_TYPE_WINDOW_SLOT, ScatolaWindowSlotClass))
#define SCATOLA_WINDOW_SLOT(obj)	 (G_TYPE_CHECK_INSTANCE_CAST ((obj), SCATOLA_TYPE_WINDOW_SLOT, ScatolaWindowSlot))
#define SCATOLA_IS_WINDOW_SLOT(obj)      (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SCATOLA_TYPE_WINDOW_SLOT))
#define SCATOLA_IS_WINDOW_SLOT_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE ((k), SCATOLA_TYPE_WINDOW_SLOT))
#define SCATOLA_WINDOW_SLOT_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), SCATOLA_TYPE_WINDOW_SLOT, ScatolaWindowSlotClass))

typedef enum
{
    SCATOLA_LOCATION_CHANGE_STANDARD,
    SCATOLA_LOCATION_CHANGE_BACK,
    SCATOLA_LOCATION_CHANGE_FORWARD,
    SCATOLA_LOCATION_CHANGE_RELOAD,
    SCATOLA_LOCATION_CHANGE_REDIRECT,
    SCATOLA_LOCATION_CHANGE_FALLBACK
} ScatolaLocationChangeType;

struct ScatolaWindowSlotClass
{
    GObjectClass parent_class;

    /* wrapped ScatolaWindowInfo signals, for overloading */
    void (* active)   (ScatolaWindowSlot *slot);
    void (* inactive) (ScatolaWindowSlot *slot);

    void (* update_query_editor) (ScatolaWindowSlot *slot);
};

/* Each ScatolaWindowSlot corresponds to
 * a location in the window for displaying
 * a ScatolaView.
 *
 * For navigation windows, this would be a
 * tab, while spatial windows only have one slot.
 */
struct ScatolaWindowSlot
{
    GObject parent;

    ScatolaWindowPane *pane;

    /* content_box contains
     *  1) an event box containing extra_location_widgets
     *  2) the view box for the content view
     */
    GtkWidget *content_box;
    GtkWidget *extra_location_frame;
    GtkWidget *extra_location_widgets;
    GtkWidget *view_box;

    ScatolaView *content_view;
    ScatolaView *new_content_view;

    /* Information about bookmarks */
    ScatolaBookmark *current_location_bookmark;
    ScatolaBookmark *last_location_bookmark;

    /* Current location. */
    GFile *location;
    char *title;
    char *status_text;

    ScatolaFile *viewed_file;
    gboolean viewed_file_seen;
    gboolean viewed_file_in_trash;

    gboolean allow_stop;

    ScatolaQueryEditor *query_editor;

    /* New location. */
    ScatolaLocationChangeType location_change_type;
    guint location_change_distance;
    GFile *pending_location;
    char *pending_scroll_to;
    GList *pending_selection;
    ScatolaFile *determine_view_file;
    GCancellable *mount_cancellable;
    GError *mount_error;
    gboolean tried_mount;
    ScatolaWindowGoToCallback open_callback;
    gpointer open_callback_user_data;

    GCancellable *find_mount_cancellable;

    gboolean visible;
};

GType   scatola_window_slot_get_type (void);

char *  scatola_window_slot_get_title			   (ScatolaWindowSlot *slot);
void    scatola_window_slot_update_title		   (ScatolaWindowSlot *slot);
void    scatola_window_slot_update_icon		   (ScatolaWindowSlot *slot);
void    scatola_window_slot_update_query_editor	   (ScatolaWindowSlot *slot);

GFile * scatola_window_slot_get_location		   (ScatolaWindowSlot *slot);
char *  scatola_window_slot_get_location_uri		   (ScatolaWindowSlot *slot);

void    scatola_window_slot_close			   (ScatolaWindowSlot *slot);
void    scatola_window_slot_reload			   (ScatolaWindowSlot *slot);

void			scatola_window_slot_open_location	      (ScatolaWindowSlot	*slot,
        GFile			*location,
        gboolean			 close_behind);
void			scatola_window_slot_open_location_with_selection (ScatolaWindowSlot	    *slot,
        GFile		    *location,
        GList		    *selection,
        gboolean		     close_behind);
void			scatola_window_slot_open_location_full       (ScatolaWindowSlot	*slot,
        GFile			*location,
        ScatolaWindowOpenMode	 mode,
        ScatolaWindowOpenFlags	 flags,
        GList			*new_selection,
        ScatolaWindowGoToCallback   callback,
        gpointer		 user_data);
void			scatola_window_slot_stop_loading	      (ScatolaWindowSlot	*slot);

void			scatola_window_slot_set_content_view	      (ScatolaWindowSlot	*slot,
        const char		*id);
const char	       *scatola_window_slot_get_content_view_id      (ScatolaWindowSlot	*slot);
gboolean		scatola_window_slot_content_view_matches_iid (ScatolaWindowSlot	*slot,
        const char		*iid);

void                    scatola_window_slot_connect_content_view     (ScatolaWindowSlot       *slot,
        ScatolaView             *view);
void                    scatola_window_slot_disconnect_content_view  (ScatolaWindowSlot       *slot,
        ScatolaView             *view);

#define scatola_window_slot_go_to(slot,location, new_tab) \
	scatola_window_slot_open_location_full(slot, location, SCATOLA_WINDOW_OPEN_ACCORDING_TO_MODE, \
						(new_tab ? SCATOLA_WINDOW_OPEN_FLAG_NEW_TAB : 0), \
						NULL, NULL, NULL)

#define scatola_window_slot_go_to_full(slot, location, new_tab, callback, user_data) \
	scatola_window_slot_open_location_full(slot, location, SCATOLA_WINDOW_OPEN_ACCORDING_TO_MODE, \
						(new_tab ? SCATOLA_WINDOW_OPEN_FLAG_NEW_TAB : 0), \
						NULL, callback, user_data)

#define scatola_window_slot_go_to_with_selection(slot,location,new_selection) \
	scatola_window_slot_open_location_with_selection(slot, location, new_selection, FALSE)

void    scatola_window_slot_go_home			   (ScatolaWindowSlot *slot,
        gboolean            new_tab);
void    scatola_window_slot_go_up			   (ScatolaWindowSlot *slot,
        gboolean           close_behind);

void    scatola_window_slot_set_content_view_widget	   (ScatolaWindowSlot *slot,
        ScatolaView       *content_view);
void    scatola_window_slot_set_viewed_file		   (ScatolaWindowSlot *slot,
        ScatolaFile      *file);
void    scatola_window_slot_set_allow_stop		   (ScatolaWindowSlot *slot,
        gboolean	    allow_stop);
void    scatola_window_slot_set_status			   (ScatolaWindowSlot *slot,
        const char	 *status);

void    scatola_window_slot_add_extra_location_widget     (ScatolaWindowSlot *slot,
        GtkWidget       *widget);
void    scatola_window_slot_remove_extra_location_widgets (ScatolaWindowSlot *slot);

void    scatola_window_slot_add_current_location_to_history_list (ScatolaWindowSlot *slot);

void    scatola_window_slot_is_in_active_pane (ScatolaWindowSlot *slot, gboolean is_active);

#endif /* SCATOLA_WINDOW_SLOT_H */
