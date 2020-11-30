/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*-

   scatola-progress-info.h: file operation progress info.

   Copyright (C) 2007 Red Hat, Inc.

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

#ifndef SCATOLA_PROGRESS_INFO_H
#define SCATOLA_PROGRESS_INFO_H

#include <glib-object.h>
#include <gio/gio.h>

#define SCATOLA_TYPE_PROGRESS_INFO         (scatola_progress_info_get_type ())
#define SCATOLA_PROGRESS_INFO(o)           (G_TYPE_CHECK_INSTANCE_CAST ((o), SCATOLA_TYPE_PROGRESS_INFO, ScatolaProgressInfo))
#define SCATOLA_PROGRESS_INFO_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST((k), SCATOLA_TYPE_PROGRESS_INFO, ScatolaProgressInfoClass))
#define SCATOLA_IS_PROGRESS_INFO(o)        (G_TYPE_CHECK_INSTANCE_TYPE ((o), SCATOLA_TYPE_PROGRESS_INFO))
#define SCATOLA_IS_PROGRESS_INFO_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE ((k), SCATOLA_TYPE_PROGRESS_INFO))
#define SCATOLA_PROGRESS_INFO_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), SCATOLA_TYPE_PROGRESS_INFO, ScatolaProgressInfoClass))

typedef struct _ScatolaProgressInfo      ScatolaProgressInfo;
typedef struct _ScatolaProgressInfoClass ScatolaProgressInfoClass;

GType scatola_progress_info_get_type (void) G_GNUC_CONST;

/* Signals:
   "changed" - status or details changed
   "progress-changed" - the percentage progress changed (or we pulsed if in activity_mode
   "started" - emited on job start
   "finished" - emitted when job is done

   All signals are emitted from idles in main loop.
   All methods are threadsafe.
 */

ScatolaProgressInfo *scatola_progress_info_new (void);

GList *       scatola_get_all_progress_info (void);

char *        scatola_progress_info_get_status      (ScatolaProgressInfo *info);
char *        scatola_progress_info_get_details     (ScatolaProgressInfo *info);
double        scatola_progress_info_get_progress    (ScatolaProgressInfo *info);
GCancellable *scatola_progress_info_get_cancellable (ScatolaProgressInfo *info);
void          scatola_progress_info_cancel          (ScatolaProgressInfo *info);
gboolean      scatola_progress_info_get_is_started  (ScatolaProgressInfo *info);
gboolean      scatola_progress_info_get_is_finished (ScatolaProgressInfo *info);
gboolean      scatola_progress_info_get_is_paused   (ScatolaProgressInfo *info);

void          scatola_progress_info_start           (ScatolaProgressInfo *info);
void          scatola_progress_info_finish          (ScatolaProgressInfo *info);
void          scatola_progress_info_pause           (ScatolaProgressInfo *info);
void          scatola_progress_info_resume          (ScatolaProgressInfo *info);
void          scatola_progress_info_set_status      (ScatolaProgressInfo *info,
        const char           *status);
void          scatola_progress_info_take_status     (ScatolaProgressInfo *info,
        char                 *status);
void          scatola_progress_info_set_details     (ScatolaProgressInfo *info,
        const char           *details);
void          scatola_progress_info_take_details    (ScatolaProgressInfo *info,
        char                 *details);
void          scatola_progress_info_set_progress    (ScatolaProgressInfo *info,
        double                current,
        double                total);
void          scatola_progress_info_pulse_progress  (ScatolaProgressInfo *info);



#endif /* SCATOLA_PROGRESS_INFO_H */
