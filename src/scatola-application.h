/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/*
 * Scatola
 *
 * Copyright (C) 2000 Red Hat, Inc.
 *
 * Scatola is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * Scatola is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

/* scatola-application.h
 */

#ifndef SCATOLA_APPLICATION_H
#define SCATOLA_APPLICATION_H

#include <gdk/gdk.h>
#include <gio/gio.h>
#include <unique/unique.h>
#include <libegg/eggsmclient.h>

#define SCATOLA_DESKTOP_ICON_VIEW_IID "OAFIID:Scatola_File_Manager_Desktop_Icon_View"

#define SCATOLA_TYPE_APPLICATION \
	scatola_application_get_type()
#define SCATOLA_APPLICATION(obj) \
	(G_TYPE_CHECK_INSTANCE_CAST((obj), SCATOLA_TYPE_APPLICATION, ScatolaApplication))
#define SCATOLA_APPLICATION_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_CAST((klass), SCATOLA_TYPE_APPLICATION, ScatolaApplicationClass))
#define SCATOLA_IS_APPLICATION(obj) \
	(G_TYPE_CHECK_INSTANCE_TYPE((obj), SCATOLA_TYPE_APPLICATION))
#define SCATOLA_IS_APPLICATION_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_TYPE((klass), SCATOLA_TYPE_APPLICATION))
#define SCATOLA_APPLICATION_GET_CLASS(obj) \
	(G_TYPE_INSTANCE_GET_CLASS((obj), SCATOLA_TYPE_APPLICATION, ScatolaApplicationClass))

#ifndef SCATOLA_WINDOW_DEFINED
#define SCATOLA_WINDOW_DEFINED
typedef struct ScatolaWindow ScatolaWindow;
#endif

#ifndef SCATOLA_SPATIAL_WINDOW_DEFINED
#define SCATOLA_SPATIAL_WINDOW_DEFINED
typedef struct _ScatolaSpatialWindow ScatolaSpatialWindow;
#endif

typedef struct ScatolaShell ScatolaShell;

typedef struct
{
    GObject parent;
    UniqueApp* unique_app;
    EggSMClient* smclient;
    GVolumeMonitor* volume_monitor;
    unsigned int automount_idle_id;
    gboolean screensaver_active;
    guint ss_watch_id;
    GDBusProxy *ss_proxy;
    GList *volume_queue;
} ScatolaApplication;

typedef struct
{
    GObjectClass parent_class;
} ScatolaApplicationClass;

GType                scatola_application_get_type          (void);
ScatolaApplication *scatola_application_new               (void);
void                 scatola_application_startup           (ScatolaApplication *application,
        gboolean             kill_shell,
        gboolean             no_default_window,
        gboolean             no_desktop,
        gboolean             browser_window,
        const char          *default_geometry,
        char               **urls);
GList *              scatola_application_get_window_list           (void);
GList *              scatola_application_get_spatial_window_list    (void);
unsigned int         scatola_application_get_n_windows            (void);

ScatolaWindow *     scatola_application_get_spatial_window     (ScatolaApplication *application,
        ScatolaWindow      *requesting_window,
        const char      *startup_id,
        GFile           *location,
        GdkScreen       *screen,
        gboolean        *existing);

ScatolaWindow *     scatola_application_create_navigation_window     (ScatolaApplication *application,
        const char          *startup_id,
        GdkScreen           *screen);

void scatola_application_close_all_navigation_windows (void);
void scatola_application_close_parent_windows     (ScatolaSpatialWindow *window);
void scatola_application_close_all_spatial_windows  (void);
void scatola_application_open_desktop      (ScatolaApplication *application);
void scatola_application_close_desktop     (void);
gboolean scatola_application_save_accel_map    (gpointer data);
void scatola_application_open_location (ScatolaApplication *application,
        GFile *location,
        GFile *selection,
        const char *startup_id);

#endif /* SCATOLA_APPLICATION_H */
