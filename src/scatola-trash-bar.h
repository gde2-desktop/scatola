/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * Copyright (C) 2006 Paolo Borelli <pborelli@katamail.com>
 *
 * This program is free software; you can redistribute it and/or modify
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
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * Authors: Paolo Borelli <pborelli@katamail.com>
 *
 */

#ifndef __SCATOLA_TRASH_BAR_H
#define __SCATOLA_TRASH_BAR_H

#include "scatola-window.h"

#include <gtk/gtk.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SCATOLA_TYPE_TRASH_BAR         (scatola_trash_bar_get_type ())
#define SCATOLA_TRASH_BAR(o)           (G_TYPE_CHECK_INSTANCE_CAST ((o), SCATOLA_TYPE_TRASH_BAR, ScatolaTrashBar))
#define SCATOLA_TRASH_BAR_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST((k), SCATOLA_TYPE_TRASH_BAR, ScatolaTrashBarClass))
#define SCATOLA_IS_TRASH_BAR(o)        (G_TYPE_CHECK_INSTANCE_TYPE ((o), SCATOLA_TYPE_TRASH_BAR))
#define SCATOLA_IS_TRASH_BAR_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE ((k), SCATOLA_TYPE_TRASH_BAR))
#define SCATOLA_TRASH_BAR_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), SCATOLA_TYPE_TRASH_BAR, ScatolaTrashBarClass))

    typedef struct ScatolaTrashBarPrivate ScatolaTrashBarPrivate;

    typedef struct
    {
#if GTK_CHECK_VERSION (3, 0, 0)
        GtkBox	box;
#else
        GtkHBox	box;
#endif

        ScatolaTrashBarPrivate *priv;
    } ScatolaTrashBar;

    typedef struct
    {
#if GTK_CHECK_VERSION (3, 0, 0)
        GtkBoxClass	    parent_class;
#else
        GtkHBoxClass	    parent_class;
#endif
    } ScatolaTrashBarClass;

    GType		 scatola_trash_bar_get_type	(void) G_GNUC_CONST;

    GtkWidget       *scatola_trash_bar_new         (ScatolaWindow *window);


#ifdef __cplusplus
}
#endif

#endif /* __GS_TRASH_BAR_H */
