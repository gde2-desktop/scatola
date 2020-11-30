/*
 *  scatola-menu-item.c - Menu items exported by ScatolaMenuProvider
 *                         objects.
 *
 *  Copyright (C) 2003 Novell, Inc.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public
 *  License along with this library; if not, write to the Free
 *  Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 *  Author:  Dave Camp <dave@ximian.com>
 *
 */

#include <config.h>
#include "scatola-menu.h"
#include "scatola-extension-i18n.h"

enum {
    ACTIVATE,
    LAST_SIGNAL
};

enum {
    PROP_0,
    PROP_NAME,
    PROP_LABEL,
    PROP_TIP,
    PROP_ICON,
    PROP_SENSITIVE,
    PROP_PRIORITY,
    PROP_MENU,
    LAST_PROP
};

struct _ScatolaMenuItemDetails {
    char *name;
    char *label;
    char *tip;
    char *icon;
    ScatolaMenu *menu;
    gboolean sensitive;
    gboolean priority;
};

static guint signals[LAST_SIGNAL] = { 0 };

static GObjectClass *parent_class = NULL;

/**
 * SECTION:scatola-menu-item
 * @title: ScatolaMenuItem
 * @short_description: Menu item descriptor object
 * @include: libscatola-extension/scatola-menu-item.h
 *
 * #ScatolaMenuItem is an object that describes an item in a file manager
 * menu. Extensions can provide #ScatolaMenuItem objects by registering a
 * #ScatolaMenuProvider and returning them from
 * scatola_menu_provider_get_file_items(), or
 * scatola_menu_provider_get_background_items(), which will be called by the
 * main application when creating menus.
 */

/**
 * scatola_menu_item_new:
 * @name: the identifier for the menu item
 * @label: the user-visible label of the menu item
 * @tip: the tooltip of the menu item
 * @icon: the name of the icon to display in the menu item
 *
 * Creates a new menu item that can be added to the toolbar or to a contextual menu.
 *
 * Returns: a newly create #ScatolaMenuItem
 */
ScatolaMenuItem *
scatola_menu_item_new (const char *name,
                    const char *label,
                    const char *tip,
                    const char *icon)
{
    ScatolaMenuItem *item;

    g_return_val_if_fail (name != NULL, NULL);
    g_return_val_if_fail (label != NULL, NULL);
    g_return_val_if_fail (tip != NULL, NULL);

    item = g_object_new (SCATOLA_TYPE_MENU_ITEM,
                         "name", name,
                         "label", label,
                         "tip", tip,
                         "icon", icon,
                         NULL);

    return item;
}

/**
 * scatola_menu_item_activate:
 * @item: pointer to a #ScatolaMenuItem
 *
 * emits the activate signal.
 */
void
scatola_menu_item_activate (ScatolaMenuItem *item)
{
    g_signal_emit (item, signals[ACTIVATE], 0);
}

/**
 * scatola_menu_item_set_submenu:
 * @item: pointer to a #ScatolaMenuItem
 * @menu: pointer to a #ScatolaMenu to attach to the button
 *
 * Attachs a menu to the given #ScatolaMenuItem.
 */
void
scatola_menu_item_set_submenu (ScatolaMenuItem *item, ScatolaMenu *menu)
{
    g_object_set (item, "menu", menu, NULL);
}

static void
scatola_menu_item_get_property (GObject *object,
                             guint param_id,
                             GValue *value,
                             GParamSpec *pspec)
{
    ScatolaMenuItem *item;

    item = SCATOLA_MENU_ITEM (object);

    switch (param_id) {
    case PROP_NAME :
        g_value_set_string (value, item->details->name);
        break;
    case PROP_LABEL :
        g_value_set_string (value, item->details->label);
        break;
    case PROP_TIP :
        g_value_set_string (value, item->details->tip);
        break;
    case PROP_ICON :
        g_value_set_string (value, item->details->icon);
        break;
    case PROP_SENSITIVE :
        g_value_set_boolean (value, item->details->sensitive);
        break;
    case PROP_PRIORITY :
        g_value_set_boolean (value, item->details->priority);
        break;
    case PROP_MENU :
        g_value_set_object (value, item->details->menu);
        break;
    default :
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, param_id, pspec);
        break;
    }
}

