/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/* scatola-bookmark.h - interface for individual bookmarks.

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

#ifndef SCATOLA_BOOKMARK_H
#define SCATOLA_BOOKMARK_H

#include <gtk/gtk.h>
#include <gio/gio.h>
typedef struct ScatolaBookmark ScatolaBookmark;

#define SCATOLA_TYPE_BOOKMARK scatola_bookmark_get_type()
#define SCATOLA_BOOKMARK(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), SCATOLA_TYPE_BOOKMARK, ScatolaBookmark))
#define SCATOLA_BOOKMARK_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), SCATOLA_TYPE_BOOKMARK, ScatolaBookmarkClass))
#define SCATOLA_IS_BOOKMARK(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SCATOLA_TYPE_BOOKMARK))
#define SCATOLA_IS_BOOKMARK_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), SCATOLA_TYPE_BOOKMARK))
#define SCATOLA_BOOKMARK_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), SCATOLA_TYPE_BOOKMARK, ScatolaBookmarkClass))

typedef struct ScatolaBookmarkDetails ScatolaBookmarkDetails;

struct ScatolaBookmark
{
    GObject object;
    ScatolaBookmarkDetails *details;
};

struct ScatolaBookmarkClass
{
    GObjectClass parent_class;

    /* Signals that clients can connect to. */

    /* The appearance_changed signal is emitted when the bookmark's
     * name or icon has changed.
     */
    void	(* appearance_changed) (ScatolaBookmark *bookmark);

    /* The contents_changed signal is emitted when the bookmark's
     * URI has changed.
     */
    void	(* contents_changed) (ScatolaBookmark *bookmark);
};

typedef struct ScatolaBookmarkClass ScatolaBookmarkClass;

GType                 scatola_bookmark_get_type               (void);
ScatolaBookmark *    scatola_bookmark_new                    (GFile *location,
        const char *name,
        gboolean has_custom_name,
        GIcon *icon);
ScatolaBookmark *    scatola_bookmark_copy                   (ScatolaBookmark      *bookmark);
char *                scatola_bookmark_get_name               (ScatolaBookmark      *bookmark);
GFile *               scatola_bookmark_get_location           (ScatolaBookmark      *bookmark);
char *                scatola_bookmark_get_uri                (ScatolaBookmark      *bookmark);
GIcon *               scatola_bookmark_get_icon               (ScatolaBookmark      *bookmark);
gboolean	      scatola_bookmark_get_has_custom_name    (ScatolaBookmark      *bookmark);
gboolean              scatola_bookmark_set_name               (ScatolaBookmark      *bookmark,
        const char            *new_name);
gboolean              scatola_bookmark_uri_known_not_to_exist (ScatolaBookmark      *bookmark);
int                   scatola_bookmark_compare_with           (gconstpointer          a,
        gconstpointer          b);
int                   scatola_bookmark_compare_uris           (gconstpointer          a,
        gconstpointer          b);

void                  scatola_bookmark_set_scroll_pos         (ScatolaBookmark      *bookmark,
        const char            *uri);
char *                scatola_bookmark_get_scroll_pos         (ScatolaBookmark      *bookmark);


/* Helper functions for displaying bookmarks */
GdkPixbuf *           scatola_bookmark_get_pixbuf             (ScatolaBookmark      *bookmark,
        GtkIconSize            icon_size);
GtkWidget *           scatola_bookmark_menu_item_new          (ScatolaBookmark      *bookmark);

#endif /* SCATOLA_BOOKMARK_H */
