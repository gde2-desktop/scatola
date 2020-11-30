/*
 * scatola-freedesktop-dbus: Implementation for the org.freedesktop DBus file-management interfaces
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
 * License along with this program; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 * Authors: Akshay Gupta <kitallis@gmail.com>
 *          Federico Mena Quintero <federico@gnome.org>
 *          Stefano Karapetsas <stefano@karapetsas.com>
 */

#include <config.h>

#include "scatola-application.h"
#include "scatola-freedesktop-dbus.h"
#include "scatola-freedesktop-generated.h"

#include <libscatola-private/scatola-debug-log.h>

#include "file-manager/fm-properties-window.h"

#include <gio/gio.h>

struct _ScatolaFreedesktopDBus {
    GObject parent;

    /* Id from g_dbus_own_name() */
    guint owner_id;

    /* DBus paraphernalia */
    GDBusObjectManagerServer *object_manager;

    /* Our DBus implementation skeleton */
    ScatolaFreedesktopFileManager1 *skeleton;

    /* Scatola application */
    ScatolaApplication *application;
};

struct _ScatolaFreedesktopDBusClass {
    GObjectClass parent_class;
};

G_DEFINE_TYPE (ScatolaFreedesktopDBus, scatola_freedesktop_dbus, G_TYPE_OBJECT);

static gboolean
skeleton_handle_show_items_cb (ScatolaFreedesktopFileManager1 *object,
                               GDBusMethodInvocation *invocation,
                               const gchar *const *uris,
                               const gchar *startup_id,
                               ScatolaFreedesktopDBus *fdb)
{
    ScatolaApplication *application;
    int i;

    application = SCATOLA_APPLICATION (fdb->application);

    for (i = 0; uris[i] != NULL; i++) {
        GFile *file;
        GFile *parent;

        file = g_file_new_for_uri (uris[i]);
        parent = g_file_get_parent (file);

        if (parent != NULL) {
            scatola_application_open_location (application, parent, file, startup_id);
            g_object_unref (parent);
        } else {
            scatola_application_open_location (application, file, NULL, startup_id);
        }

        g_object_unref (file);
    }

    scatola_freedesktop_file_manager1_complete_show_items (object, invocation);
    return TRUE;
}

static gboolean
skeleton_handle_show_folders_cb (ScatolaFreedesktopFileManager1 *object,
                                 GDBusMethodInvocation *invocation,
                                 const gchar *const *uris,
                                 const gchar *startup_id,
                                 ScatolaFreedesktopDBus *fdb)
{
    ScatolaApplication *application;
    int i;

    application = SCATOLA_APPLICATION (fdb->application);

    for (i = 0; uris[i] != NULL; i++) {
        GFile *file;

        file = g_file_new_for_uri (uris[i]);

        scatola_application_open_location (application, file, NULL, startup_id);

        g_object_unref (file);
    }

    scatola_freedesktop_file_manager1_complete_show_folders (object, invocation);
    return TRUE;
}

static gboolean
skeleton_handle_show_item_properties_cb (ScatolaFreedesktopFileManager1 *object,
                     GDBusMethodInvocation *invocation,
                     const gchar *const *uris,
                     const gchar *startup_id,
                     ScatolaFreedesktopDBus *fdb)
{
    ScatolaApplication *application;
    ScatolaWindow *window;
    GList *files;
    int i;

    application = SCATOLA_APPLICATION (fdb->application);
    files = NULL;

    for (i = 0; uris[i] != NULL; i++) {
        files = g_list_prepend (files, scatola_file_get_by_uri (uris[i]));
    }

    files = g_list_reverse (files);

    if (uris[0] != NULL) {
        GFile *file;
        file = g_file_new_for_uri (uris[i]);
        window = scatola_application_get_spatial_window (application,
                                                      NULL,
                                                      startup_id,
                                                      file,
                                                      gdk_screen_get_default (),
                                                      NULL);
        fm_properties_window_present (files, GTK_WIDGET (window));
        g_object_unref (file);
    }

    scatola_file_list_free (files);

    scatola_freedesktop_file_manager1_complete_show_item_properties (object, invocation);
    return TRUE;
}

static void
bus_acquired_cb (GDBusConnection *conn,
                 const gchar     *name,
                 gpointer         user_data)
{
    ScatolaFreedesktopDBus *fdb = user_data;

    scatola_debug_log (FALSE, SCATOLA_DEBUG_LOG_DOMAIN_USER, "Bus acquired at %s", name);

    fdb->object_manager = g_dbus_object_manager_server_new (SCATOLA_FDO_DBUS_PATH);

    fdb->skeleton = scatola_freedesktop_file_manager1_skeleton_new ();

    g_signal_connect (fdb->skeleton, "handle-show-items",
                      G_CALLBACK (skeleton_handle_show_items_cb), fdb);
    g_signal_connect (fdb->skeleton, "handle-show-folders",
                      G_CALLBACK (skeleton_handle_show_folders_cb), fdb);
    g_signal_connect (fdb->skeleton, "handle-show-item-properties",
                      G_CALLBACK (skeleton_handle_show_item_properties_cb), fdb);

    g_dbus_interface_skeleton_export (G_DBUS_INTERFACE_SKELETON (fdb->skeleton), conn, SCATOLA_FDO_DBUS_PATH, NULL);

    g_dbus_object_manager_server_set_connection (fdb->object_manager, conn);
}

static void
name_acquired_cb (GDBusConnection *connection,
                  const gchar     *name,
                  gpointer         user_data)
{
    scatola_debug_log (FALSE, SCATOLA_DEBUG_LOG_DOMAIN_USER, "Acquired the name %s on the session message bus\n", name);
}

static void
name_lost_cb (GDBusConnection *connection,
              const gchar     *name,
              gpointer         user_data)
{
    scatola_debug_log (FALSE, SCATOLA_DEBUG_LOG_DOMAIN_USER, "Lost (or failed to acquire) the name %s on the session message bus\n", name);
}

static void
scatola_freedesktop_dbus_dispose (GObject *object)
{
    ScatolaFreedesktopDBus *fdb = (ScatolaFreedesktopDBus *) object;

    if (fdb->owner_id != 0) {
        g_bus_unown_name (fdb->owner_id);
        fdb->owner_id = 0;
    }

    if (fdb->skeleton != NULL) {
        g_dbus_interface_skeleton_unexport (G_DBUS_INTERFACE_SKELETON (fdb->skeleton));
        g_object_unref (fdb->skeleton);
        fdb->skeleton = NULL;
    }

    g_clear_object (&fdb->object_manager);

    G_OBJECT_CLASS (scatola_freedesktop_dbus_parent_class)->dispose (object);
}

static void
scatola_freedesktop_dbus_class_init (ScatolaFreedesktopDBusClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);

    object_class->dispose = scatola_freedesktop_dbus_dispose;
}

static void
scatola_freedesktop_dbus_init (ScatolaFreedesktopDBus *fdb)
{
    fdb->owner_id = g_bus_own_name (G_BUS_TYPE_SESSION,
                                    SCATOLA_FDO_DBUS_NAME,
                                    G_BUS_NAME_OWNER_FLAGS_NONE,
                                    bus_acquired_cb,
                                    name_acquired_cb,
                                    name_lost_cb,
                                    fdb,
                                    NULL);
}

/* Tries to own the org.freedesktop.FileManager1 service name */
ScatolaFreedesktopDBus *
scatola_freedesktop_dbus_new (ScatolaApplication *application)
{
    ScatolaFreedesktopDBus *fdb = g_object_new (scatola_freedesktop_dbus_get_type (), NULL);
    fdb->application = application;
    return fdb;
}
