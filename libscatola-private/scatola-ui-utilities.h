/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/* scatola-ui-utilities.h - helper functions for GtkUIManager stuff

   Copyright (C) 2004 Red Hat, Inc.

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

   Authors: Alexander Larsson <alexl@redhat.com>
*/
#ifndef SCATOLA_UI_UTILITIES_H
#define SCATOLA_UI_UTILITIES_H

#include <gtk/gtk.h>
#include <libscatola-extension/scatola-menu-item.h>

char *      scatola_get_ui_directory              (void);
char *      scatola_ui_file                       (const char        *partial_path);
void        scatola_ui_unmerge_ui                 (GtkUIManager      *ui_manager,
        guint             *merge_id,
        GtkActionGroup   **action_group);
void        scatola_ui_prepare_merge_ui           (GtkUIManager      *ui_manager,
        const char        *name,
        guint             *merge_id,
        GtkActionGroup   **action_group);
GtkAction * scatola_action_from_menu_item         (ScatolaMenuItem  *item);
GtkAction * scatola_toolbar_action_from_menu_item (ScatolaMenuItem  *item);
const char *scatola_ui_string_get                 (const char        *filename);
void   scatola_ui_frame_image                     (GdkPixbuf        **pixbuf);

#endif /* SCATOLA_UI_UTILITIES_H */
