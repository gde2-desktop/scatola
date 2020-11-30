/* -*- Mode: C; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

/*
 *  Scatola
 *
 *  Copyright (C) 1999, 2000 Red Hat, Inc.
 *  Copyright (C) 1999, 2000, 2001 Eazel, Inc.
 *
 *  Scatola is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  Scatola is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public
 *  License along with this program; if not, write to the Free
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 *  Author: Darin Adler <darin@bentspoon.com>
 *
 */

#ifndef SCATOLA_WINDOW_MANAGE_VIEWS_H
#define SCATOLA_WINDOW_MANAGE_VIEWS_H

#include "scatola-window.h"
#include "scatola-window-pane.h"
#include "scatola-navigation-window.h"

void scatola_window_manage_views_close_slot (ScatolaWindowPane *pane,
        ScatolaWindowSlot *slot);


/* ScatolaWindowInfo implementation: */
void scatola_window_report_load_underway     (ScatolaWindow     *window,
        ScatolaView       *view);
void scatola_window_report_selection_changed (ScatolaWindowInfo *window);
void scatola_window_report_view_failed       (ScatolaWindow     *window,
        ScatolaView       *view);
void scatola_window_report_load_complete     (ScatolaWindow     *window,
        ScatolaView       *view);
void scatola_window_report_location_change   (ScatolaWindow     *window);
void scatola_window_update_up_button         (ScatolaWindow     *window);

#endif /* SCATOLA_WINDOW_MANAGE_VIEWS_H */
