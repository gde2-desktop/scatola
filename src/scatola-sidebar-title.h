/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/*
 * Scatola
 *
 * Copyright (C) 2000 Eazel, Inc.
 *
 * Scatola is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * Author: Andy Hertzfeld <andy@eazel.com>
 */

/*
 * This is the header file for the sidebar title, which is part of the sidebar.
 */

#ifndef SCATOLA_SIDEBAR_TITLE_H
#define SCATOLA_SIDEBAR_TITLE_H

#include <gtk/gtk.h>
#include <eel/eel-background.h>
#include <libscatola-private/scatola-file.h>

#define SCATOLA_TYPE_SIDEBAR_TITLE scatola_sidebar_title_get_type()
#define SCATOLA_SIDEBAR_TITLE(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), SCATOLA_TYPE_SIDEBAR_TITLE, ScatolaSidebarTitle))
#define SCATOLA_SIDEBAR_TITLE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), SCATOLA_TYPE_SIDEBAR_TITLE, ScatolaSidebarTitleClass))
#define SCATOLA_IS_SIDEBAR_TITLE(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SCATOLA_TYPE_SIDEBAR_TITLE))
#define SCATOLA_IS_SIDEBAR_TITLE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), SCATOLA_TYPE_SIDEBAR_TITLE))
#define SCATOLA_SIDEBAR_TITLE_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), SCATOLA_TYPE_SIDEBAR_TITLE, ScatolaSidebarTitleClass))

typedef struct ScatolaSidebarTitleDetails ScatolaSidebarTitleDetails;

typedef struct
{
#if GTK_CHECK_VERSION (3, 0, 0)
    GtkBox box;
#else
    GtkVBox box;
#endif
    ScatolaSidebarTitleDetails *details;
} ScatolaSidebarTitle;

typedef struct
{
#if GTK_CHECK_VERSION (3, 0, 0)
    GtkBoxClass parent_class;
#else
    GtkVBoxClass parent_class;
#endif
} ScatolaSidebarTitleClass;

GType      scatola_sidebar_title_get_type          (void);
GtkWidget *scatola_sidebar_title_new               (void);
void       scatola_sidebar_title_set_file          (ScatolaSidebarTitle *sidebar_title,
        ScatolaFile         *file,
        const char           *initial_text);
void       scatola_sidebar_title_set_text          (ScatolaSidebarTitle *sidebar_title,
        const char           *new_title);
char *     scatola_sidebar_title_get_text          (ScatolaSidebarTitle *sidebar_title);
gboolean   scatola_sidebar_title_hit_test_icon     (ScatolaSidebarTitle *sidebar_title,
        int                   x,
        int                   y);
void       scatola_sidebar_title_select_text_color (ScatolaSidebarTitle *sidebar_title,
        					 EelBackground        *background);

#endif /* SCATOLA_SIDEBAR_TITLE_H */
