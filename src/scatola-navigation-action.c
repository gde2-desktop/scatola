/* -*- Mode: C; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 8 -*- */

/*
 *  Scatola
 *
 *  Copyright (C) 2004 Red Hat, Inc.
 *  Copyright (C) 2003 Marco Pesenti Gritti
 *
 *  Scatola is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License as
 *  published by the Free Software Foundation; either version 2 of the
 *  License, or (at your option) any later version.
 *
 *  Scatola is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 *  Based on ephy-navigation-action.h from Epiphany
 *
 *  Authors: Alexander Larsson <alexl@redhat.com>
 *           Marco Pesenti Gritti
 *
 */

#include <config.h>

#include "scatola-navigation-action.h"
#include "scatola-navigation-window.h"
#include "scatola-window-private.h"
#include "scatola-navigation-window-slot.h"
#include <gtk/gtk.h>
#include <eel/eel-gtk-extensions.h>

static void scatola_navigation_action_init       (ScatolaNavigationAction *action);
static void scatola_navigation_action_class_init (ScatolaNavigationActionClass *class);

static GObjectClass *parent_class = NULL;

#define SCATOLA_NAVIGATION_ACTION_GET_PRIVATE(object)(G_TYPE_INSTANCE_GET_PRIVATE ((object), SCATOLA_TYPE_NAVIGATION_ACTION, ScatolaNavigationActionPrivate))

struct ScatolaNavigationActionPrivate
{
    ScatolaNavigationWindow *window;
    ScatolaNavigationDirection direction;
    char *arrow_tooltip;
};

enum
{
    PROP_0,
    PROP_ARROW_TOOLTIP,
    PROP_DIRECTION,
    PROP_WINDOW
};

GType
scatola_navigation_action_get_type (void)
{
    static GType type = 0;

    if (type == 0)
    {
        const GTypeInfo type_info =
        {
            sizeof (ScatolaNavigationActionClass),
            (GBaseInitFunc) NULL,
            (GBaseFinalizeFunc) NULL,
            (GClassInitFunc) scatola_navigation_action_class_init,
            (GClassFinalizeFunc) NULL,
            NULL,
            sizeof (ScatolaNavigationAction),
            0, /* n_preallocs */
            (GInstanceInitFunc) scatola_navigation_action_init,
        };

        type = g_type_register_static (GTK_TYPE_ACTION,
                                       "ScatolaNavigationAction",
                                       &type_info, 0);
    }

    return type;
}

static gboolean
should_open_in_new_tab (void)
{
    /* FIXME this is duplicated */
    GdkEvent *event;

    event = gtk_get_current_event ();
    if (event->type == GDK_BUTTON_PRESS || event->type == GDK_BUTTON_RELEASE)
    {
        return event->button.button == 2;
    }

    gdk_event_free (event);

    return FALSE;
}

static void
activate_back_or_forward_menu_item (GtkMenuItem *menu_item,
                                    ScatolaNavigationWindow *window,
                                    gboolean back)
{
    int index;

    g_assert (GTK_IS_MENU_ITEM (menu_item));
    g_assert (SCATOLA_IS_NAVIGATION_WINDOW (window));

    index = GPOINTER_TO_INT (g_object_get_data (G_OBJECT (menu_item), "user_data"));

    scatola_navigation_window_back_or_forward (window, back, index, should_open_in_new_tab ());
}

static void
activate_back_menu_item_callback (GtkMenuItem *menu_item, ScatolaNavigationWindow *window)
{
    activate_back_or_forward_menu_item (menu_item, window, TRUE);
}

static void
activate_forward_menu_item_callback (GtkMenuItem *menu_item, ScatolaNavigationWindow *window)
{
    activate_back_or_forward_menu_item (menu_item, window, FALSE);
}

