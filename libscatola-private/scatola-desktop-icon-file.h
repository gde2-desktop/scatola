/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*-

   scatola-desktop-file.h: Subclass of ScatolaFile to implement the
   the case of a desktop icon file

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

#ifndef SCATOLA_DESKTOP_ICON_FILE_H
#define SCATOLA_DESKTOP_ICON_FILE_H

#include <libscatola-private/scatola-file.h>
#include <libscatola-private/scatola-desktop-link.h>

#define SCATOLA_TYPE_DESKTOP_ICON_FILE scatola_desktop_icon_file_get_type()
#define SCATOLA_DESKTOP_ICON_FILE(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), SCATOLA_TYPE_DESKTOP_ICON_FILE, ScatolaDesktopIconFile))
#define SCATOLA_DESKTOP_ICON_FILE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), SCATOLA_TYPE_DESKTOP_ICON_FILE, ScatolaDesktopIconFileClass))
#define SCATOLA_IS_DESKTOP_ICON_FILE(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SCATOLA_TYPE_DESKTOP_ICON_FILE))
#define SCATOLA_IS_DESKTOP_ICON_FILE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), SCATOLA_TYPE_DESKTOP_ICON_FILE))
#define SCATOLA_DESKTOP_ICON_FILE_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), SCATOLA_TYPE_DESKTOP_ICON_FILE, ScatolaDesktopIconFileClass))

typedef struct ScatolaDesktopIconFileDetails ScatolaDesktopIconFileDetails;

typedef struct
{
    ScatolaFile parent_slot;
    ScatolaDesktopIconFileDetails *details;
} ScatolaDesktopIconFile;

typedef struct
{
    ScatolaFileClass parent_slot;
} ScatolaDesktopIconFileClass;

GType   scatola_desktop_icon_file_get_type (void);

ScatolaDesktopIconFile *scatola_desktop_icon_file_new      (ScatolaDesktopLink     *link);
void                     scatola_desktop_icon_file_update   (ScatolaDesktopIconFile *icon_file);
void                     scatola_desktop_icon_file_remove   (ScatolaDesktopIconFile *icon_file);
ScatolaDesktopLink     *scatola_desktop_icon_file_get_link (ScatolaDesktopIconFile *icon_file);

#endif /* SCATOLA_DESKTOP_ICON_FILE_H */
