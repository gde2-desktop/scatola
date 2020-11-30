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

#include <config.h>
#include "scatola-property-page.h"

#include "scatola-extension-i18n.h"

enum {
    PROP_0,
    PROP_NAME,
    PROP_LABEL,
    PROP_PAGE,
    LAST_PROP
};

struct _ScatolaPropertyPageDetails {
    char *name;
    GtkWidget *label;
    GtkWidget *page;
};

static GObjectClass *parent_class = NULL;

/**
 * SECTION:scatola-property-page
 * @title: ScatolaPropertyPage
 * @short_description: Property page descriptor object
 * @include: libscatola-extension/scatola-property-page.h
 *
 * #ScatolaPropertyPage is an object that describes a page in the file
 * properties dialog. Extensions can provide #ScatolaPropertyPage objects
 * by registering a #ScatolaPropertyPageProvider and returning them from
 * scatola_property_page_provider_get_pages(), which will be called by the
 * main application when creating file properties dialogs.
 */

/**
 * scatola_property_page_new:
 * @name: the identifier for the property page
 * @label: the user-visible label of the property page
 * @page: the property page to display
 *
 * Creates a new #ScatolaPropertyPage from page_widget.
 *
 * Returns: a newly created #ScatolaPropertyPage
 */
ScatolaPropertyPage *
scatola_property_page_new (const char *name,
                        GtkWidget *label,
                        GtkWidget *page_widget)
{
    ScatolaPropertyPage *page;

    g_return_val_if_fail (name != NULL, NULL);
    g_return_val_if_fail (label != NULL && GTK_IS_WIDGET (label), NULL);
    g_return_val_if_fail (page_widget != NULL && GTK_IS_WIDGET (page_widget),
                          NULL);

    page = g_object_new (SCATOLA_TYPE_PROPERTY_PAGE,
                         "name", name,
                         "label", label,
                         "page", page_widget,
                         NULL);

    return page;
}

static void
scatola_property_page_get_property (GObject *object,
                                 guint param_id,
                                 GValue *value,
                                 GParamSpec *pspec)
{
    ScatolaPropertyPage *page;

    page = SCATOLA_PROPERTY_PAGE (object);

    switch (param_id) {
    case PROP_NAME :
        g_value_set_string (value, page->details->name);
        break;
    case PROP_LABEL :
        g_value_set_object (value, page->details->label);
        break;
    case PROP_PAGE :
        g_value_set_object (value, page->details->page);
        break;
    default :
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, param_id, pspec);
        break;
    }
}

static void
scatola_property_page_set_property (GObject *object,
                                 guint param_id,
                                 const GValue *value,
                                 GParamSpec *pspec)
{
    ScatolaPropertyPage *page;

    page = SCATOLA_PROPERTY_PAGE (object);

    switch (param_id) {
    case PROP_NAME :
        g_free (page->details->name);
        page->details->name = g_strdup (g_value_get_string (value));
        g_object_notify (object, "name");
        break;
    case PROP_LABEL :
        if (page->details->label) {
            g_object_unref (page->details->label);
        }

        page->details->label = g_object_ref (g_value_get_object (value));
        g_object_notify (object, "label");
        break;
    case PROP_PAGE :
        if (page->details->page) {
            g_object_unref (page->details->page);
        }

        page->details->page = g_object_ref (g_value_get_object (value));
        g_object_notify (object, "page");
        break;
    default :
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, param_id, pspec);
        break;
    }
}

static void
scatola_property_page_dispose (GObject *object)
{
    ScatolaPropertyPage *page;

    page = SCATOLA_PROPERTY_PAGE (object);

    if (page->details->label) {
        g_object_unref (page->details->label);
        page->details->label = NULL;
    }
    if (page->details->page) {
        g_object_unref (page->details->page);
        page->details->page = NULL;
    }
}

static void
scatola_property_page_finalize (GObject *object)
{
    ScatolaPropertyPage *page;

    page = SCATOLA_PROPERTY_PAGE (object);

    g_free (page->details->name);

    g_free (page->details);

    G_OBJECT_CLASS (parent_class)->finalize (object);
}

static void
scatola_property_page_instance_init (ScatolaPropertyPage *page)
{
    page->details = g_new0 (ScatolaPropertyPageDetails, 1);
}

static void
scatola_property_page_class_init (ScatolaPropertyPageClass *class)
{
    parent_class = g_type_class_peek_parent (class);

    G_OBJECT_CLASS (class)->finalize = scatola_property_page_finalize;
    G_OBJECT_CLASS (class)->dispose = scatola_property_page_dispose;
    G_OBJECT_CLASS (class)->get_property = scatola_property_page_get_property;
    G_OBJECT_CLASS (class)->set_property = scatola_property_page_set_property;

    g_object_class_install_property (G_OBJECT_CLASS (class),
                                     PROP_NAME,
                                     g_param_spec_string ("name",
                                             "Name",
                                             "Name of the page",
                                             NULL,
                                             G_PARAM_CONSTRUCT_ONLY | G_PARAM_WRITABLE | G_PARAM_READABLE));
    g_object_class_install_property (G_OBJECT_CLASS (class),
                                     PROP_LABEL,
                                     g_param_spec_object ("label",
                                             "Label",
                                             "Label widget to display in the notebook tab",
                                             GTK_TYPE_WIDGET,
                                             G_PARAM_READWRITE));
    g_object_class_install_property (G_OBJECT_CLASS (class),
                                     PROP_PAGE,
                                     g_param_spec_object ("page",
                                             "Page",
                                             "Widget for the property page",
                                             GTK_TYPE_WIDGET,
                                             G_PARAM_READWRITE));
}

GType
scatola_property_page_get_type (void)
{
    static GType type = 0;

    if (!type) {
        const GTypeInfo info = {
            sizeof (ScatolaPropertyPageClass),
            NULL,
            NULL,
            (GClassInitFunc)scatola_property_page_class_init,
            NULL,
            NULL,
            sizeof (ScatolaPropertyPage),
            0,
            (GInstanceInitFunc)scatola_property_page_instance_init
        };

        type = g_type_register_static
               (G_TYPE_OBJECT,
                "ScatolaPropertyPage",
                &info, 0);
    }

    return type;
}
