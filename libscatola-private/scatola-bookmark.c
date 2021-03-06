/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/* scatola-bookmark.c - implementation of individual bookmarks.

   Copyright (C) 1999, 2000 Eazel, Inc.

   The Gde2 Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   The Gde2 Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with the Gde2 Library; see the file COPYING.LIB.  If not,
   write to the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
   Boston, MA 02110-1301, USA.

   Authors: John Sullivan <sullivan@eazel.com>
*/

#include <config.h>
#include "scatola-bookmark.h"

#include "scatola-file.h"
#include <eel/eel-gdk-pixbuf-extensions.h>
#include <eel/eel-gtk-extensions.h>
#include <eel/eel-gtk-macros.h>
#include <eel/eel-vfs-extensions.h>
#include <gtk/gtk.h>
#include <gio/gio.h>
#include <libscatola-private/scatola-file.h>
#include <libscatola-private/scatola-icon-names.h>

enum
{
    APPEARANCE_CHANGED,
    CONTENTS_CHANGED,
    LAST_SIGNAL
};

#define ELLIPSISED_MENU_ITEM_MIN_CHARS  32

static guint signals[LAST_SIGNAL] = { 0 };

struct ScatolaBookmarkDetails
{
    char *name;
    gboolean has_custom_name;
    GFile *location;
    GIcon *icon;
    ScatolaFile *file;

    char *scroll_file;
};

static void	  scatola_bookmark_connect_file	  (ScatolaBookmark	 *file);
static void	  scatola_bookmark_disconnect_file	  (ScatolaBookmark	 *file);

G_DEFINE_TYPE (ScatolaBookmark, scatola_bookmark, G_TYPE_OBJECT);

/* GObject methods.  */

static void
scatola_bookmark_finalize (GObject *object)
{
    ScatolaBookmark *bookmark;

    g_assert (SCATOLA_IS_BOOKMARK (object));

    bookmark = SCATOLA_BOOKMARK (object);

    scatola_bookmark_disconnect_file (bookmark);

    g_free (bookmark->details->name);
    g_object_unref (bookmark->details->location);
    if (bookmark->details->icon)
    {
        g_object_unref (bookmark->details->icon);
    }
    g_free (bookmark->details->scroll_file);
    g_free (bookmark->details);

    G_OBJECT_CLASS (scatola_bookmark_parent_class)->finalize (object);
}

/* Initialization.  */

static void
scatola_bookmark_class_init (ScatolaBookmarkClass *class)
{
    G_OBJECT_CLASS (class)->finalize = scatola_bookmark_finalize;

    signals[APPEARANCE_CHANGED] =
        g_signal_new ("appearance_changed",
                      G_TYPE_FROM_CLASS (class),
                      G_SIGNAL_RUN_LAST,
                      G_STRUCT_OFFSET (ScatolaBookmarkClass, appearance_changed),
                      NULL, NULL,
                      g_cclosure_marshal_VOID__VOID,
                      G_TYPE_NONE, 0);

    signals[CONTENTS_CHANGED] =
        g_signal_new ("contents_changed",
                      G_TYPE_FROM_CLASS (class),
                      G_SIGNAL_RUN_LAST,
                      G_STRUCT_OFFSET (ScatolaBookmarkClass, contents_changed),
                      NULL, NULL,
                      g_cclosure_marshal_VOID__VOID,
                      G_TYPE_NONE, 0);

}

static void
scatola_bookmark_init (ScatolaBookmark *bookmark)
{
    bookmark->details = g_new0 (ScatolaBookmarkDetails, 1);
}

/**
 * scatola_bookmark_compare_with:
 *
 * Check whether two bookmarks are considered identical.
 * @a: first ScatolaBookmark*.
 * @b: second ScatolaBookmark*.
 *
 * Return value: 0 if @a and @b have same name and uri, 1 otherwise
 * (GCompareFunc style)
 **/
