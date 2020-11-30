/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/*
 * Scatola
 *
 * Copyright (C) 1999, 2000 Eazel, Inc.
 *
 * Scatola is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Scatola is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 *
 *  This is the header file for the index panel widget, which displays oversidebar information
 *  in a vertical panel and hosts the meta-sidebars.
 */

#ifndef SCATOLA_EMBLEM_SIDEBAR_H
#define SCATOLA_EMBLEM_SIDEBAR_H

#include <gtk/gtk.h>

#define SCATOLA_TYPE_EMBLEM_SIDEBAR scatola_emblem_sidebar_get_type()
#define SCATOLA_EMBLEM_SIDEBAR(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), SCATOLA_TYPE_EMBLEM_SIDEBAR, ScatolaEmblemSidebar))
#define SCATOLA_EMBLEM_SIDEBAR_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), SCATOLA_TYPE_EMBLEM_SIDEBAR, ScatolaEmblemSidebarClass))
#define SCATOLA_IS_EMBLEM_SIDEBAR(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SCATOLA_TYPE_EMBLEM_SIDEBAR))
#define SCATOLA_IS_EMBLEM_SIDEBAR_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), SCATOLA_TYPE_EMBLEM_SIDEBAR))
#define SCATOLA_EMBLEM_SIDEBAR_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), SCATOLA_TYPE_EMBLEM_SIDEBAR, ScatolaEmblemSidebarClass))

#define SCATOLA_EMBLEM_SIDEBAR_ID "emblems"

typedef struct ScatolaEmblemSidebarDetails ScatolaEmblemSidebarDetails;

typedef struct
{
#if GTK_CHECK_VERSION (3, 0, 0)
    GtkBox parent_slot;
#else
    GtkVBox parent_slot;
#endif
    ScatolaEmblemSidebarDetails *details;
} ScatolaEmblemSidebar;

typedef struct
{
#if GTK_CHECK_VERSION (3, 0, 0)
    GtkBoxClass parent_slot;
#else
    GtkVBoxClass parent_slot;
#endif

} ScatolaEmblemSidebarClass;

GType	scatola_emblem_sidebar_get_type     (void);
void    scatola_emblem_sidebar_register     (void);

#endif /* SCATOLA_EMBLEM_SIDEBAR_H */