static void
fill_menu (ScatolaNavigationWindow *window,
           GtkWidget *menu,
           gboolean back)
{
    ScatolaNavigationWindowSlot *slot;
    GtkWidget *menu_item;
    int index;
    GList *list;

    g_assert (SCATOLA_IS_NAVIGATION_WINDOW (window));

    slot = SCATOLA_NAVIGATION_WINDOW_SLOT (SCATOLA_WINDOW (window)->details->active_pane->active_slot);

    list = back ? slot->back_list : slot->forward_list;
    index = 0;
    while (list != NULL)
    {
        menu_item = scatola_bookmark_menu_item_new (SCATOLA_BOOKMARK (list->data));
        g_object_set_data (G_OBJECT (menu_item), "user_data", GINT_TO_POINTER (index));
        gtk_widget_show (GTK_WIDGET (menu_item));
        g_signal_connect_object (menu_item, "activate",
                                 back
                                 ? G_CALLBACK (activate_back_menu_item_callback)
                                 : G_CALLBACK (activate_forward_menu_item_callback),
                                 window, 0);

        gtk_menu_shell_append (GTK_MENU_SHELL (menu), menu_item);
        list = g_list_next (list);
        ++index;
    }
}

static void
show_menu_callback (GtkMenuToolButton *button,
                    ScatolaNavigationAction *action)
{
    ScatolaNavigationActionPrivate *p;
    ScatolaNavigationWindow *window;
    GtkWidget *menu;
    GList *children;
    GList *li;

    p = action->priv;
    window = action->priv->window;

    menu = gtk_menu_tool_button_get_menu (button);

    children = gtk_container_get_children (GTK_CONTAINER (menu));
    for (li = children; li; li = li->next)
    {
        gtk_container_remove (GTK_CONTAINER (menu), li->data);
    }
    g_list_free (children);

    switch (p->direction)
    {
    case SCATOLA_NAVIGATION_DIRECTION_FORWARD:
        fill_menu (window, menu, FALSE);
        break;
    case SCATOLA_NAVIGATION_DIRECTION_BACK:
        fill_menu (window, menu, TRUE);
        break;
    default:
        g_assert_not_reached ();
        break;
    }
}

static gboolean
proxy_button_press_event_cb (GtkButton *button,
                             GdkEventButton *event,
                             gpointer user_data)
{
    if (event->button == 2)
    {
        g_signal_emit_by_name (button, "pressed", 0);
    }

    return FALSE;
}

static gboolean
proxy_button_release_event_cb (GtkButton *button,
                               GdkEventButton *event,
                               gpointer user_data)
{
    if (event->button == 2)
    {
        g_signal_emit_by_name (button, "released", 0);
    }

    return FALSE;
}

static void
connect_proxy (GtkAction *action, GtkWidget *proxy)
{
    if (GTK_IS_MENU_TOOL_BUTTON (proxy))
    {
        ScatolaNavigationAction *naction = SCATOLA_NAVIGATION_ACTION (action);
        GtkMenuToolButton *button = GTK_MENU_TOOL_BUTTON (proxy);
        GtkWidget *menu;
        GtkWidget *child;

        /* set an empty menu, so the arrow button becomes sensitive */
        menu = gtk_menu_new ();
        gtk_menu_tool_button_set_menu (button, menu);

        gtk_menu_tool_button_set_arrow_tooltip_text (button,
                naction->priv->arrow_tooltip);

        g_signal_connect (proxy, "show-menu",
                          G_CALLBACK (show_menu_callback), action);

        /* Make sure that middle click works. Note that there is some code duplication
         * between here and scatola-window-menus.c */
        child = eel_gtk_menu_tool_button_get_button (button);
        g_signal_connect (child, "button-press-event", G_CALLBACK (proxy_button_press_event_cb), NULL);
        g_signal_connect (child, "button-release-event", G_CALLBACK (proxy_button_release_event_cb), NULL);
    }

    (* GTK_ACTION_CLASS (parent_class)->connect_proxy) (action, proxy);
}

