/*
 *  scatola-menu.h - Menus exported by ScatolaMenuProvider objects.
 *
 *  Copyright (C) 2005 Raffaele Sandrini
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
 *           Raffaele Sandrini <rasa@gmx.ch>
 *
 */

#ifndef SCATOLA_MENU_H
#define SCATOLA_MENU_H

#include <glib-object.h>
#include "scatola-extension-types.h"

G_BEGIN_DECLS

/* ScatolaMenu defines */
#define SCATOLA_TYPE_MENU         (scatola_menu_get_type ())
#define SCATOLA_MENU(o)           (G_TYPE_CHECK_INSTANCE_CAST ((o), SCATOLA_TYPE_MENU, ScatolaMenu))
#define SCATOLA_MENU_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST((k), SCATOLA_TYPE_MENU, ScatolaMenuClass))
#define SCATOLA_IS_MENU(o)        (G_TYPE_CHECK_INSTANCE_TYPE ((o), SCATOLA_TYPE_MENU))
#define SCATOLA_IS_MENU_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE ((k), SCATOLA_TYPE_MENU))
#define SCATOLA_MENU_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), SCATOLA_TYPE_MENU, ScatolaMenuClass))
/* ScatolaMenuItem defines */
#define SCATOLA_TYPE_MENU_ITEM            (scatola_menu_item_get_type())
#define SCATOLA_MENU_ITEM(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), SCATOLA_TYPE_MENU_ITEM, ScatolaMenuItem))
#define SCATOLA_MENU_ITEM_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), SCATOLA_TYPE_MENU_ITEM, ScatolaMenuItemClass))
#define SCATOLA_MENU_IS_ITEM(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SCATOLA_TYPE_MENU_ITEM))
#define SCATOLA_MENU_IS_ITEM_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((obj), SCATOLA_TYPE_MENU_ITEM))
#define SCATOLA_MENU_ITEM_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj), SCATOLA_TYPE_MENU_ITEM, ScatolaMenuItemClass))

/* ScatolaMenu types */
typedef struct _ScatolaMenu		ScatolaMenu;
typedef struct _ScatolaMenuPrivate	ScatolaMenuPrivate;
typedef struct _ScatolaMenuClass	ScatolaMenuClass;
/* ScatolaMenuItem types */
typedef struct _ScatolaMenuItem        ScatolaMenuItem;
typedef struct _ScatolaMenuItemDetails ScatolaMenuItemDetails;
typedef struct _ScatolaMenuItemClass   ScatolaMenuItemClass;

/* ScatolaMenu structs */
struct _ScatolaMenu {
    GObject parent;
    ScatolaMenuPrivate *priv;
};

struct _ScatolaMenuClass {
    GObjectClass parent_class;
};

/* ScatolaMenuItem structs */
struct _ScatolaMenuItem {
    GObject parent;

    ScatolaMenuItemDetails *details;
};

struct _ScatolaMenuItemClass {
    GObjectClass parent;

    void (*activate) (ScatolaMenuItem *item);
};

/* ScatolaMenu methods */
GType     scatola_menu_get_type       (void);
ScatolaMenu *scatola_menu_new            (void);

void      scatola_menu_append_item    (ScatolaMenu     *menu,
                                    ScatolaMenuItem *item);
GList    *scatola_menu_get_items      (ScatolaMenu *menu);
void      scatola_menu_item_list_free (GList *item_list);

/* ScatolaMenuItem methods */
GType         scatola_menu_item_get_type    (void);
ScatolaMenuItem *scatola_menu_item_new         (const char   *name,
                                          const char   *label,
                                          const char   *tip,
                                          const char   *icon);

void          scatola_menu_item_activate    (ScatolaMenuItem *item);
void          scatola_menu_item_set_submenu (ScatolaMenuItem *item,
                                          ScatolaMenu     *menu);

/* ScatolaMenuItem has the following properties:
 *   name (string)        - the identifier for the menu item
 *   label (string)       - the user-visible label of the menu item
 *   tip (string)         - the tooltip of the menu item
 *   icon (string)        - the name of the icon to display in the menu item
 *   sensitive (boolean)  - whether the menu item is sensitive or not
 *   priority (boolean)   - used for toolbar items, whether to show priority
 *                          text.
 *   menu (ScatolaMenu)      - The menu belonging to this item. May be null.
 */

G_END_DECLS

#endif /* SCATOLA_MENU_H */
