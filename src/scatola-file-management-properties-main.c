/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/* scatola-file-management-properties-main.c - Start the scatola-file-management preference dialog.

   Copyright (C) 2002 Jan Arne Petersen

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

   Authors: Jan Arne Petersen <jpetersen@uni-bonn.de>
*/

#include <config.h>

#include <gtk/gtk.h>
#include <glib/gi18n.h>

#include <libscatola-private/scatola-global-preferences.h>
#include <libscatola-private/scatola-module.h>

#include "scatola-file-management-properties.h"

static void
scatola_file_management_properties_main_close_callback (GtkDialog *dialog,
        int response_id)
{
    if (response_id == GTK_RESPONSE_CLOSE)
    {
        gtk_main_quit ();
    }
}

int
main (int argc, char *argv[])
{
    bindtextdomain (GETTEXT_PACKAGE, GDE2LOCALEDIR);
    bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
    textdomain (GETTEXT_PACKAGE);

    gtk_init (&argc, &argv);

    scatola_global_preferences_init ();

    scatola_module_setup ();

    scatola_file_management_properties_dialog_show (G_CALLBACK (scatola_file_management_properties_main_close_callback), NULL);

    gtk_main ();

    return 0;
}
