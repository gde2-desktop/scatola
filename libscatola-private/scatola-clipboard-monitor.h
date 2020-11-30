/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*-

   scatola-clipboard-monitor.h: lets you notice clipboard changes.

   Copyright (C) 2004 Red Hat, Inc.

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

#ifndef SCATOLA_CLIPBOARD_MONITOR_H
#define SCATOLA_CLIPBOARD_MONITOR_H

#include <gtk/gtk.h>

#define SCATOLA_TYPE_CLIPBOARD_MONITOR scatola_clipboard_monitor_get_type()
#define SCATOLA_CLIPBOARD_MONITOR(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), SCATOLA_TYPE_CLIPBOARD_MONITOR, ScatolaClipboardMonitor))
#define SCATOLA_CLIPBOARD_MONITOR_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), SCATOLA_TYPE_CLIPBOARD_MONITOR, ScatolaClipboardMonitorClass))
#define SCATOLA_IS_CLIPBOARD_MONITOR(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SCATOLA_TYPE_CLIPBOARD_MONITOR))
#define SCATOLA_IS_CLIPBOARD_MONITOR_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), SCATOLA_TYPE_CLIPBOARD_MONITOR))
#define SCATOLA_CLIPBOARD_MONITOR_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), SCATOLA_TYPE_CLIPBOARD_MONITOR, ScatolaClipboardMonitorClass))

typedef struct ScatolaClipboardMonitorDetails ScatolaClipboardMonitorDetails;
typedef struct ScatolaClipboardInfo ScatolaClipboardInfo;

typedef struct
{
    GObject parent_slot;

    ScatolaClipboardMonitorDetails *details;
} ScatolaClipboardMonitor;

typedef struct
{
    GObjectClass parent_slot;

    void (* clipboard_changed) (ScatolaClipboardMonitor *monitor);
    void (* clipboard_info) (ScatolaClipboardMonitor *monitor,
                             ScatolaClipboardInfo *info);
} ScatolaClipboardMonitorClass;

struct ScatolaClipboardInfo
{
    GList *files;
    gboolean cut;
};

GType   scatola_clipboard_monitor_get_type (void);

ScatolaClipboardMonitor *   scatola_clipboard_monitor_get (void);
void scatola_clipboard_monitor_set_clipboard_info (ScatolaClipboardMonitor *monitor,
        ScatolaClipboardInfo *info);
ScatolaClipboardInfo * scatola_clipboard_monitor_get_clipboard_info (ScatolaClipboardMonitor *monitor);
void scatola_clipboard_monitor_emit_changed (void);

void scatola_clear_clipboard_callback (GtkClipboard *clipboard,
                                    gpointer      user_data);
void scatola_get_clipboard_callback   (GtkClipboard     *clipboard,
                                    GtkSelectionData *selection_data,
                                    guint             info,
                                    gpointer          user_data);



#endif /* SCATOLA_CLIPBOARD_MONITOR_H */

