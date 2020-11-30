/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*-

   scatola-window-slot-info.h: Interface for scatola window slots

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

#ifndef SCATOLA_WINDOW_SLOT_INFO_H
#define SCATOLA_WINDOW_SLOT_INFO_H

#include "scatola-window-info.h"
#include "scatola-view.h"


#define SCATOLA_TYPE_WINDOW_SLOT_INFO           (scatola_window_slot_info_get_type ())
#define SCATOLA_WINDOW_SLOT_INFO(obj)           (G_TYPE_CHECK_INSTANCE_CAST ((obj), SCATOLA_TYPE_WINDOW_SLOT_INFO, ScatolaWindowSlotInfo))
#define SCATOLA_IS_WINDOW_SLOT_INFO(obj)        (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SCATOLA_TYPE_WINDOW_SLOT_INFO))
#define SCATOLA_WINDOW_SLOT_INFO_GET_IFACE(obj) (G_TYPE_INSTANCE_GET_INTERFACE ((obj), SCATOLA_TYPE_WINDOW_SLOT_INFO, ScatolaWindowSlotInfoIface))

typedef struct _ScatolaWindowSlotInfoIface ScatolaWindowSlotInfoIface;

struct _ScatolaWindowSlotInfoIface
{
    GTypeInterface g_iface;

    /* signals */

    /* emitted right after this slot becomes active.
     * Views should connect to this signal and merge their UI
     * into the main window.
     */
    void  (* active)  (ScatolaWindowSlotInfo *slot);
    /* emitted right before this slot becomes inactive.
     * Views should connect to this signal and unmerge their UI
     * from the main window.
     */
    void  (* inactive) (ScatolaWindowSlotInfo *slot);

    /* returns the window info associated with this slot */
    ScatolaWindowInfo * (* get_window) (ScatolaWindowSlotInfo *slot);

    /* Returns the number of selected items in the view */
    int  (* get_selection_count)  (ScatolaWindowSlotInfo    *slot);

    /* Returns a list of uris for th selected items in the view, caller frees it */
    GList *(* get_selection)      (ScatolaWindowSlotInfo    *slot);

    char * (* get_current_location)  (ScatolaWindowSlotInfo *slot);
    ScatolaView * (* get_current_view) (ScatolaWindowSlotInfo *slot);
    void   (* set_status)            (ScatolaWindowSlotInfo *slot,
                                      const char *status);
    char * (* get_title)             (ScatolaWindowSlotInfo *slot);

    void   (* open_location)      (ScatolaWindowSlotInfo *slot,
                                   GFile *location,
                                   ScatolaWindowOpenMode mode,
                                   ScatolaWindowOpenFlags flags,
                                   GList *selection,
                                   ScatolaWindowGoToCallback callback,
                                   gpointer user_data);
    void   (* make_hosting_pane_active) (ScatolaWindowSlotInfo *slot);
};


GType                             scatola_window_slot_info_get_type            (void);
ScatolaWindowInfo *              scatola_window_slot_info_get_window          (ScatolaWindowSlotInfo            *slot);
#define scatola_window_slot_info_open_location(slot, location, mode, flags, selection) \
	scatola_window_slot_info_open_location_full(slot, location, mode, \
						 flags, selection, NULL, NULL)

void                              scatola_window_slot_info_open_location_full
	(ScatolaWindowSlotInfo *slot,
        GFile                             *location,
        ScatolaWindowOpenMode                 mode,
        ScatolaWindowOpenFlags                flags,
        GList                             *selection,
        ScatolaWindowGoToCallback		   callback,
        gpointer			   user_data);
void                              scatola_window_slot_info_set_status          (ScatolaWindowSlotInfo            *slot,
        const char *status);
void                              scatola_window_slot_info_make_hosting_pane_active (ScatolaWindowSlotInfo       *slot);

char *                            scatola_window_slot_info_get_current_location (ScatolaWindowSlotInfo           *slot);
ScatolaView *                    scatola_window_slot_info_get_current_view     (ScatolaWindowSlotInfo           *slot);
int                               scatola_window_slot_info_get_selection_count  (ScatolaWindowSlotInfo           *slot);
GList *                           scatola_window_slot_info_get_selection        (ScatolaWindowSlotInfo           *slot);
char *                            scatola_window_slot_info_get_title            (ScatolaWindowSlotInfo           *slot);

#endif /* SCATOLA_WINDOW_SLOT_INFO_H */
