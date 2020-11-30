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

#include "config.h"

#include <glib/gi18n-lib.h>
#include <gtk/gtk.h>

#include "scatola-trash-bar.h"

#include "scatola-window.h"
#include <libscatola-private/scatola-file-operations.h>
#include <libscatola-private/scatola-file-utilities.h>
#include <libscatola-private/scatola-file.h>
#include <libscatola-private/scatola-trash-monitor.h>

#define SCATOLA_TRASH_BAR_GET_PRIVATE(o)\
	(G_TYPE_INSTANCE_GET_PRIVATE ((o), SCATOLA_TYPE_TRASH_BAR, ScatolaTrashBarPrivate))

enum
{
    PROP_WINDOW = 1,
    NUM_PROPERTIES
};

struct ScatolaTrashBarPrivate
{
    GtkWidget *empty_button;
    GtkWidget *restore_button;

    ScatolaWindow *window;
    gulong selection_handler_id;
};

#if GTK_CHECK_VERSION (3, 0, 0)
G_DEFINE_TYPE (ScatolaTrashBar, scatola_trash_bar, GTK_TYPE_BOX);
#else
G_DEFINE_TYPE (ScatolaTrashBar, scatola_trash_bar, GTK_TYPE_HBOX);
#endif

static void
restore_button_clicked_cb (GtkWidget *button,
                           ScatolaTrashBar *bar)
{
    GList *locations, *files, *l;

    locations = scatola_window_info_get_selection (SCATOLA_WINDOW_INFO  (bar->priv->window));
    files = NULL;

    for (l = locations; l != NULL; l = l->next)
    {
        files = g_list_prepend (files, scatola_file_get (l->data));
    }

    scatola_restore_files_from_trash (files, GTK_WINDOW (gtk_widget_get_toplevel (button)));

    scatola_file_list_free (files);
    g_list_free_full (locations, g_object_unref);
}

static void
selection_changed_cb (ScatolaWindow *window,
                      ScatolaTrashBar *bar)
{
    int count;

    count = scatola_window_info_get_selection_count (SCATOLA_WINDOW_INFO (window));

    gtk_widget_set_sensitive (bar->priv->restore_button, (count > 0));
}

static void
connect_window_and_update_button (ScatolaTrashBar *bar)
{
    bar->priv->selection_handler_id =
        g_signal_connect (bar->priv->window, "selection_changed",
                          G_CALLBACK (selection_changed_cb), bar);

    selection_changed_cb (bar->priv->window, bar);
}

static void
scatola_trash_bar_set_property (GObject      *object,
                             guint         prop_id,
                             const GValue *value,
                             GParamSpec   *pspec)
{
    ScatolaTrashBar *bar;

    bar = SCATOLA_TRASH_BAR (object);

    switch (prop_id)
    {
    case PROP_WINDOW:
        bar->priv->window = g_value_get_object (value);
        connect_window_and_update_button (bar);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
        break;
    }
}

static void
scatola_trash_bar_finalize (GObject *obj)
{
    ScatolaTrashBar *bar;

    bar = SCATOLA_TRASH_BAR (obj);

    if (bar->priv->selection_handler_id)
    {
        g_signal_handler_disconnect (bar->priv->window, bar->priv->selection_handler_id);
    }

    G_OBJECT_CLASS (scatola_trash_bar_parent_class)->finalize (obj);
}

static void
scatola_trash_bar_trash_state_changed (ScatolaTrashMonitor *trash_monitor,
                                    gboolean              state,
                                    gpointer              data)
{
    ScatolaTrashBar *bar;

    bar = SCATOLA_TRASH_BAR (data);

    gtk_widget_set_sensitive (bar->priv->empty_button,
                              !scatola_trash_monitor_is_empty ());
}

static void
scatola_trash_bar_class_init (ScatolaTrashBarClass *klass)
{
    GObjectClass *object_class;

    object_class = G_OBJECT_CLASS (klass);

    object_class->set_property = scatola_trash_bar_set_property;
    object_class->finalize = scatola_trash_bar_finalize;

    g_object_class_install_property (object_class,
                                     PROP_WINDOW,
                                     g_param_spec_object ("window",
                                             "window",
                                             "the ScatolaWindow",
                                             SCATOLA_TYPE_WINDOW,
                                             G_PARAM_WRITABLE |
                                             G_PARAM_CONSTRUCT_ONLY |
                                             G_PARAM_STATIC_STRINGS));

    g_type_class_add_private (klass, sizeof (ScatolaTrashBarPrivate));
}

static void
empty_trash_callback (GtkWidget *button, gpointer data)
{
    GtkWidget *window;

    window = gtk_widget_get_toplevel (button);

    scatola_file_operations_empty_trash (window);
}

static void
scatola_trash_bar_init (ScatolaTrashBar *bar)
{
    GtkWidget *label;
    GtkWidget *hbox;

    bar->priv = SCATOLA_TRASH_BAR_GET_PRIVATE (bar);

    hbox = GTK_WIDGET (bar);

    label = gtk_label_new (_("Trash"));
    gtk_widget_show (label);
#if GTK_CHECK_VERSION (3, 0, 0)
    gtk_orientable_set_orientation (GTK_ORIENTABLE (bar), GTK_ORIENTATION_HORIZONTAL);
#endif
    gtk_box_pack_start (GTK_BOX (bar), label, FALSE, FALSE, 0);

    bar->priv->empty_button = gtk_button_new_with_mnemonic (_("Empty _Trash"));
    gtk_widget_show (bar->priv->empty_button);
    gtk_box_pack_end (GTK_BOX (hbox), bar->priv->empty_button, FALSE, FALSE, 0);

    gtk_widget_set_sensitive (bar->priv->empty_button,
                              !scatola_trash_monitor_is_empty ());
    gtk_widget_set_tooltip_text (bar->priv->empty_button,
                                 _("Delete all items in the Trash"));

    g_signal_connect (bar->priv->empty_button,
                      "clicked",
                      G_CALLBACK (empty_trash_callback),
                      bar);

    bar->priv->restore_button = gtk_button_new_with_mnemonic (_("Restore Selected Items"));
    gtk_widget_show (bar->priv->restore_button);
    gtk_box_pack_end (GTK_BOX (hbox), bar->priv->restore_button, FALSE, FALSE, 6);

    gtk_widget_set_sensitive (bar->priv->restore_button, FALSE);
    gtk_widget_set_tooltip_text (bar->priv->restore_button,
                                 _("Restore selected items to their original position"));

    g_signal_connect (bar->priv->restore_button,
                      "clicked",
                      G_CALLBACK (restore_button_clicked_cb),
                      bar);

    g_signal_connect_object (scatola_trash_monitor_get (),
                             "trash_state_changed",
                             G_CALLBACK (scatola_trash_bar_trash_state_changed),
                             bar,
                             0);
}

GtkWidget *
scatola_trash_bar_new (ScatolaWindow *window)
{
    GObject *bar;

    bar = g_object_new (SCATOLA_TYPE_TRASH_BAR, "window", window, NULL);

    return GTK_WIDGET (bar);
}
