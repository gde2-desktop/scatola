/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/*
 * Scatola
 *
 * Copyright (C) 2000 Eazel, Inc.
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
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * Authors: Darin Adler <darin@bentspoon.com>
 */

#include <config.h>
#include "scatola-desktop-window.h"
#include "scatola-window-private.h"
#include "scatola-actions.h"

#include <X11/Xatom.h>
#include <gdk/gdkx.h>
#include <gtk/gtk.h>
#include <eel/eel-background.h>
#include <eel/eel-vfs-extensions.h>
#include <libscatola-private/scatola-file-utilities.h>
#include <libscatola-private/scatola-icon-names.h>
#include <gio/gio.h>
#include <glib/gi18n.h>    

struct ScatolaDesktopWindowDetails
{
    gulong size_changed_id;

    gboolean loaded;
};

G_DEFINE_TYPE (ScatolaDesktopWindow, scatola_desktop_window,
               SCATOLA_TYPE_SPATIAL_WINDOW);

static void
scatola_desktop_window_init (ScatolaDesktopWindow *window)
{
    GtkAction *action;
    AtkObject *accessible;

    window->details = G_TYPE_INSTANCE_GET_PRIVATE (window, SCATOLA_TYPE_DESKTOP_WINDOW,
                                                   ScatolaDesktopWindowDetails);

#if GTK_CHECK_VERSION(3, 0, 0)
    GtkStyleContext *context;

    context = gtk_widget_get_style_context (GTK_WIDGET (window));
    gtk_style_context_add_class (context, "scatola-desktop-window");
#endif

    gtk_window_move (GTK_WINDOW (window), 0, 0);

    /* shouldn't really be needed given our semantic type
     * of _NET_WM_TYPE_DESKTOP, but why not
     */
    gtk_window_set_resizable (GTK_WINDOW (window),
                              FALSE);

    g_object_set_data (G_OBJECT (window), "is_desktop_window",
                       GINT_TO_POINTER (1));

    gtk_widget_hide (SCATOLA_WINDOW (window)->details->statusbar);
    gtk_widget_hide (SCATOLA_WINDOW (window)->details->menubar);

    /* Don't allow close action on desktop */
    action = gtk_action_group_get_action (SCATOLA_WINDOW (window)->details->main_action_group,
                                          SCATOLA_ACTION_CLOSE);
    gtk_action_set_sensitive (action, FALSE);

    /* Set the accessible name so that it doesn't inherit the cryptic desktop URI. */
    accessible = gtk_widget_get_accessible (GTK_WIDGET (window));

    if (accessible) {
        atk_object_set_name (accessible, _("Desktop"));
    }
}

static gint
scatola_desktop_window_delete_event (ScatolaDesktopWindow *window)
{
    /* Returning true tells GTK+ not to delete the window. */
    return TRUE;
}

void
scatola_desktop_window_update_directory (ScatolaDesktopWindow *window)
{
    GFile *location;

    g_assert (SCATOLA_IS_DESKTOP_WINDOW (window));

    location = g_file_new_for_uri (EEL_DESKTOP_URI);
    scatola_window_go_to (SCATOLA_WINDOW (window), location);
    window->details->loaded = TRUE;

    g_object_unref (location);
}

static void
scatola_desktop_window_screen_size_changed (GdkScreen             *screen,
        ScatolaDesktopWindow *window)
{
    int width_request, height_request;

    width_request = gdk_screen_get_width (screen);
    height_request = gdk_screen_get_height (screen);

    g_object_set (window,
                  "width_request", width_request,
                  "height_request", height_request,
                  NULL);
}

ScatolaDesktopWindow *
scatola_desktop_window_new (ScatolaApplication *application,
                         GdkScreen           *screen)
{
    ScatolaDesktopWindow *window;
    int width_request, height_request;

    width_request = gdk_screen_get_width (screen);
    height_request = gdk_screen_get_height (screen);

    window = SCATOLA_DESKTOP_WINDOW
             (gtk_widget_new (scatola_desktop_window_get_type(),
                              "app", application,
                              "width_request", width_request,
                              "height_request", height_request,
                              "screen", screen,
                              NULL));
    /* Stop wrong desktop window size in GTK 3.20*/
    /* We don't want to set a default size, which the parent does, since this */
    /* will cause the desktop window to open at the wrong size in gtk 3.20 */
#if GTK_CHECK_VERSION (3, 19, 0) 
    gtk_window_set_default_size (GTK_WINDOW (window), -1, -1);
#endif
    /* Special sawmill setting*/
    gtk_window_set_wmclass (GTK_WINDOW (window), "desktop_window", "Scatola");

    g_signal_connect (window, "delete_event", G_CALLBACK (scatola_desktop_window_delete_event), NULL);

    /* Point window at the desktop folder.
     * Note that scatola_desktop_window_init is too early to do this.
     */
    scatola_desktop_window_update_directory (window);

    return window;
}

