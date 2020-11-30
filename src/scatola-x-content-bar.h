/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * Copyright (C) 2008 Red Hat, Inc.
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
 * Authors: David Zeuthen <davidz@redhat.com>
 *          Paolo Borelli <pborelli@katamail.com>
 *
 */

#ifndef __SCATOLA_X_CONTENT_BAR_H
#define __SCATOLA_X_CONTENT_BAR_H

#include <gtk/gtk.h>
#include <gio/gio.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SCATOLA_TYPE_X_CONTENT_BAR         (scatola_x_content_bar_get_type ())
#define SCATOLA_X_CONTENT_BAR(o)           (G_TYPE_CHECK_INSTANCE_CAST ((o), SCATOLA_TYPE_X_CONTENT_BAR, ScatolaXContentBar))
#define SCATOLA_X_CONTENT_BAR_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST((k), SCATOLA_TYPE_X_CONTENT_BAR, ScatolaXContentBarClass))
#define SCATOLA_IS_X_CONTENT_BAR(o)        (G_TYPE_CHECK_INSTANCE_TYPE ((o), SCATOLA_TYPE_X_CONTENT_BAR))
#define SCATOLA_IS_X_CONTENT_BAR_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE ((k), SCATOLA_TYPE_X_CONTENT_BAR))
#define SCATOLA_X_CONTENT_BAR_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), SCATOLA_TYPE_X_CONTENT_BAR, ScatolaXContentBarClass))

    typedef struct ScatolaXContentBarPrivate ScatolaXContentBarPrivate;

    typedef struct
    {
#if GTK_CHECK_VERSION (3, 0, 0)
        GtkBox	box;
#else
        GtkHBox	box;
#endif

        ScatolaXContentBarPrivate *priv;
    } ScatolaXContentBar;

    typedef struct
    {
#if GTK_CHECK_VERSION (3, 0, 0)
        GtkBoxClass	    parent_class;
#else
        GtkHBoxClass	    parent_class;
#endif
    } ScatolaXContentBarClass;

    GType		 scatola_x_content_bar_get_type	(void) G_GNUC_CONST;

    GtkWidget	*scatola_x_content_bar_new		   (GMount              *mount,
            const char          *x_content_type);
    const char      *scatola_x_content_bar_get_x_content_type (ScatolaXContentBar *bar);
    void             scatola_x_content_bar_set_x_content_type (ScatolaXContentBar *bar,
            const char          *x_content_type);
    void             scatola_x_content_bar_set_mount          (ScatolaXContentBar *bar,
            GMount              *mount);
    GMount          *scatola_x_content_bar_get_mount          (ScatolaXContentBar *bar);

#ifdef __cplusplus
}
#endif

#endif /* __SCATOLA_X_CONTENT_BAR_H */
