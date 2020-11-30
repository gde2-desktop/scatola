/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/*
   scatola-trash-monitor.h: Scatola trash state watcher.

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

   Author: Pavel Cisler <pavel@eazel.com>
*/

#ifndef SCATOLA_TRASH_MONITOR_H
#define SCATOLA_TRASH_MONITOR_H

#include <gtk/gtk.h>
#include <gio/gio.h>

typedef struct ScatolaTrashMonitor ScatolaTrashMonitor;
typedef struct ScatolaTrashMonitorClass ScatolaTrashMonitorClass;
typedef struct ScatolaTrashMonitorDetails ScatolaTrashMonitorDetails;

#define SCATOLA_TYPE_TRASH_MONITOR scatola_trash_monitor_get_type()
#define SCATOLA_TRASH_MONITOR(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), SCATOLA_TYPE_TRASH_MONITOR, ScatolaTrashMonitor))
#define SCATOLA_TRASH_MONITOR_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), SCATOLA_TYPE_TRASH_MONITOR, ScatolaTrashMonitorClass))
#define SCATOLA_IS_TRASH_MONITOR(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SCATOLA_TYPE_TRASH_MONITOR))
#define SCATOLA_IS_TRASH_MONITOR_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), SCATOLA_TYPE_TRASH_MONITOR))
#define SCATOLA_TRASH_MONITOR_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), SCATOLA_TYPE_TRASH_MONITOR, ScatolaTrashMonitorClass))

struct ScatolaTrashMonitor
{
    GObject object;
    ScatolaTrashMonitorDetails *details;
};

struct ScatolaTrashMonitorClass
{
    GObjectClass parent_class;

    void (* trash_state_changed)		(ScatolaTrashMonitor 	*trash_monitor,
                                         gboolean 		 new_state);
};

GType			scatola_trash_monitor_get_type				(void);

ScatolaTrashMonitor   *scatola_trash_monitor_get 				(void);
gboolean		scatola_trash_monitor_is_empty 			(void);
GIcon                  *scatola_trash_monitor_get_icon                         (void);

#endif
