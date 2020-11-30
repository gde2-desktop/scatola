/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/*
 * Scatola
 *
 * Copyright (C) 1999, 2000 Eazel, Inc.
 *
 * Scatola is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * Scatola is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * Authors: John Sullivan <sullivan@eazel.com>
 */

/* scatola-bookmarks-window.h - interface for bookmark-editing window.
 */

#ifndef SCATOLA_BOOKMARKS_WINDOW_H
#define SCATOLA_BOOKMARKS_WINDOW_H

#include <gtk/gtk.h>
#include "scatola-bookmark-list.h"
#include "scatola-window.h"

GtkWindow *create_bookmarks_window                 (ScatolaBookmarkList *bookmarks,
                                                    ScatolaWindow       *window_source);
void       scatola_bookmarks_window_save_geometry     (GtkWindow        *window);
void	   edit_bookmarks_dialog_set_signals	   (ScatolaWindow       *window);

#endif /* SCATOLA_BOOKMARKS_WINDOW_H */
