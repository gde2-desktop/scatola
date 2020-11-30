/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */
/*
 * Scatola
 *
 * Copyright (C) 2003 Red Hat, Inc.
 * Copyright (C) 2010 Cosimo Cecchi <cosimoc@gnome.org>
 *
 * Scatola is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * Scatola is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; see the file COPYING.  If not,
 * write to the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#ifndef SCATOLA_CONNECT_SERVER_DIALOG_H
#define SCATOLA_CONNECT_SERVER_DIALOG_H

#include <gio/gio.h>
#include <gtk/gtk.h>

#include "scatola-window.h"

#define SCATOLA_TYPE_CONNECT_SERVER_DIALOG\
	(scatola_connect_server_dialog_get_type ())
#define SCATOLA_CONNECT_SERVER_DIALOG(obj)\
        (G_TYPE_CHECK_INSTANCE_CAST ((obj), SCATOLA_TYPE_CONNECT_SERVER_DIALOG,\
				     ScatolaConnectServerDialog))
#define SCATOLA_CONNECT_SERVER_DIALOG_CLASS(klass)\
	(G_TYPE_CHECK_CLASS_CAST ((klass), SCATOLA_TYPE_CONNECT_SERVER_DIALOG,\
				  ScatolaConnectServerDialogClass))
#define SCATOLA_IS_CONNECT_SERVER_DIALOG(obj)\
	(G_TYPE_INSTANCE_CHECK_TYPE ((obj), SCATOLA_TYPE_CONNECT_SERVER_DIALOG)

typedef struct _ScatolaConnectServerDialog ScatolaConnectServerDialog;
typedef struct _ScatolaConnectServerDialogClass ScatolaConnectServerDialogClass;
typedef struct _ScatolaConnectServerDialogDetails ScatolaConnectServerDialogDetails;

struct _ScatolaConnectServerDialog
{
    GtkDialog parent;
    ScatolaConnectServerDialogDetails *details;
};

struct _ScatolaConnectServerDialogClass
{
    GtkDialogClass parent_class;
};

GType scatola_connect_server_dialog_get_type (void);

GtkWidget* scatola_connect_server_dialog_new (ScatolaWindow *window);

void scatola_connect_server_dialog_display_location_async (ScatolaConnectServerDialog *self,
							    ScatolaApplication *application,
							    GFile *location,
							    GAsyncReadyCallback callback,
							    gpointer user_data);
gboolean scatola_connect_server_dialog_display_location_finish (ScatolaConnectServerDialog *self,
								 GAsyncResult *result,
								 GError **error);

void scatola_connect_server_dialog_fill_details_async (ScatolaConnectServerDialog *self,
							GMountOperation *operation,
							const gchar *default_user,
							const gchar *default_domain,
							GAskPasswordFlags flags,
							GAsyncReadyCallback callback,
							gpointer user_data);
gboolean scatola_connect_server_dialog_fill_details_finish (ScatolaConnectServerDialog *self,
							     GAsyncResult *result);

#endif /* SCATOLA_CONNECT_SERVER_DIALOG_H */
