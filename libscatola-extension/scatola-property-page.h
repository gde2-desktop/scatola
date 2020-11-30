/*
 *  scatola-property-page.h - Property pages exported by
 *                             ScatolaPropertyProvider objects.
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

#ifndef SCATOLA_PROPERTY_PAGE_H
#define SCATOLA_PROPERTY_PAGE_H

#include <glib-object.h>
#include <gtk/gtk.h>
#include "scatola-extension-types.h"

G_BEGIN_DECLS

#define SCATOLA_TYPE_PROPERTY_PAGE            (scatola_property_page_get_type())
#define SCATOLA_PROPERTY_PAGE(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), SCATOLA_TYPE_PROPERTY_PAGE, ScatolaPropertyPage))
#define SCATOLA_PROPERTY_PAGE_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), SCATOLA_TYPE_PROPERTY_PAGE, ScatolaPropertyPageClass))
#define SCATOLA_IS_PROPERTY_PAGE(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SCATOLA_TYPE_PROPERTY_PAGE))
#define SCATOLA_IS_PROPERTY_PAGE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((obj), SCATOLA_TYPE_PROPERTY_PAGE))
#define SCATOLA_PROPERTY_PAGE_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj), SCATOLA_TYPE_PROPERTY_PAGE, ScatolaPropertyPageClass))

typedef struct _ScatolaPropertyPage        ScatolaPropertyPage;
typedef struct _ScatolaPropertyPageDetails ScatolaPropertyPageDetails;
typedef struct _ScatolaPropertyPageClass   ScatolaPropertyPageClass;

struct _ScatolaPropertyPage {
    GObject parent;

    ScatolaPropertyPageDetails *details;
};

struct _ScatolaPropertyPageClass {
    GObjectClass parent;
};

GType             scatola_property_page_get_type  (void);
ScatolaPropertyPage *scatola_property_page_new       (const char *name,
                                                GtkWidget  *label,
                                                GtkWidget  *page);

/* ScatolaPropertyPage has the following properties:
 *   name (string)        - the identifier for the property page
 *   label (widget)       - the user-visible label of the property page
 *   page (widget)        - the property page to display
 */

G_END_DECLS

#endif
