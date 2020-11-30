/*
 *  Scatola
 *
 *  Copyright (C) 1999, 2000 Red Hat, Inc.
 *  Copyright (C) 1999, 2000, 2001 Eazel, Inc.
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
/* scatola-window.h: Interface of the main window object */

#ifndef SCATOLA_WINDOW_H
#define SCATOLA_WINDOW_H

#include <gtk/gtk.h>
#include <eel/eel-glib-extensions.h>
#include <libscatola-private/scatola-bookmark.h>
#include <libscatola-private/scatola-entry.h>
#include <libscatola-private/scatola-window-info.h>
#include <libscatola-private/scatola-search-directory.h>
#include "scatola-application.h"
#include "scatola-information-panel.h"
#include "scatola-side-pane.h"

#define SCATOLA_TYPE_WINDOW scatola_window_get_type()
#define SCATOLA_WINDOW(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), SCATOLA_TYPE_WINDOW, ScatolaWindow))
#define SCATOLA_WINDOW_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), SCATOLA_TYPE_WINDOW, ScatolaWindowClass))
#define SCATOLA_IS_WINDOW(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SCATOLA_TYPE_WINDOW))
#define SCATOLA_IS_WINDOW_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), SCATOLA_TYPE_WINDOW))
#define SCATOLA_WINDOW_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), SCATOLA_TYPE_WINDOW, ScatolaWindowClass))

#ifndef SCATOLA_WINDOW_DEFINED
#define SCATOLA_WINDOW_DEFINED
typedef struct ScatolaWindow ScatolaWindow;
#endif

#ifndef SCATOLA_WINDOW_SLOT_DEFINED
#define SCATOLA_WINDOW_SLOT_DEFINED
typedef struct ScatolaWindowSlot ScatolaWindowSlot;
#endif

typedef struct _ScatolaWindowPane      ScatolaWindowPane;

typedef struct ScatolaWindowSlotClass ScatolaWindowSlotClass;
typedef enum ScatolaWindowOpenSlotFlags ScatolaWindowOpenSlotFlags;

GType          scatola_window_slot_get_type (void);

typedef enum
{
    SCATOLA_WINDOW_NOT_SHOWN,
    SCATOLA_WINDOW_POSITION_SET,
    SCATOLA_WINDOW_SHOULD_SHOW
} ScatolaWindowShowState;

enum ScatolaWindowOpenSlotFlags
{
    SCATOLA_WINDOW_OPEN_SLOT_NONE = 0,
    SCATOLA_WINDOW_OPEN_SLOT_APPEND = 1
};

typedef struct ScatolaWindowDetails ScatolaWindowDetails;

typedef struct
{
    GtkWindowClass parent_spot;

    ScatolaWindowType window_type;
    const char *bookmarks_placeholder;

    /* Function pointers for overriding, without corresponding signals */

    char * (* get_title) (ScatolaWindow *window);
    void   (* sync_title) (ScatolaWindow *window,
                           ScatolaWindowSlot *slot);
    ScatolaIconInfo * (* get_icon) (ScatolaWindow *window,
                                 ScatolaWindowSlot *slot);

    void   (* sync_allow_stop) (ScatolaWindow *window,
                                ScatolaWindowSlot *slot);
    void   (* set_allow_up) (ScatolaWindow *window, gboolean allow);
    void   (* reload)              (ScatolaWindow *window);
    void   (* prompt_for_location) (ScatolaWindow *window, const char *initial);
    void   (* get_min_size) (ScatolaWindow *window, guint *default_width, guint *default_height);
    void   (* get_default_size) (ScatolaWindow *window, guint *default_width, guint *default_height);
    void   (* close) (ScatolaWindow *window);

    ScatolaWindowSlot * (* open_slot) (ScatolaWindowPane *pane,
                                    ScatolaWindowOpenSlotFlags flags);
    void                 (* close_slot) (ScatolaWindowPane *pane,
                                         ScatolaWindowSlot *slot);
    void                 (* set_active_slot) (ScatolaWindowPane *pane,
            ScatolaWindowSlot *slot);

    /* Signals used only for keybindings */
    gboolean (* go_up) (ScatolaWindow *window, gboolean close);
} ScatolaWindowClass;

struct ScatolaWindow
{
    GtkWindow parent_object;

    ScatolaWindowDetails *details;

    ScatolaApplication *application;
};

GType            scatola_window_get_type             (void);
void             scatola_window_show_window          (ScatolaWindow    *window);
void             scatola_window_close                (ScatolaWindow    *window);

void             scatola_window_connect_content_view (ScatolaWindow    *window,
        ScatolaView      *view);
void             scatola_window_disconnect_content_view (ScatolaWindow    *window,
        ScatolaView      *view);

void             scatola_window_go_to                (ScatolaWindow    *window,
        GFile             *location);
void             scatola_window_go_to_full           (ScatolaWindow    *window,
        GFile             *location,
        ScatolaWindowGoToCallback callback,
        gpointer           user_data);
void             scatola_window_go_to_with_selection (ScatolaWindow    *window,
        GFile             *location,
        GList             *new_selection);
void             scatola_window_go_home              (ScatolaWindow    *window);
void             scatola_window_new_tab              (ScatolaWindow    *window);
void             scatola_window_go_up                (ScatolaWindow    *window,
        gboolean           close_behind,
        gboolean           new_tab);
void             scatola_window_prompt_for_location  (ScatolaWindow    *window,
        const char        *initial);
void             scatola_window_display_error        (ScatolaWindow    *window,
        const char        *error_msg);
void		 scatola_window_reload		      (ScatolaWindow	 *window);

void             scatola_window_allow_reload         (ScatolaWindow    *window,
        gboolean           allow);
void             scatola_window_allow_up             (ScatolaWindow    *window,
        gboolean           allow);
void             scatola_window_allow_stop           (ScatolaWindow    *window,
        gboolean           allow);
GtkUIManager *   scatola_window_get_ui_manager       (ScatolaWindow    *window);

#endif
