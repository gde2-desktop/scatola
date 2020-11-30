/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */
/*
 * Copyright (C) 2005 Novell, Inc.
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
 * License along with this program; see the file COPYING.  If not,
 * write to the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 * Author: Anders Carlsson <andersca@imendio.com>
 *
 */

#include <config.h>
#include "scatola-search-bar.h"

#include <glib/gi18n.h>
#include <eel/eel-gtk-macros.h>
#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>

#if GTK_CHECK_VERSION (3, 0, 0)
#define gtk_hbox_new(X,Y) gtk_box_new(GTK_ORIENTATION_HORIZONTAL,Y)
#endif

struct ScatolaSearchBarDetails
{
    GtkWidget *entry;
    gboolean entry_borrowed;
};

enum
{
    ACTIVATE,
    CANCEL,
    FOCUS_IN,
    LAST_SIGNAL
};

static guint signals[LAST_SIGNAL] = { 0 };

static void  scatola_search_bar_class_init       (ScatolaSearchBarClass *class);
static void  scatola_search_bar_init             (ScatolaSearchBar      *bar);

EEL_CLASS_BOILERPLATE (ScatolaSearchBar,
                       scatola_search_bar,
                       GTK_TYPE_EVENT_BOX)


static void
finalize (GObject *object)
{
    ScatolaSearchBar *bar;

    bar = SCATOLA_SEARCH_BAR (object);

    g_free (bar->details);

    EEL_CALL_PARENT (G_OBJECT_CLASS, finalize, (object));
}

static void
scatola_search_bar_class_init (ScatolaSearchBarClass *class)
{
    GObjectClass *gobject_class;
    GtkBindingSet *binding_set;

    gobject_class = G_OBJECT_CLASS (class);
    gobject_class->finalize = finalize;

    signals[ACTIVATE] =
        g_signal_new ("activate",
                      G_TYPE_FROM_CLASS (class),
                      G_SIGNAL_RUN_LAST,
                      G_STRUCT_OFFSET (ScatolaSearchBarClass, activate),
                      NULL, NULL,
                      g_cclosure_marshal_VOID__VOID,
                      G_TYPE_NONE, 0);

    signals[FOCUS_IN] =
        g_signal_new ("focus-in",
                      G_TYPE_FROM_CLASS (class),
                      G_SIGNAL_RUN_LAST,
                      G_STRUCT_OFFSET (ScatolaSearchBarClass, focus_in),
                      NULL, NULL,
                      g_cclosure_marshal_VOID__VOID,
                      G_TYPE_NONE, 0);

    signals[CANCEL] =
        g_signal_new ("cancel",
                      G_TYPE_FROM_CLASS (class),
                      G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION,
                      G_STRUCT_OFFSET (ScatolaSearchBarClass, cancel),
                      NULL, NULL,
                      g_cclosure_marshal_VOID__VOID,
                      G_TYPE_NONE, 0);

    binding_set = gtk_binding_set_by_class (class);
	gtk_binding_entry_add_signal (binding_set, GDK_KEY_Escape, 0, "cancel", 0);
}

static gboolean
entry_has_text (ScatolaSearchBar *bar)
{
    const char *text;

    text = gtk_entry_get_text (GTK_ENTRY (bar->details->entry));

    return text != NULL && text[0] != '\0';
}

static void
entry_icon_release_cb (GtkEntry *entry,
                       GtkEntryIconPosition position,
                       GdkEvent *event,
                       ScatolaSearchBar *bar)
{
    g_signal_emit_by_name (entry, "activate", 0);
}

static void
entry_activate_cb (GtkWidget *entry, ScatolaSearchBar *bar)
{
    if (entry_has_text (bar) && !bar->details->entry_borrowed)
    {
        g_signal_emit (bar, signals[ACTIVATE], 0);
    }
}

static gboolean
focus_in_event_callback (GtkWidget *widget,
                         GdkEventFocus *event,
                         gpointer user_data)
{
    ScatolaSearchBar *bar;

    bar = SCATOLA_SEARCH_BAR (user_data);

    g_signal_emit (bar, signals[FOCUS_IN], 0);

    return FALSE;
}