int
scatola_bookmark_compare_with (gconstpointer a, gconstpointer b)
{
    ScatolaBookmark *bookmark_a;
    ScatolaBookmark *bookmark_b;

    g_return_val_if_fail (SCATOLA_IS_BOOKMARK (a), 1);
    g_return_val_if_fail (SCATOLA_IS_BOOKMARK (b), 1);

    bookmark_a = SCATOLA_BOOKMARK (a);
    bookmark_b = SCATOLA_BOOKMARK (b);

    if (g_strcmp0 (bookmark_a->details->name,
                    bookmark_b->details->name) != 0)
    {
        return 1;
    }

    if (!g_file_equal (bookmark_a->details->location,
                       bookmark_b->details->location))
    {
        return 1;
    }

    return 0;
}

/**
 * scatola_bookmark_compare_uris:
 *
 * Check whether the uris of two bookmarks are for the same location.
 * @a: first ScatolaBookmark*.
 * @b: second ScatolaBookmark*.
 *
 * Return value: 0 if @a and @b have matching uri, 1 otherwise
 * (GCompareFunc style)
 **/
int
scatola_bookmark_compare_uris (gconstpointer a, gconstpointer b)
{
    ScatolaBookmark *bookmark_a;
    ScatolaBookmark *bookmark_b;

    g_return_val_if_fail (SCATOLA_IS_BOOKMARK (a), 1);
    g_return_val_if_fail (SCATOLA_IS_BOOKMARK (b), 1);

    bookmark_a = SCATOLA_BOOKMARK (a);
    bookmark_b = SCATOLA_BOOKMARK (b);

    return !g_file_equal (bookmark_a->details->location,
                          bookmark_b->details->location);
}

ScatolaBookmark *
scatola_bookmark_copy (ScatolaBookmark *bookmark)
{
    g_return_val_if_fail (SCATOLA_IS_BOOKMARK (bookmark), NULL);

    return scatola_bookmark_new (
               bookmark->details->location,
               bookmark->details->name,
               bookmark->details->has_custom_name,
               bookmark->details->icon);
}

char *
scatola_bookmark_get_name (ScatolaBookmark *bookmark)
{
    g_return_val_if_fail(SCATOLA_IS_BOOKMARK (bookmark), NULL);

    return g_strdup (bookmark->details->name);
}


gboolean
scatola_bookmark_get_has_custom_name (ScatolaBookmark *bookmark)
{
    g_return_val_if_fail(SCATOLA_IS_BOOKMARK (bookmark), FALSE);

    return (bookmark->details->has_custom_name);
}


GdkPixbuf *
scatola_bookmark_get_pixbuf (ScatolaBookmark *bookmark,
                          GtkIconSize stock_size)
{
    GdkPixbuf *result;
    GIcon *icon;
    ScatolaIconInfo *info;
    int pixel_size;


    g_return_val_if_fail (SCATOLA_IS_BOOKMARK (bookmark), NULL);

    icon = scatola_bookmark_get_icon (bookmark);
    if (icon == NULL)
    {
        return NULL;
    }

    pixel_size = scatola_get_icon_size_for_stock_size (stock_size);
    info = scatola_icon_info_lookup (icon, pixel_size);
    result = scatola_icon_info_get_pixbuf_at_size (info, pixel_size);
    g_object_unref (info);

    g_object_unref (icon);

    return result;
}

GIcon *
scatola_bookmark_get_icon (ScatolaBookmark *bookmark)
{
    g_return_val_if_fail (SCATOLA_IS_BOOKMARK (bookmark), NULL);

    /* Try to connect a file in case file exists now but didn't earlier. */
    scatola_bookmark_connect_file (bookmark);

    if (bookmark->details->icon)
    {
        return g_object_ref (bookmark->details->icon);
    }
    return NULL;
}

GFile *
scatola_bookmark_get_location (ScatolaBookmark *bookmark)
{
    g_return_val_if_fail(SCATOLA_IS_BOOKMARK (bookmark), NULL);

    /* Try to connect a file in case file exists now but didn't earlier.
     * This allows a bookmark to update its image properly in the case
     * where a new file appears with the same URI as a previously-deleted
     * file. Calling connect_file here means that attempts to activate the
     * bookmark will update its image if possible.
     */
    scatola_bookmark_connect_file (bookmark);

    return g_object_ref (bookmark->details->location);
}

