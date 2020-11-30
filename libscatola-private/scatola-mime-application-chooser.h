/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/*
   scatola-mime-application-chooser.c: Manages applications for mime types

   Copyright (C) 2004 Novell, Inc.

   The Gde2 Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   The Gde2 Library is distributed in the hope that it will be useful,
   but APPLICATIONOUT ANY WARRANTY; applicationout even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along application the Gde2 Library; see the file COPYING.LIB.  If not,
   write to the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
   Boston, MA 02110-1301, USA.

   Authors: Dave Camp <dave@novell.com>
*/

#ifndef SCATOLA_MIME_APPLICATION_CHOOSER_H
#define SCATOLA_MIME_APPLICATION_CHOOSER_H

#include <gtk/gtk.h>

#define SCATOLA_TYPE_MIME_APPLICATION_CHOOSER         (scatola_mime_application_chooser_get_type ())
#define SCATOLA_MIME_APPLICATION_CHOOSER(obj)         (G_TYPE_CHECK_INSTANCE_CAST ((obj), SCATOLA_TYPE_MIME_APPLICATION_CHOOSER, ScatolaMimeApplicationChooser))
#define SCATOLA_MIME_APPLICATION_CHOOSER_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), SCATOLA_TYPE_MIME_APPLICATION_CHOOSER, ScatolaMimeApplicationChooserClass))
#define SCATOLA_IS_MIME_APPLICATION_CHOOSER(obj)      (G_TYPE_INSTANCE_CHECK_TYPE ((obj), SCATOLA_TYPE_MIME_APPLICATION_CHOOSER)

typedef struct _ScatolaMimeApplicationChooser        ScatolaMimeApplicationChooser;
typedef struct _ScatolaMimeApplicationChooserClass   ScatolaMimeApplicationChooserClass;
typedef struct _ScatolaMimeApplicationChooserDetails ScatolaMimeApplicationChooserDetails;

struct _ScatolaMimeApplicationChooser
{
#if GTK_CHECK_VERSION (3, 0, 0)
    GtkBox parent;
#else
    GtkVBox parent;
#endif
    ScatolaMimeApplicationChooserDetails *details;
};

struct _ScatolaMimeApplicationChooserClass
{
#if GTK_CHECK_VERSION (3, 0, 0)
    GtkBoxClass parent_class;
#else
    GtkVBoxClass parent_class;
#endif
};

GType      scatola_mime_application_chooser_get_type (void);
GtkWidget* scatola_mime_application_chooser_new      (const char *uri,
        const char *mime_type);
GtkWidget* scatola_mime_application_chooser_new_for_multiple_files (GList *uris,
        const char *mime_type);

#endif /* SCATOLA_MIME_APPLICATION_CHOOSER_H */
