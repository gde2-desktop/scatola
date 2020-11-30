/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/* scatola-mime-actions.h - uri-specific versions of mime action functions

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

   Authors: Maciej Stachowiak <mjs@eazel.com>
*/

#ifndef SCATOLA_MIME_ACTIONS_H
#define SCATOLA_MIME_ACTIONS_H

#include <gio/gio.h>

#include <libscatola-private/scatola-file.h>
#include <libscatola-private/scatola-window-info.h>
#include <libscatola-private/scatola-window-slot-info.h>

ScatolaFileAttributes scatola_mime_actions_get_required_file_attributes (void);

GAppInfo *             scatola_mime_get_default_application_for_file     (ScatolaFile            *file);
GList *                scatola_mime_get_applications_for_file            (ScatolaFile            *file);

GAppInfo *             scatola_mime_get_default_application_for_files    (GList                   *files);
GList *                scatola_mime_get_applications_for_files           (GList                   *file);

gboolean               scatola_mime_has_any_applications_for_file        (ScatolaFile            *file);

gboolean               scatola_mime_file_opens_in_view                   (ScatolaFile            *file);
gboolean               scatola_mime_file_opens_in_external_app           (ScatolaFile            *file);
void                   scatola_mime_activate_files                       (GtkWindow               *parent_window,
        ScatolaWindowSlotInfo  *slot_info,
        GList                   *files,
        const char              *launch_directory,
        ScatolaWindowOpenMode   mode,
        ScatolaWindowOpenFlags  flags,
        gboolean                 user_confirmation);
void                   scatola_mime_activate_file                        (GtkWindow               *parent_window,
        ScatolaWindowSlotInfo  *slot_info,
        ScatolaFile            *file,
        const char              *launch_directory,
        ScatolaWindowOpenMode   mode,
        ScatolaWindowOpenFlags  flags);


#endif /* SCATOLA_MIME_ACTIONS_H */
