/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/*
 * Scatola
 *
 * Copyright (C) 2002 Sun Microsystems, Inc.
 *
 * Scatola is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * Scatola is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 * Author: Dave Camp <dave@ximian.com>
 */

/* scatola-tree-view-drag-dest.h: Handles drag and drop for treeviews which
 *                                 contain a hierarchy of files
 */

#ifndef SCATOLA_TREE_VIEW_DRAG_DEST_H
#define SCATOLA_TREE_VIEW_DRAG_DEST_H

#include <gtk/gtk.h>

#include "scatola-file.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SCATOLA_TYPE_TREE_VIEW_DRAG_DEST	(scatola_tree_view_drag_dest_get_type ())
#define SCATOLA_TREE_VIEW_DRAG_DEST(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), SCATOLA_TYPE_TREE_VIEW_DRAG_DEST, ScatolaTreeViewDragDest))
#define SCATOLA_TREE_VIEW_DRAG_DEST_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST ((klass), SCATOLA_TYPE_TREE_VIEW_DRAG_DEST, ScatolaTreeViewDragDestClass))
#define SCATOLA_IS_TREE_VIEW_DRAG_DEST(obj)		(G_TYPE_INSTANCE_CHECK_TYPE ((obj), SCATOLA_TYPE_TREE_VIEW_DRAG_DEST))
#define SCATOLA_IS_TREE_VIEW_DRAG_DEST_CLASS(klass)	(G_TYPE_CLASS_CHECK_CLASS_TYPE ((klass), SCATOLA_TYPE_TREE_VIEW_DRAG_DEST))

    typedef struct _ScatolaTreeViewDragDest        ScatolaTreeViewDragDest;
    typedef struct _ScatolaTreeViewDragDestClass   ScatolaTreeViewDragDestClass;
    typedef struct _ScatolaTreeViewDragDestDetails ScatolaTreeViewDragDestDetails;

    struct _ScatolaTreeViewDragDest
    {
        GObject parent;

        ScatolaTreeViewDragDestDetails *details;
    };

    struct _ScatolaTreeViewDragDestClass
    {
        GObjectClass parent;

        char *(*get_root_uri) (ScatolaTreeViewDragDest *dest);
        ScatolaFile *(*get_file_for_path) (ScatolaTreeViewDragDest *dest,
                                        GtkTreePath *path);
        void (*move_copy_items) (ScatolaTreeViewDragDest *dest,
                                 const GList *item_uris,
                                 const char *target_uri,
                                 GdkDragAction action,
                                 int x,
                                 int y);
        void (* handle_netscape_url) (ScatolaTreeViewDragDest *dest,
                                      const char *url,
                                      const char *target_uri,
                                      GdkDragAction action,
                                      int x,
                                      int y);
        void (* handle_uri_list) (ScatolaTreeViewDragDest *dest,
                                  const char *uri_list,
                                  const char *target_uri,
                                  GdkDragAction action,
                                  int x,
                                  int y);
        void (* handle_text)    (ScatolaTreeViewDragDest *dest,
                                 const char *text,
                                 const char *target_uri,
                                 GdkDragAction action,
                                 int x,
                                 int y);
        void (* handle_raw)    (ScatolaTreeViewDragDest *dest,
                                char *raw_data,
                                int length,
                                const char *target_uri,
                                const char *direct_save_uri,
                                GdkDragAction action,
                                int x,
                                int y);
    };

    GType                     scatola_tree_view_drag_dest_get_type (void);
    ScatolaTreeViewDragDest *scatola_tree_view_drag_dest_new      (GtkTreeView *tree_view);

#ifdef __cplusplus
}
#endif

#endif