char *
scatola_bookmark_get_uri (ScatolaBookmark *bookmark)
{
    GFile *file;
    char *uri;

    file = scatola_bookmark_get_location (bookmark);
    uri = g_file_get_uri (file);
    g_object_unref (file);
    return uri;
}


/**
 * scatola_bookmark_set_name:
 *
 * Change the user-displayed name of a bookmark.
 * @new_name: The new user-displayed name for this bookmark, mustn't be NULL.
 *
 * Returns: TRUE if the name changed else FALSE.
 **/
gboolean
scatola_bookmark_set_name (ScatolaBookmark *bookmark, const char *new_name)
{
    g_return_val_if_fail (new_name != NULL, FALSE);
    g_return_val_if_fail (SCATOLA_IS_BOOKMARK (bookmark), FALSE);

    if (g_strcmp0 (new_name, bookmark->details->name) == 0)
    {
        return FALSE;
    }
    else if (!bookmark->details->has_custom_name)
    {
        bookmark->details->has_custom_name = TRUE;
    }

    g_free (bookmark->details->name);
    bookmark->details->name = g_strdup (new_name);

    g_signal_emit (bookmark, signals[APPEARANCE_CHANGED], 0);

    if (bookmark->details->has_custom_name)
    {
        g_signal_emit (bookmark, signals[CONTENTS_CHANGED], 0);
    }

    return TRUE;
}

static gboolean
scatola_bookmark_icon_is_different (ScatolaBookmark *bookmark,
                                 GIcon *new_icon)
{
    g_assert (SCATOLA_IS_BOOKMARK (bookmark));
    g_assert (new_icon != NULL);

    if (bookmark->details->icon == NULL)
    {
        return TRUE;
    }

    return !g_icon_equal (bookmark->details->icon, new_icon) != 0;
}

/**
 * Update icon if there's a better one available.
 * Return TRUE if the icon changed.
 */
static gboolean
scatola_bookmark_update_icon (ScatolaBookmark *bookmark)
{
    GIcon *new_icon;

    g_assert (SCATOLA_IS_BOOKMARK (bookmark));

    if (bookmark->details->file == NULL)
    {
        return FALSE;
    }

    if (!scatola_file_is_local (bookmark->details->file))
    {
        /* never update icons for remote bookmarks */
        return FALSE;
    }

    if (!scatola_file_is_not_yet_confirmed (bookmark->details->file) &&
            scatola_file_check_if_ready (bookmark->details->file,
                                      SCATOLA_FILE_ATTRIBUTES_FOR_ICON))
    {
        new_icon = scatola_file_get_gicon (bookmark->details->file, 0);
        if (scatola_bookmark_icon_is_different (bookmark, new_icon))
        {
            if (bookmark->details->icon)
            {
                g_object_unref (bookmark->details->icon);
            }
            bookmark->details->icon = new_icon;
            return TRUE;
        }
        g_object_unref (new_icon);
    }

    return FALSE;
}

