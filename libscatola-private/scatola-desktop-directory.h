/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*-

   scatola-desktop-directory.h: Subclass of ScatolaDirectory to implement
   a virtual directory consisting of the desktop directory and the desktop
   icons

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

#ifndef SCATOLA_DESKTOP_DIRECTORY_H
#define SCATOLA_DESKTOP_DIRECTORY_H

#include <libscatola-private/scatola-directory.h>

#define SCATOLA_TYPE_DESKTOP_DIRECTORY scatola_desktop_directory_get_type()
#define SCATOLA_DESKTOP_DIRECTORY(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), SCATOLA_TYPE_DESKTOP_DIRECTORY, ScatolaDesktopDirectory))
#define SCATOLA_DESKTOP_DIRECTORY_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), SCATOLA_TYPE_DESKTOP_DIRECTORY, ScatolaDesktopDirectoryClass))
#define SCATOLA_IS_DESKTOP_DIRECTORY(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SCATOLA_TYPE_DESKTOP_DIRECTORY))
#define SCATOLA_IS_DESKTOP_DIRECTORY_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), SCATOLA_TYPE_DESKTOP_DIRECTORY))
#define SCATOLA_DESKTOP_DIRECTORY_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), SCATOLA_TYPE_DESKTOP_DIRECTORY, ScatolaDesktopDirectoryClass))

typedef struct ScatolaDesktopDirectoryDetails ScatolaDesktopDirectoryDetails;

typedef struct
{
    ScatolaDirectory parent_slot;
    ScatolaDesktopDirectoryDetails *details;
} ScatolaDesktopDirectory;

typedef struct
{
    ScatolaDirectoryClass parent_slot;

} ScatolaDesktopDirectoryClass;

GType   scatola_desktop_directory_get_type             (void);
ScatolaDirectory * scatola_desktop_directory_get_real_directory   (ScatolaDesktopDirectory *desktop_directory);

#endif /* SCATOLA_DESKTOP_DIRECTORY_H */
