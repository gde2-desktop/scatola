/* scatola-pathbar.h
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 *
 */

#ifndef SCATOLA_PATHBAR_H
#define SCATOLA_PATHBAR_H

#include <gtk/gtk.h>
#include <gio/gio.h>

typedef struct _ScatolaPathBar      ScatolaPathBar;
typedef struct _ScatolaPathBarClass ScatolaPathBarClass;


#define SCATOLA_TYPE_PATH_BAR                 (scatola_path_bar_get_type ())
#define SCATOLA_PATH_BAR(obj)                 (G_TYPE_CHECK_INSTANCE_CAST ((obj), SCATOLA_TYPE_PATH_BAR, ScatolaPathBar))
#define SCATOLA_PATH_BAR_CLASS(klass)         (G_TYPE_CHECK_CLASS_CAST ((klass), SCATOLA_TYPE_PATH_BAR, ScatolaPathBarClass))
#define SCATOLA_IS_PATH_BAR(obj)              (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SCATOLA_TYPE_PATH_BAR))
#define SCATOLA_IS_PATH_BAR_CLASS(klass)      (G_TYPE_CHECK_CLASS_TYPE ((klass), SCATOLA_TYPE_PATH_BAR))
#define SCATOLA_PATH_BAR_GET_CLASS(obj)       (G_TYPE_INSTANCE_GET_CLASS ((obj), SCATOLA_TYPE_PATH_BAR, ScatolaPathBarClass))

struct _ScatolaPathBar
{
    GtkContainer parent;

    GFile *root_path;
    GFile *home_path;
    GFile *desktop_path;

    GFile *current_path;
    gpointer current_button_data;

    GList *button_list;
    GList *first_scrolled_button;
    GList *fake_root;
    GtkWidget *up_slider_button;
    GtkWidget *down_slider_button;
    guint settings_signal_id;
    gint icon_size;
    gint16 slider_width;
    gint16 spacing;
    gint16 button_offset;
    guint timer;
    guint slider_visible : 1;
    guint need_timer : 1;
    guint ignore_click : 1;

    unsigned int drag_slider_timeout;
    gboolean drag_slider_timeout_for_up_button;
};

struct _ScatolaPathBarClass
{
    GtkContainerClass parent_class;

    void (* path_clicked)   (ScatolaPathBar  *path_bar,
                             GFile             *location);
    void (* path_set)       (ScatolaPathBar  *path_bar,
                             GFile             *location);
};

GType    scatola_path_bar_get_type (void) G_GNUC_CONST;

gboolean scatola_path_bar_set_path    (ScatolaPathBar *path_bar, GFile *file);
GFile *  scatola_path_bar_get_path_for_button (ScatolaPathBar *path_bar,
        GtkWidget       *button);
void     scatola_path_bar_clear_buttons (ScatolaPathBar *path_bar);

void     scatola_path_bar_up              (ScatolaPathBar *path_bar);
void     scatola_path_bar_down            (ScatolaPathBar *path_bar);

GtkWidget * scatola_path_bar_get_button_from_button_list_entry (gpointer entry);

#endif /* SCATOLA_PATHBAR_H */
