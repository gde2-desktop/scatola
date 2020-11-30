/*
 *  scatola-menu.h - Menus exported by ScatolaMenuProvider objects.
 *
 *  Copyright (C) 2005 Raffaele Sandrini
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
 *  Author:  Raffaele Sandrini <rasa@gmx.ch>
 *
 */

#include <config.h>
#include "scatola-menu.h"
#include "scatola-extension-i18n.h"

#include <glib.h>

/**
 * SECTION:scatola-menu
 * @title: ScatolaMenu
 * @short_description: Menu descriptor object
 * @include: libscatola-extension/scatola-menu.h
 *
 * #ScatolaMenu is an object that describes a submenu in a file manager
 * menu. Extensions can provide #ScatolaMenu objects by attaching them to
 * #ScatolaMenuItem objects, using scatola_menu_item_set_submenu().
 */

#define SCATOLA_MENU_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE ((o), SCATOLA_TYPE_MENU, ScatolaMenuPrivate))
G_DEFINE_TYPE (ScatolaMenu, scatola_menu, G_TYPE_OBJECT);

struct _ScatolaMenuPrivate {
    GList *item_list;
};

void
scatola_menu_append_item (ScatolaMenu *menu, ScatolaMenuItem *item)
{
    g_return_if_fail (menu != NULL);
    g_return_if_fail (item != NULL);

    menu->priv->item_list = g_list_append (menu->priv->item_list, g_object_ref (item));
}

/**
 * scatola_menu_get_items:
 * @menu: a #ScatolaMenu
 *
 * Returns: (element-type ScatolaMenuItem) (transfer full): the provided #ScatolaMenuItem list
 */
GList *
scatola_menu_get_items (ScatolaMenu *menu)
{
    GList *item_list;

    g_return_val_if_fail (menu != NULL, NULL);

    item_list = g_list_copy (menu->priv->item_list);
    g_list_foreach (item_list, (GFunc)g_object_ref, NULL);

    return item_list;
}

/**
 * scatola_menu_item_list_free:
 * @item_list: (element-type ScatolaMenuItem): a list of #ScatolaMenuItem
 *
 */
void
scatola_menu_item_list_free (GList *item_list)
{
    g_return_if_fail (item_list != NULL);

    g_list_foreach (item_list, (GFunc)g_object_unref, NULL);
    g_list_free (item_list);
}

/* Type initialization */

static void
scatola_menu_finalize (GObject *object)
{
    ScatolaMenu *menu = SCATOLA_MENU (object);

    if (menu->priv->item_list) {
        g_list_free (menu->priv->item_list);
    }

    G_OBJECT_CLASS (scatola_menu_parent_class)->finalize (object);
}

static void
scatola_menu_init (ScatolaMenu *menu)
{
    menu->priv = SCATOLA_MENU_GET_PRIVATE (menu);

    menu->priv->item_list = NULL;
}

static void
scatola_menu_class_init (ScatolaMenuClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);

    g_type_class_add_private (klass, sizeof (ScatolaMenuPrivate));

    object_class->finalize = scatola_menu_finalize;
}

/* public constructors */

ScatolaMenu *
scatola_menu_new (void)
{
    ScatolaMenu *obj;

    obj = SCATOLA_MENU (g_object_new (SCATOLA_TYPE_MENU, NULL));

    return obj;
}