static void
bookmark_file_changed_callback (ScatolaFile *file, ScatolaBookmark *bookmark)
{
    GFile *location;
    gboolean should_emit_appearance_changed_signal;
    gboolean should_emit_contents_changed_signal;
    char *display_name;

    g_assert (SCATOLA_IS_FILE (file));
    g_assert (SCATOLA_IS_BOOKMARK (bookmark));
    g_assert (file == bookmark->details->file);

    should_emit_appearance_changed_signal = FALSE;
    should_emit_contents_changed_signal = FALSE;
    location = scatola_file_get_location (file);

    if (!g_file_equal (bookmark->details->location, location) &&
            !scatola_file_is_in_trash (file))
    {
        g_object_unref (bookmark->details->location);
        bookmark->details->location = location;
        should_emit_contents_changed_signal = TRUE;
    }
    else
    {
        g_object_unref (location);
    }

    if (scatola_file_is_gone (file) ||
            scatola_file_is_in_trash (file))
    {
        /* The file we were monitoring has been trashed, deleted,
         * or moved in a way that we didn't notice. We should make
         * a spanking new ScatolaFile object for this
         * location so if a new file appears in this place
         * we will notice. However, we can't immediately do so
         * because creating a new ScatolaFile directly as a result
         * of noticing a file goes away may trigger i/o on that file
         * again, noticeing it is gone, leading to a loop.
         * So, the new ScatolaFile is created when the bookmark
         * is used again. However, this is not really a problem, as
         * we don't want to change the icon or anything about the
         * bookmark just because its not there anymore.
         */
        scatola_bookmark_disconnect_file (bookmark);
    }
    else if (scatola_bookmark_update_icon (bookmark))
    {
        /* File hasn't gone away, but it has changed
         * in a way that affected its icon.
         */
        should_emit_appearance_changed_signal = TRUE;
    }

    if (!bookmark->details->has_custom_name)
    {
        display_name = scatola_file_get_display_name (file);

        if (g_strcmp0 (bookmark->details->name, display_name) != 0)
        {
            g_free (bookmark->details->name);
            bookmark->details->name = display_name;
            should_emit_appearance_changed_signal = TRUE;
        }
        else
        {
            g_free (display_name);
        }
    }

    if (should_emit_appearance_changed_signal)
    {
        g_signal_emit (bookmark, signals[APPEARANCE_CHANGED], 0);
    }

    if (should_emit_contents_changed_signal)
    {
        g_signal_emit (bookmark, signals[CONTENTS_CHANGED], 0);
    }
}

/**
 * scatola_bookmark_set_icon_to_default:
 *
 * Reset the icon to either the missing bookmark icon or the generic
 * bookmark icon, depending on whether the file still exists.
 */
static void
scatola_bookmark_set_icon_to_default (ScatolaBookmark *bookmark)
{
    GIcon *icon, *emblemed_icon, *folder;
    GEmblem *emblem;

    if (bookmark->details->icon)
    {
        g_object_unref (bookmark->details->icon);
    }

    folder = g_themed_icon_new (SCATOLA_ICON_FOLDER);

    if (scatola_bookmark_uri_known_not_to_exist (bookmark))
    {
        icon = g_themed_icon_new ("dialog-warning");
        emblem = g_emblem_new (icon);

        emblemed_icon = g_emblemed_icon_new (folder, emblem);

        g_object_unref (emblem);
        g_object_unref (icon);
        g_object_unref (folder);

        folder = emblemed_icon;
    }

    bookmark->details->icon = folder;
}

static void
scatola_bookmark_disconnect_file (ScatolaBookmark *bookmark)
{
    g_assert (SCATOLA_IS_BOOKMARK (bookmark));

    if (bookmark->details->file != NULL)
    {
        g_signal_handlers_disconnect_by_func (bookmark->details->file,
                                              G_CALLBACK (bookmark_file_changed_callback),
                                              bookmark);
        scatola_file_unref (bookmark->details->file);
        bookmark->details->file = NULL;
    }

    if (bookmark->details->icon != NULL)
    {
        g_object_unref (bookmark->details->icon);
        bookmark->details->icon = NULL;
    }
}

