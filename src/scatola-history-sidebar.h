/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/*
 *  Scatola
 *
 *  Copyright (C) 1999, 2000 Red Hat, Inc.
 *  Copyright (C) 2000, 2001 Eazel, Inc.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License as
 *  published by the Free Software Foundation; either version 2 of the
 *  License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 *  Authors: Elliot Lee <sopwith@redhat.com>
 *           Darin Adler <darin@bentspoon.com>
 *
 */
#ifndef _SCATOLA_HISTORY_SIDEBAR_H
#define _SCATOLA_HISTORY_SIDEBAR_H

#include <gtk/gtk.h>
#include <libscatola-private/scatola-view.h>
#include <libscatola-private/scatola-window-info.h>

#define SCATOLA_HISTORY_SIDEBAR_ID    "history"

#define SCATOLA_TYPE_HISTORY_SIDEBAR scatola_history_sidebar_get_type()
#define SCATOLA_HISTORY_SIDEBAR(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), SCATOLA_TYPE_HISTORY_SIDEBAR, ScatolaHistorySidebar))

typedef struct
{
    GtkScrolledWindow parent;
    GtkTreeView *tree_view;
    ScatolaWindowInfo *window;
} ScatolaHistorySidebar;

GType scatola_history_sidebar_get_type (void);
void scatola_history_sidebar_register (void);

#endif
