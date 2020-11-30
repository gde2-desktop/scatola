/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*-

   scatola-vfs-directory.h: Subclass of ScatolaDirectory to implement the
   the case of a VFS directory.

   Copyright (C) 1999, 2000 Eazel, Inc.

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

   Author: Darin Adler <darin@bentspoon.com>
*/

#ifndef SCATOLA_VFS_DIRECTORY_H
#define SCATOLA_VFS_DIRECTORY_H

#include <libscatola-private/scatola-directory.h>

#define SCATOLA_TYPE_VFS_DIRECTORY scatola_vfs_directory_get_type()
#define SCATOLA_VFS_DIRECTORY(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), SCATOLA_TYPE_VFS_DIRECTORY, ScatolaVFSDirectory))
#define SCATOLA_VFS_DIRECTORY_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), SCATOLA_TYPE_VFS_DIRECTORY, ScatolaVFSDirectoryClass))
#define SCATOLA_IS_VFS_DIRECTORY(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SCATOLA_TYPE_VFS_DIRECTORY))
#define SCATOLA_IS_VFS_DIRECTORY_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), SCATOLA_TYPE_VFS_DIRECTORY))
#define SCATOLA_VFS_DIRECTORY_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), SCATOLA_TYPE_VFS_DIRECTORY, ScatolaVFSDirectoryClass))

typedef struct ScatolaVFSDirectoryDetails ScatolaVFSDirectoryDetails;

typedef struct
{
    ScatolaDirectory parent_slot;
} ScatolaVFSDirectory;

typedef struct
{
    ScatolaDirectoryClass parent_slot;
} ScatolaVFSDirectoryClass;

GType   scatola_vfs_directory_get_type (void);

#endif /* SCATOLA_VFS_DIRECTORY_H */
