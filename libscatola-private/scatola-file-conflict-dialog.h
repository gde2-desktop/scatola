/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/* scatola-file-conflict-dialog: dialog that handles file conflicts
   during transfer operations.

   Copyright (C) 2008, Cosimo Cecchi

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

   Authors: Cosimo Cecchi <cosimoc@gnome.org>
*/

#ifndef SCATOLA_FILE_CONFLICT_DIALOG_H
#define SCATOLA_FILE_CONFLICT_DIALOG_H

#include <glib-object.h>
#include <gio/gio.h>
#include <gtk/gtk.h>

#define SCATOLA_TYPE_FILE_CONFLICT_DIALOG \
	(scatola_file_conflict_dialog_get_type ())
#define SCATOLA_FILE_CONFLICT_DIALOG(o) \
	(G_TYPE_CHECK_INSTANCE_CAST ((o), SCATOLA_TYPE_FILE_CONFLICT_DIALOG,\
				     ScatolaFileConflictDialog))
#define SCATOLA_FILE_CONFLICT_DIALOG_CLASS(k) \
	(G_TYPE_CHECK_CLASS_CAST((k), SCATOLA_TYPE_FILE_CONFLICT_DIALOG,\
				 ScatolaFileConflictDialogClass))
#define SCATOLA_IS_FILE_CONFLICT_DIALOG(o) \
	(G_TYPE_CHECK_INSTANCE_TYPE ((o), SCATOLA_TYPE_FILE_CONFLICT_DIALOG))
#define SCATOLA_IS_FILE_CONFLICT_DIALOG_CLASS(k) \
	(G_TYPE_CHECK_CLASS_TYPE ((k), SCATOLA_TYPE_FILE_CONFLICT_DIALOG))
#define SCATOLA_FILE_CONFLICT_DIALOG_GET_CLASS(o) \
	(G_TYPE_INSTANCE_GET_CLASS ((o), SCATOLA_TYPE_FILE_CONFLICT_DIALOG,\
				    ScatolaFileConflictDialogClass))

typedef struct _ScatolaFileConflictDialog        ScatolaFileConflictDialog;
typedef struct _ScatolaFileConflictDialogClass   ScatolaFileConflictDialogClass;
typedef struct _ScatolaFileConflictDialogDetails ScatolaFileConflictDialogDetails;

struct _ScatolaFileConflictDialog
{
    GtkDialog parent;
    ScatolaFileConflictDialogDetails *details;
};

struct _ScatolaFileConflictDialogClass
{
    GtkDialogClass parent_class;
};

enum
{
    CONFLICT_RESPONSE_SKIP = 1,
    CONFLICT_RESPONSE_REPLACE = 2,
    CONFLICT_RESPONSE_RENAME = 3,
};

GType scatola_file_conflict_dialog_get_type (void) G_GNUC_CONST;

GtkWidget* scatola_file_conflict_dialog_new              (GtkWindow *parent,
        GFile *source,
        GFile *destination,
        GFile *dest_dir);
char*      scatola_file_conflict_dialog_get_new_name     (ScatolaFileConflictDialog *dialog);
gboolean   scatola_file_conflict_dialog_get_apply_to_all (ScatolaFileConflictDialog *dialog);

#endif /* SCATOLA_FILE_CONFLICT_DIALOG_H */
