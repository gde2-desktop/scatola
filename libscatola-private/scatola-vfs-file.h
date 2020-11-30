/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*-

   scatola-vfs-file.h: Subclass of ScatolaFile to implement the
   the case of a VFS file.

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

#ifndef SCATOLA_VFS_FILE_H
#define SCATOLA_VFS_FILE_H

#include <libscatola-private/scatola-file.h>

#define SCATOLA_TYPE_VFS_FILE scatola_vfs_file_get_type()
#define SCATOLA_VFS_FILE(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), SCATOLA_TYPE_VFS_FILE, ScatolaVFSFile))
#define SCATOLA_VFS_FILE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), SCATOLA_TYPE_VFS_FILE, ScatolaVFSFileClass))
#define SCATOLA_IS_VFS_FILE(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SCATOLA_TYPE_VFS_FILE))
#define SCATOLA_IS_VFS_FILE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), SCATOLA_TYPE_VFS_FILE))
#define SCATOLA_VFS_FILE_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), SCATOLA_TYPE_VFS_FILE, ScatolaVFSFileClass))

typedef struct ScatolaVFSFileDetails ScatolaVFSFileDetails;

typedef struct
{
    ScatolaFile parent_slot;
} ScatolaVFSFile;

typedef struct
{
    ScatolaFileClass parent_slot;
} ScatolaVFSFileClass;

GType   scatola_vfs_file_get_type (void);

#endif /* SCATOLA_VFS_FILE_H */
