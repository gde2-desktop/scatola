/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*-

   scatola-desktop-link-monitor.h: singleton that manages the desktop links

   Copyright (C) 2003 Red Hat, Inc.

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

   Author: Alexander Larsson <alexl@redhat.com>
*/

#ifndef SCATOLA_DESKTOP_LINK_MONITOR_H
#define SCATOLA_DESKTOP_LINK_MONITOR_H

#include <gtk/gtk.h>
#include <libscatola-private/scatola-desktop-link.h>

#define SCATOLA_TYPE_DESKTOP_LINK_MONITOR scatola_desktop_link_monitor_get_type()
#define SCATOLA_DESKTOP_LINK_MONITOR(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), SCATOLA_TYPE_DESKTOP_LINK_MONITOR, ScatolaDesktopLinkMonitor))
#define SCATOLA_DESKTOP_LINK_MONITOR_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), SCATOLA_TYPE_DESKTOP_LINK_MONITOR, ScatolaDesktopLinkMonitorClass))
#define SCATOLA_IS_DESKTOP_LINK_MONITOR(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SCATOLA_TYPE_DESKTOP_LINK_MONITOR))
#define SCATOLA_IS_DESKTOP_LINK_MONITOR_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), SCATOLA_TYPE_DESKTOP_LINK_MONITOR))
#define SCATOLA_DESKTOP_LINK_MONITOR_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), SCATOLA_TYPE_DESKTOP_LINK_MONITOR, ScatolaDesktopLinkMonitorClass))

typedef struct ScatolaDesktopLinkMonitorDetails ScatolaDesktopLinkMonitorDetails;

typedef struct
{
    GObject parent_slot;
    ScatolaDesktopLinkMonitorDetails *details;
} ScatolaDesktopLinkMonitor;

typedef struct
{
    GObjectClass parent_slot;
} ScatolaDesktopLinkMonitorClass;

GType   scatola_desktop_link_monitor_get_type (void);

ScatolaDesktopLinkMonitor *   scatola_desktop_link_monitor_get (void);

/* Used by scatola-desktop-link.c */
char * scatola_desktop_link_monitor_make_filename_unique (ScatolaDesktopLinkMonitor *monitor,
        const char *filename);

#endif /* SCATOLA_DESKTOP_LINK_MONITOR_H */