static void
disconnect_proxy (GtkAction *action, GtkWidget *proxy)
{
    if (GTK_IS_MENU_TOOL_BUTTON (proxy))
    {
        GtkWidget *child;

        g_signal_handlers_disconnect_by_func (proxy, G_CALLBACK (show_menu_callback), action);

        child = eel_gtk_menu_tool_button_get_button (GTK_MENU_TOOL_BUTTON (proxy));
        g_signal_handlers_disconnect_by_func (child, G_CALLBACK (proxy_button_press_event_cb), NULL);
        g_signal_handlers_disconnect_by_func (child, G_CALLBACK (proxy_button_release_event_cb), NULL);
    }

    (* GTK_ACTION_CLASS (parent_class)->disconnect_proxy) (action, proxy);
}

static void
scatola_navigation_action_finalize (GObject *object)
{
    ScatolaNavigationAction *action = SCATOLA_NAVIGATION_ACTION (object);

    g_free (action->priv->arrow_tooltip);

    (* G_OBJECT_CLASS (parent_class)->finalize) (object);
}

static void
scatola_navigation_action_set_property (GObject *object,
                                     guint prop_id,
                                     const GValue *value,
                                     GParamSpec *pspec)
{
    ScatolaNavigationAction *nav;

    nav = SCATOLA_NAVIGATION_ACTION (object);

    switch (prop_id)
    {
    case PROP_ARROW_TOOLTIP:
        g_free (nav->priv->arrow_tooltip);
        nav->priv->arrow_tooltip = g_value_dup_string (value);
        break;
    case PROP_DIRECTION:
        nav->priv->direction = g_value_get_int (value);
        break;
    case PROP_WINDOW:
        nav->priv->window = SCATOLA_NAVIGATION_WINDOW (g_value_get_object (value));
        break;
    }
}

static void
scatola_navigation_action_get_property (GObject *object,
                                     guint prop_id,
                                     GValue *value,
                                     GParamSpec *pspec)
{
    ScatolaNavigationAction *nav;

    nav = SCATOLA_NAVIGATION_ACTION (object);

    switch (prop_id)
    {
    case PROP_ARROW_TOOLTIP:
        g_value_set_string (value, nav->priv->arrow_tooltip);
        break;
    case PROP_DIRECTION:
        g_value_set_int (value, nav->priv->direction);
        break;
    case PROP_WINDOW:
        g_value_set_object (value, nav->priv->window);
        break;
    }
}

static void
scatola_navigation_action_class_init (ScatolaNavigationActionClass *class)
{
    GObjectClass *object_class = G_OBJECT_CLASS (class);
    GtkActionClass *action_class = GTK_ACTION_CLASS (class);

    object_class->finalize = scatola_navigation_action_finalize;
    object_class->set_property = scatola_navigation_action_set_property;
    object_class->get_property = scatola_navigation_action_get_property;

    parent_class = g_type_class_peek_parent (class);

    action_class->toolbar_item_type = GTK_TYPE_MENU_TOOL_BUTTON;
    action_class->connect_proxy = connect_proxy;
    action_class->disconnect_proxy = disconnect_proxy;

    g_object_class_install_property (object_class,
                                     PROP_ARROW_TOOLTIP,
                                     g_param_spec_string ("arrow-tooltip",
                                             "Arrow Tooltip",
                                             "Arrow Tooltip",
                                             NULL,
                                             G_PARAM_READWRITE));
    g_object_class_install_property (object_class,
                                     PROP_DIRECTION,
                                     g_param_spec_int ("direction",
                                             "Direction",
                                             "Direction",
                                             0,
                                             G_MAXINT,
                                             0,
                                             G_PARAM_READWRITE));
    g_object_class_install_property (object_class,
                                     PROP_WINDOW,
                                     g_param_spec_object ("window",
                                             "Window",
                                             "The navigation window",
                                             G_TYPE_OBJECT,
                                             G_PARAM_READWRITE));

    g_type_class_add_private (object_class, sizeof(ScatolaNavigationActionPrivate));
}

static void
scatola_navigation_action_init (ScatolaNavigationAction *action)
{
    action->priv = SCATOLA_NAVIGATION_ACTION_GET_PRIVATE (action);
}
