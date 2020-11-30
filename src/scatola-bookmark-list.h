/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/*
 * Scatola
 *
 * Copyright (C) 1999, 2000 Eazel, Inc.
 *
 * Scatola is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * Scatola is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * Authors: John Sullivan <sullivan@eazel.com>
 */

/* scatola-bookmark-list.h - interface for centralized list of bookmarks.
 */

#ifndef SCATOLA_BOOKMARK_LIST_H
#define SCATOLA_BOOKMARK_LIST_H

#include <libscatola-private/scatola-bookmark.h>
#include <gio/gio.h>

typedef struct ScatolaBookmarkList ScatolaBookmarkList;
typedef struct ScatolaBookmarkListClass ScatolaBookmarkListClass;

#define SCATOLA_TYPE_BOOKMARK_LIST scatola_bookmark_list_get_type()
#define SCATOLA_BOOKMARK_LIST(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), SCATOLA_TYPE_BOOKMARK_LIST, ScatolaBookmarkList))
#define SCATOLA_BOOKMARK_LIST_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), SCATOLA_TYPE_BOOKMARK_LIST, ScatolaBookmarkListClass))
#define SCATOLA_IS_BOOKMARK_LIST(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SCATOLA_TYPE_BOOKMARK_LIST))
#define SCATOLA_IS_BOOKMARK_LIST_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), SCATOLA_TYPE_BOOKMARK_LIST))
#define SCATOLA_BOOKMARK_LIST_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), SCATOLA_TYPE_BOOKMARK_LIST, ScatolaBookmarkListClass))

struct ScatolaBookmarkList
{
    GObject object;

    GList *list;
    GFileMonitor *monitor;
    GQueue *pending_ops;
};

struct ScatolaBookmarkListClass
{
    GObjectClass parent_class;
    void (* contents_changed) (ScatolaBookmarkList *bookmarks);
};

GType                   scatola_bookmark_list_get_type            (void);
ScatolaBookmarkList *  scatola_bookmark_list_new                 (void);
void                    scatola_bookmark_list_append              (ScatolaBookmarkList   *bookmarks,
        ScatolaBookmark *bookmark);
gboolean                scatola_bookmark_list_contains            (ScatolaBookmarkList   *bookmarks,
        ScatolaBookmark *bookmark);
void                    scatola_bookmark_list_delete_item_at      (ScatolaBookmarkList   *bookmarks,
        guint                   index);
void                    scatola_bookmark_list_delete_items_with_uri (ScatolaBookmarkList *bookmarks,
        const char		   *uri);
void                    scatola_bookmark_list_insert_item         (ScatolaBookmarkList   *bookmarks,
        ScatolaBookmark *bookmark,
        guint                   index);
guint                   scatola_bookmark_list_length              (ScatolaBookmarkList   *bookmarks);
ScatolaBookmark *      scatola_bookmark_list_item_at             (ScatolaBookmarkList   *bookmarks,
        guint                   index);
void                    scatola_bookmark_list_move_item           (ScatolaBookmarkList *bookmarks,
        guint                 index,
        guint                 destination);
void                    scatola_bookmark_list_set_window_geometry (ScatolaBookmarkList   *bookmarks,
        const char             *geometry);
const char *            scatola_bookmark_list_get_window_geometry (ScatolaBookmarkList   *bookmarks);

#endif /* SCATOLA_BOOKMARK_LIST_H */