static void
scatola_bookmark_connect_file (ScatolaBookmark *bookmark)
{
    char *display_name;

    g_assert (SCATOLA_IS_BOOKMARK (bookmark));

    if (bookmark->details->file != NULL)
    {
        return;
    }

    if (!scatola_bookmark_uri_known_not_to_exist (bookmark))
    {
        bookmark->details->file = scatola_file_get (bookmark->details->location);
        g_assert (!scatola_file_is_gone (bookmark->details->file));

        g_signal_connect_object (bookmark->details->file, "changed",
                                 G_CALLBACK (bookmark_file_changed_callback), bookmark, 0);
    }

    /* Set icon based on available information; don't force network i/o
     * to get any currently unknown information.
     */
    if (!scatola_bookmark_update_icon (bookmark))
    {
        if (bookmark->details->icon == NULL || bookmark->details->file == NULL)
        {
            scatola_bookmark_set_icon_to_default (bookmark);
        }
    }

    if (!bookmark->details->has_custom_name &&
            bookmark->details->file &&
            scatola_file_check_if_ready (bookmark->details->file, SCATOLA_FILE_ATTRIBUTE_INFO))
    {
        display_name = scatola_file_get_display_name (bookmark->details->file);
        if (g_strcmp0 (bookmark->details->name, display_name) != 0)
        {
            g_free (bookmark->details->name);
            bookmark->details->name = display_name;
        }
        else
        {
            g_free (display_name);
        }
    }
}

ScatolaBookmark *
scatola_bookmark_new (GFile *location, const char *name, gboolean has_custom_name,
                   GIcon *icon)
{
    ScatolaBookmark *new_bookmark;

    new_bookmark = SCATOLA_BOOKMARK (g_object_new (SCATOLA_TYPE_BOOKMARK, NULL));
    g_object_ref_sink (new_bookmark);

    new_bookmark->details->name = g_strdup (name);
    new_bookmark->details->location = g_object_ref (location);
    new_bookmark->details->has_custom_name = has_custom_name;
    if (icon)
    {
        new_bookmark->details->icon = g_object_ref (icon);
    }

    scatola_bookmark_connect_file (new_bookmark);

    return new_bookmark;
}

static GtkWidget *
create_image_widget_for_bookmark (ScatolaBookmark *bookmark)
{
    GdkPixbuf *pixbuf;
    GtkWidget *widget;

    pixbuf = scatola_bookmark_get_pixbuf (bookmark, GTK_ICON_SIZE_MENU);
    if (pixbuf == NULL)
    {
        return NULL;
    }

    widget = gtk_image_new_from_pixbuf (pixbuf);

    g_object_unref (pixbuf);
    return widget;
}

/**
 * scatola_bookmark_menu_item_new:
 *
 * Return a menu item representing a bookmark.
 * @bookmark: The bookmark the menu item represents.
 * Return value: A newly-created bookmark, not yet shown.
 **/
GtkWidget *
scatola_bookmark_menu_item_new (ScatolaBookmark *bookmark)
{
    GtkWidget *menu_item;
    GtkWidget *image_widget;
    GtkLabel *label;

    menu_item = gtk_image_menu_item_new_with_label (bookmark->details->name);
    label = GTK_LABEL (gtk_bin_get_child (GTK_BIN (menu_item)));
    gtk_label_set_use_underline (label, FALSE);
    gtk_label_set_ellipsize (label, PANGO_ELLIPSIZE_END);
    gtk_label_set_max_width_chars (label, ELLIPSISED_MENU_ITEM_MIN_CHARS);

    image_widget = create_image_widget_for_bookmark (bookmark);
    if (image_widget != NULL)
    {
        gtk_widget_show (image_widget);
        gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (menu_item),
                                       image_widget);
    }

    return menu_item;
}

gboolean
scatola_bookmark_uri_known_not_to_exist (ScatolaBookmark *bookmark)
{
    char *path_name;
    gboolean exists;

    /* Convert to a path, returning FALSE if not local. */
    if (!g_file_is_native (bookmark->details->location))
    {
        return FALSE;
    }
    path_name = g_file_get_path (bookmark->details->location);

    /* Now check if the file exists (sync. call OK because it is local). */
    exists = g_file_test (path_name, G_FILE_TEST_EXISTS);
    g_free (path_name);

    return !exists;
}

void
scatola_bookmark_set_scroll_pos (ScatolaBookmark      *bookmark,
                              const char            *uri)
{
    g_free (bookmark->details->scroll_file);
    bookmark->details->scroll_file = g_strdup (uri);
}

char *
scatola_bookmark_get_scroll_pos (ScatolaBookmark      *bookmark)
{
    return g_strdup (bookmark->details->scroll_file);
}
