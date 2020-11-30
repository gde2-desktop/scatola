/* -*- Mode: C; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 8 -*- */

/*
 *  Scatola
 *
 *  Copyright (C) 1999, 2000 Red Hat, Inc.
 *  Copyright (C) 1999, 2000, 2001 Eazel, Inc.
 *  Copyright (C) 2003 Ximian, Inc.
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
 *  Authors: Elliot Lee <sopwith@redhat.com>
 *           Darin Adler <darin@bentspoon.com>
 *
 */
/* scatola-navigation-window.h: Interface of the navigation window object */

#ifndef SCATOLA_NAVIGATION_WINDOW_H
#define SCATOLA_NAVIGATION_WINDOW_H

#include <gtk/gtk.h>
#include <eel/eel-glib-extensions.h>
#include <libscatola-private/scatola-bookmark.h>
#include <libscatola-private/scatola-sidebar.h>
#include "scatola-application.h"
#include "scatola-information-panel.h"
#include "scatola-side-pane.h"
#include "scatola-window.h"

#define SCATOLA_TYPE_NAVIGATION_WINDOW scatola_navigation_window_get_type()
#define SCATOLA_NAVIGATION_WINDOW(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), SCATOLA_TYPE_NAVIGATION_WINDOW, ScatolaNavigationWindow))
#define SCATOLA_NAVIGATION_WINDOW_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), SCATOLA_TYPE_NAVIGATION_WINDOW, ScatolaNavigationWindowClass))
#define SCATOLA_IS_NAVIGATION_WINDOW(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SCATOLA_TYPE_NAVIGATION_WINDOW))
#define SCATOLA_IS_NAVIGATION_WINDOW_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), SCATOLA_TYPE_NAVIGATION_WINDOW))
#define SCATOLA_NAVIGATION_WINDOW_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), SCATOLA_TYPE_NAVIGATION_WINDOW, ScatolaNavigationWindowClass))

typedef struct _ScatolaNavigationWindow        ScatolaNavigationWindow;
typedef struct _ScatolaNavigationWindowClass   ScatolaNavigationWindowClass;
typedef struct _ScatolaNavigationWindowDetails ScatolaNavigationWindowDetails;


struct _ScatolaNavigationWindow
{
    ScatolaWindow parent_object;

    ScatolaNavigationWindowDetails *details;

    /** UI stuff **/
    ScatolaSidePane *sidebar;

    /* Current views stuff */
    GList *sidebar_panels;
};


struct _ScatolaNavigationWindowClass
{
    ScatolaWindowClass parent_spot;
};

GType    scatola_navigation_window_get_type             (void);
void     scatola_navigation_window_allow_back           (ScatolaNavigationWindow *window,
        gboolean                  allow);
void     scatola_navigation_window_allow_forward        (ScatolaNavigationWindow *window,
        gboolean                  allow);
void     scatola_navigation_window_clear_back_list      (ScatolaNavigationWindow *window);
void     scatola_navigation_window_clear_forward_list   (ScatolaNavigationWindow *window);
void     scatola_forget_history                         (void);
gint     scatola_navigation_window_get_base_page_index  (ScatolaNavigationWindow *window);
void     scatola_navigation_window_hide_toolbar         (ScatolaNavigationWindow *window);
void     scatola_navigation_window_show_toolbar         (ScatolaNavigationWindow *window);
gboolean scatola_navigation_window_toolbar_showing      (ScatolaNavigationWindow *window);
void     scatola_navigation_window_hide_sidebar         (ScatolaNavigationWindow *window);
void     scatola_navigation_window_show_sidebar         (ScatolaNavigationWindow *window);
gboolean scatola_navigation_window_sidebar_showing      (ScatolaNavigationWindow *window);
void     scatola_navigation_window_add_sidebar_panel    (ScatolaNavigationWindow *window,
        ScatolaSidebar          *sidebar_panel);
void     scatola_navigation_window_remove_sidebar_panel (ScatolaNavigationWindow *window,
        ScatolaSidebar          *sidebar_panel);
void     scatola_navigation_window_hide_status_bar      (ScatolaNavigationWindow *window);
void     scatola_navigation_window_show_status_bar      (ScatolaNavigationWindow *window);
gboolean scatola_navigation_window_status_bar_showing   (ScatolaNavigationWindow *window);
void     scatola_navigation_window_back_or_forward      (ScatolaNavigationWindow *window,
        gboolean                  back,
        guint                     distance,
        gboolean                  new_tab);
void     scatola_navigation_window_show_search          (ScatolaNavigationWindow *window);
void     scatola_navigation_window_unset_focus_widget   (ScatolaNavigationWindow *window);
void     scatola_navigation_window_hide_search          (ScatolaNavigationWindow *window);
void     scatola_navigation_window_set_search_button	 (ScatolaNavigationWindow *window,
        gboolean		    state);
void     scatola_navigation_window_restore_focus_widget (ScatolaNavigationWindow *window);
void     scatola_navigation_window_split_view_on        (ScatolaNavigationWindow *window);
void     scatola_navigation_window_split_view_off       (ScatolaNavigationWindow *window);
gboolean scatola_navigation_window_split_view_showing   (ScatolaNavigationWindow *window);

gboolean scatola_navigation_window_is_in_temporary_navigation_bar (GtkWidget *widget,
        ScatolaNavigationWindow *window);
gboolean scatola_navigation_window_is_in_temporary_search_bar (GtkWidget *widget,
        ScatolaNavigationWindow *window);

#endif