static void
scatola_menu_item_set_property (GObject *object,
                             guint param_id,
                             const GValue *value,
                             GParamSpec *pspec)
{
    ScatolaMenuItem *item;

    item = SCATOLA_MENU_ITEM (object);

    switch (param_id) {
    case PROP_NAME :
        g_free (item->details->name);
        item->details->name = g_strdup (g_value_get_string (value));
        g_object_notify (object, "name");
        break;
    case PROP_LABEL :
        g_free (item->details->label);
        item->details->label = g_strdup (g_value_get_string (value));
        g_object_notify (object, "label");
        break;
    case PROP_TIP :
        g_free (item->details->tip);
        item->details->tip = g_strdup (g_value_get_string (value));
        g_object_notify (object, "tip");
        break;
    case PROP_ICON :
        g_free (item->details->icon);
        item->details->icon = g_strdup (g_value_get_string (value));
        g_object_notify (object, "icon");
        break;
    case PROP_SENSITIVE :
        item->details->sensitive = g_value_get_boolean (value);
        g_object_notify (object, "sensitive");
        break;
    case PROP_PRIORITY :
        item->details->priority = g_value_get_boolean (value);
        g_object_notify (object, "priority");
        break;
    case PROP_MENU :
        if (item->details->menu) {
            g_object_unref (item->details->menu);
        }
        item->details->menu = g_object_ref (g_value_get_object (value));
        g_object_notify (object, "menu");
        break;
    default :
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, param_id, pspec);
        break;
    }
}

static void
scatola_menu_item_finalize (GObject *object)
{
    ScatolaMenuItem *item;

    item = SCATOLA_MENU_ITEM (object);

    g_free (item->details->name);
    g_free (item->details->label);
    g_free (item->details->tip);
    g_free (item->details->icon);
    if (item->details->menu) {
        g_object_unref (item->details->menu);
    }

    g_free (item->details);

    G_OBJECT_CLASS (parent_class)->finalize (object);
}

static void
scatola_menu_item_instance_init (ScatolaMenuItem *item)
{
    item->details = g_new0 (ScatolaMenuItemDetails, 1);
    item->details->sensitive = TRUE;
    item->details->menu = NULL;
}

static void
scatola_menu_item_class_init (ScatolaMenuItemClass *class)
{
    parent_class = g_type_class_peek_parent (class);

    G_OBJECT_CLASS (class)->finalize = scatola_menu_item_finalize;
    G_OBJECT_CLASS (class)->get_property = scatola_menu_item_get_property;
    G_OBJECT_CLASS (class)->set_property = scatola_menu_item_set_property;

    signals[ACTIVATE] =
        g_signal_new ("activate",
                      G_TYPE_FROM_CLASS (class),
                      G_SIGNAL_RUN_LAST,
                      G_STRUCT_OFFSET (ScatolaMenuItemClass,
                                       activate),
                      NULL, NULL,
                      g_cclosure_marshal_VOID__VOID,
                      G_TYPE_NONE, 0);

    g_object_class_install_property (G_OBJECT_CLASS (class),
                                     PROP_NAME,
                                     g_param_spec_string ("name",
                                             "Name",
                                             "Name of the item",
                                             NULL,
                                             G_PARAM_CONSTRUCT_ONLY | G_PARAM_WRITABLE | G_PARAM_READABLE));
    g_object_class_install_property (G_OBJECT_CLASS (class),
                                     PROP_LABEL,
                                     g_param_spec_string ("label",
                                             "Label",
                                             "Label to display to the user",
                                             NULL,
                                             G_PARAM_READWRITE));
    g_object_class_install_property (G_OBJECT_CLASS (class),
                                     PROP_TIP,
                                     g_param_spec_string ("tip",
                                             "Tip",
                                             "Tooltip for the menu item",
                                             NULL,
                                             G_PARAM_READWRITE));
    g_object_class_install_property (G_OBJECT_CLASS (class),
                                     PROP_ICON,
                                     g_param_spec_string ("icon",
                                             "Icon",
                                             "Name of the icon to display in the menu item",
                                             NULL,
                                             G_PARAM_READWRITE));

    g_object_class_install_property (G_OBJECT_CLASS (class),
                                     PROP_SENSITIVE,
                                     g_param_spec_boolean ("sensitive",
                                             "Sensitive",
                                             "Whether the menu item is sensitive",
                                             TRUE,
                                             G_PARAM_READWRITE));
    g_object_class_install_property (G_OBJECT_CLASS (class),
                                     PROP_PRIORITY,
                                     g_param_spec_boolean ("priority",
                                             "Priority",
                                             "Show priority text in toolbars",
                                             TRUE,
                                             G_PARAM_READWRITE));
    g_object_class_install_property (G_OBJECT_CLASS (class),
                                     PROP_MENU,
                                     g_param_spec_object ("menu",
                                             "Menu",
                                             "The menu belonging to this item. May be null.",
                                             SCATOLA_TYPE_MENU,
                                             G_PARAM_READWRITE));
}

GType
scatola_menu_item_get_type (void)
{
    static GType type = 0;

    if (!type) {
        const GTypeInfo info = {
            sizeof (ScatolaMenuItemClass),
            NULL,
            NULL,
            (GClassInitFunc)scatola_menu_item_class_init,
            NULL,
            NULL,
            sizeof (ScatolaMenuItem),
            0,
            (GInstanceInitFunc)scatola_menu_item_instance_init
        };

        type = g_type_register_static
               (G_TYPE_OBJECT,
                "ScatolaMenuItem",
                &info, 0);
    }

    return type;
}