static void
scatola_search_bar_init (ScatolaSearchBar *bar)
{
#if !GTK_CHECK_VERSION(3, 0, 0)
    GtkWidget *alignment;
#endif
    GtkWidget *hbox;
    GtkWidget *label;
#if GTK_CHECK_VERSION(3, 0, 0)
    GtkStyleContext *context;

    context = gtk_widget_get_style_context (GTK_WIDGET (bar));
    gtk_style_context_add_class (context, "scatola-search-bar");
#endif

    bar->details = g_new0 (ScatolaSearchBarDetails, 1);

    gtk_event_box_set_visible_window (GTK_EVENT_BOX (bar), FALSE);

#if GTK_CHECK_VERSION(3, 0, 0)
    hbox = gtk_hbox_new (FALSE, 6);
    gtk_widget_set_margin_start (hbox, 6);
    gtk_widget_set_margin_end (hbox, 6);
    gtk_widget_show (hbox);
    gtk_container_add (GTK_CONTAINER (bar), hbox);

#else
    alignment = gtk_alignment_new (0.5, 0.5,
                                   1.0, 1.0);
    gtk_alignment_set_padding (GTK_ALIGNMENT (alignment),
                               0, 0, 6, 6);
    gtk_widget_show (alignment);
    gtk_container_add (GTK_CONTAINER (bar), alignment);

    hbox = gtk_hbox_new (FALSE, 6);
    gtk_widget_show (hbox);
    gtk_container_add (GTK_CONTAINER (alignment), hbox);
#endif

    label = gtk_label_new (_("Search:"));
    gtk_widget_show (label);

    gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, FALSE, 0);

    bar->details->entry = gtk_entry_new ();
    gtk_entry_set_icon_from_icon_name (GTK_ENTRY (bar->details->entry),
                                   GTK_ENTRY_ICON_SECONDARY,
                                   "find");
    gtk_box_pack_start (GTK_BOX (hbox), bar->details->entry, TRUE, TRUE, 0);

    g_signal_connect (bar->details->entry, "activate",
                      G_CALLBACK (entry_activate_cb), bar);
    g_signal_connect (bar->details->entry, "icon-release",
                      G_CALLBACK (entry_icon_release_cb), bar);
    g_signal_connect (bar->details->entry, "focus-in-event",
                      G_CALLBACK (focus_in_event_callback), bar);

    gtk_widget_show (bar->details->entry);
}

GtkWidget *
scatola_search_bar_borrow_entry (ScatolaSearchBar *bar)
{
    GtkBindingSet *binding_set;

    bar->details->entry_borrowed = TRUE;

    binding_set = gtk_binding_set_by_class (G_OBJECT_GET_CLASS (bar));
	gtk_binding_entry_remove (binding_set, GDK_KEY_Escape, 0);
    return bar->details->entry;
}

void
scatola_search_bar_return_entry (ScatolaSearchBar *bar)
{
    GtkBindingSet *binding_set;

    bar->details->entry_borrowed = FALSE;

    binding_set = gtk_binding_set_by_class (G_OBJECT_GET_CLASS (bar));
	gtk_binding_entry_add_signal (binding_set, GDK_KEY_Escape, 0, "cancel", 0);
}

GtkWidget *
scatola_search_bar_new (void)
{
    GtkWidget *bar;

    bar = g_object_new (SCATOLA_TYPE_SEARCH_BAR, NULL);

    return bar;
}

ScatolaQuery *
scatola_search_bar_get_query (ScatolaSearchBar *bar)
{
    const char *query_text;
    ScatolaQuery *query;

    query_text = gtk_entry_get_text (GTK_ENTRY (bar->details->entry));

    /* Empty string is a NULL query */
    if (query_text && query_text[0] == '\0')
    {
        return NULL;
    }

    query = scatola_query_new ();
    scatola_query_set_text (query, query_text);

    return query;
}

void
scatola_search_bar_grab_focus (ScatolaSearchBar *bar)
{
    gtk_widget_grab_focus (bar->details->entry);
}

void
scatola_search_bar_clear (ScatolaSearchBar *bar)
{
    gtk_entry_set_text (GTK_ENTRY (bar->details->entry), "");
}