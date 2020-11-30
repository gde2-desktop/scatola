/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/*
   scatola-open-with-dialog.c: an open-with dialog

   Copyright (C) 2004 Novell, Inc.

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

   Authors: Dave Camp <dave@novell.com>
*/

#ifndef SCATOLA_OPEN_WITH_DIALOG_H
#define SCATOLA_OPEN_WITH_DIALOG_H

#include <gtk/gtk.h>
#include <gio/gio.h>

#define SCATOLA_TYPE_OPEN_WITH_DIALOG         (scatola_open_with_dialog_get_type ())
#define SCATOLA_OPEN_WITH_DIALOG(obj)         (G_TYPE_CHECK_INSTANCE_CAST ((obj), SCATOLA_TYPE_OPEN_WITH_DIALOG, ScatolaOpenWithDialog))
#define SCATOLA_OPEN_WITH_DIALOG_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), SCATOLA_TYPE_OPEN_WITH_DIALOG, ScatolaOpenWithDialogClass))
#define SCATOLA_IS_OPEN_WITH_DIALOG(obj)      (G_TYPE_INSTANCE_CHECK_TYPE ((obj), SCATOLA_TYPE_OPEN_WITH_DIALOG)

typedef struct _ScatolaOpenWithDialog        ScatolaOpenWithDialog;
typedef struct _ScatolaOpenWithDialogClass   ScatolaOpenWithDialogClass;
typedef struct _ScatolaOpenWithDialogDetails ScatolaOpenWithDialogDetails;

struct _ScatolaOpenWithDialog
{
    GtkDialog parent;
    ScatolaOpenWithDialogDetails *details;
};

struct _ScatolaOpenWithDialogClass
{
    GtkDialogClass parent_class;

    void (*application_selected) (ScatolaOpenWithDialog *dialog,
                                  GAppInfo *application);
};

GType      scatola_open_with_dialog_get_type (void);
GtkWidget* scatola_open_with_dialog_new      (const char *uri,
        const char *mime_type,
        const char *extension);
GtkWidget* scatola_add_application_dialog_new (const char *uri,
        const char *mime_type);
GtkWidget* scatola_add_application_dialog_new_for_multiple_files (const char *extension,
        const char *mime_type);



#endif /* SCATOLA_OPEN_WITH_DIALOG_H */