static void
map (GtkWidget *widget)
{
    /* Chain up to realize our children */
    GTK_WIDGET_CLASS (scatola_desktop_window_parent_class)->map (widget);
    gdk_window_lower (gtk_widget_get_window (widget));
}

static void
unrealize (GtkWidget *widget)
{
    ScatolaDesktopWindow *window;
    ScatolaDesktopWindowDetails *details;
    GdkWindow *root_window;

    window = SCATOLA_DESKTOP_WINDOW (widget);
    details = window->details;

    root_window = gdk_screen_get_root_window (
                      gtk_window_get_screen (GTK_WINDOW (window)));

    gdk_property_delete (root_window,
                         gdk_atom_intern ("SCATOLA_DESKTOP_WINDOW_ID", TRUE));

    if (details->size_changed_id != 0) {
        g_signal_handler_disconnect (gtk_window_get_screen (GTK_WINDOW (window)),
                         details->size_changed_id);
        details->size_changed_id = 0;
    }

    GTK_WIDGET_CLASS (scatola_desktop_window_parent_class)->unrealize (widget);
}

static void
set_wmspec_desktop_hint (GdkWindow *window)
{
    GdkAtom atom;

    atom = gdk_atom_intern ("_NET_WM_WINDOW_TYPE_DESKTOP", FALSE);

    gdk_property_change (window,
                         gdk_atom_intern ("_NET_WM_WINDOW_TYPE", FALSE),
                         gdk_x11_xatom_to_atom (XA_ATOM), 32,
                         GDK_PROP_MODE_REPLACE, (guchar *) &atom, 1);
}

static void
set_desktop_window_id (ScatolaDesktopWindow *window,
                       GdkWindow             *gdkwindow)
{
    /* Tuck the desktop windows xid in the root to indicate we own the desktop.
     */
    Window window_xid;
    GdkWindow *root_window;

    root_window = gdk_screen_get_root_window (
                      gtk_window_get_screen (GTK_WINDOW (window)));

#if GTK_CHECK_VERSION (3, 0, 0)
    window_xid = GDK_WINDOW_XID (gdkwindow);
#else
    window_xid = GDK_WINDOW_XWINDOW (gdkwindow);
#endif

    gdk_property_change (root_window,
                         gdk_atom_intern ("SCATOLA_DESKTOP_WINDOW_ID", FALSE),
                         gdk_x11_xatom_to_atom (XA_WINDOW), 32,
                         GDK_PROP_MODE_REPLACE, (guchar *) &window_xid, 1);
}

static void
realize (GtkWidget *widget)
{
    ScatolaDesktopWindow *window;
    ScatolaDesktopWindowDetails *details;
    window = SCATOLA_DESKTOP_WINDOW (widget);
    details = window->details;

    /* Make sure we get keyboard events */
    gtk_widget_set_events (widget, gtk_widget_get_events (widget)
                           | GDK_KEY_PRESS_MASK | GDK_KEY_RELEASE_MASK);
    /* Do the work of realizing. */
    GTK_WIDGET_CLASS (scatola_desktop_window_parent_class)->realize (widget);

    /* This is the new way to set up the desktop window */
    set_wmspec_desktop_hint (gtk_widget_get_window (widget));

    set_desktop_window_id (window, gtk_widget_get_window (widget));

    details->size_changed_id =
        g_signal_connect (gtk_window_get_screen (GTK_WINDOW (window)), "size_changed",
                          G_CALLBACK (scatola_desktop_window_screen_size_changed), window);
}

#if GTK_CHECK_VERSION (3, 22, 0)
static gboolean
draw (GtkWidget *widget,
      cairo_t   *cr)
{
    eel_background_draw (widget, cr);

    return GTK_WIDGET_CLASS (scatola_desktop_window_parent_class)->draw (widget, cr);
}
#endif

static char *
real_get_title (ScatolaWindow *window)
{
    return g_strdup (_("Desktop"));
}

static ScatolaIconInfo *
real_get_icon (ScatolaWindow *window,
               ScatolaWindowSlot *slot)
{
    return scatola_icon_info_lookup_from_name (SCATOLA_ICON_DESKTOP, 48);
}

static void
scatola_desktop_window_class_init (ScatolaDesktopWindowClass *klass)
{
    GtkWidgetClass *wclass = GTK_WIDGET_CLASS (klass);
    ScatolaWindowClass *nclass = SCATOLA_WINDOW_CLASS (klass);

    wclass->realize = realize;
    wclass->unrealize = unrealize;
    wclass->map = map;
#if GTK_CHECK_VERSION (3, 22, 0)
    wclass->draw = draw;
#endif
    nclass->window_type = SCATOLA_WINDOW_DESKTOP;
    nclass->get_title = real_get_title;
    nclass->get_icon = real_get_icon;

    g_type_class_add_private (klass, sizeof (ScatolaDesktopWindowDetails));
}

gboolean
scatola_desktop_window_loaded (ScatolaDesktopWindow *window)
{
    return window->details->loaded;
}