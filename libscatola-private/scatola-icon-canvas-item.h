/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/* Scatola - Icon canvas item class for icon container.
 *
 * Copyright (C) 2000 Eazel, Inc.
 *
 * Author: Andy Hertzfeld <andy@eazel.com>
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
 */

#ifndef SCATOLA_ICON_CANVAS_ITEM_H
#define SCATOLA_ICON_CANVAS_ITEM_H

#include <eel/eel-canvas.h>
#include <eel/eel-art-extensions.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SCATOLA_TYPE_ICON_CANVAS_ITEM scatola_icon_canvas_item_get_type()
#define SCATOLA_ICON_CANVAS_ITEM(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), SCATOLA_TYPE_ICON_CANVAS_ITEM, ScatolaIconCanvasItem))
#define SCATOLA_ICON_CANVAS_ITEM_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), SCATOLA_TYPE_ICON_CANVAS_ITEM, ScatolaIconCanvasItemClass))
#define SCATOLA_IS_ICON_CANVAS_ITEM(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SCATOLA_TYPE_ICON_CANVAS_ITEM))
#define SCATOLA_IS_ICON_CANVAS_ITEM_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), SCATOLA_TYPE_ICON_CANVAS_ITEM))
#define SCATOLA_ICON_CANVAS_ITEM_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), SCATOLA_TYPE_ICON_CANVAS_ITEM, ScatolaIconCanvasItemClass))

    typedef struct ScatolaIconCanvasItem ScatolaIconCanvasItem;
    typedef struct ScatolaIconCanvasItemClass ScatolaIconCanvasItemClass;
    typedef struct ScatolaIconCanvasItemDetails ScatolaIconCanvasItemDetails;

    struct ScatolaIconCanvasItem
    {
        EelCanvasItem item;
        ScatolaIconCanvasItemDetails *details;
        gpointer user_data;
    };

    struct ScatolaIconCanvasItemClass
    {
        EelCanvasItemClass parent_class;
    };

    /* not namespaced due to their length */
    typedef enum
    {
        BOUNDS_USAGE_FOR_LAYOUT,
        BOUNDS_USAGE_FOR_ENTIRE_ITEM,
        BOUNDS_USAGE_FOR_DISPLAY
    } ScatolaIconCanvasItemBoundsUsage;

    /* GObject */
    GType       scatola_icon_canvas_item_get_type                 (void);

    /* attributes */
    void        scatola_icon_canvas_item_set_image                (ScatolaIconCanvasItem       *item,
            GdkPixbuf                    *image);
#if GTK_CHECK_VERSION(3,0,0)
    cairo_surface_t* scatola_icon_canvas_item_get_drag_surface    (ScatolaIconCanvasItem       *item);
#else
    GdkPixmap * scatola_icon_canvas_item_get_image                (ScatolaIconCanvasItem       *item,
    								GdkBitmap                **mask,
    								GdkColormap		 *colormap);
#endif
    void        scatola_icon_canvas_item_set_emblems              (ScatolaIconCanvasItem       *item,
            GList                        *emblem_pixbufs);
    void        scatola_icon_canvas_item_set_show_stretch_handles (ScatolaIconCanvasItem       *item,
            gboolean                      show_stretch_handles);
    void        scatola_icon_canvas_item_set_attach_points        (ScatolaIconCanvasItem       *item,
            GdkPoint                     *attach_points,
            int                           n_attach_points);
    void        scatola_icon_canvas_item_set_embedded_text_rect   (ScatolaIconCanvasItem       *item,
            const GdkRectangle           *text_rect);
    void        scatola_icon_canvas_item_set_embedded_text        (ScatolaIconCanvasItem       *item,
            const char                   *text);
    double      scatola_icon_canvas_item_get_max_text_width       (ScatolaIconCanvasItem       *item);
    const char *scatola_icon_canvas_item_get_editable_text        (ScatolaIconCanvasItem       *icon_item);
    void        scatola_icon_canvas_item_set_renaming             (ScatolaIconCanvasItem       *icon_item,
            gboolean                      state);

    /* geometry and hit testing */
    gboolean    scatola_icon_canvas_item_hit_test_rectangle       (ScatolaIconCanvasItem       *item,
            EelIRect                      canvas_rect);
    gboolean    scatola_icon_canvas_item_hit_test_stretch_handles (ScatolaIconCanvasItem       *item,
            EelDPoint                     world_point,
            GtkCornerType                *corner);
    void        scatola_icon_canvas_item_invalidate_label         (ScatolaIconCanvasItem       *item);
    void        scatola_icon_canvas_item_invalidate_label_size    (ScatolaIconCanvasItem       *item);
    EelDRect    scatola_icon_canvas_item_get_icon_rectangle       (const ScatolaIconCanvasItem *item);
    EelDRect    scatola_icon_canvas_item_get_text_rectangle       (ScatolaIconCanvasItem       *item,
            gboolean                      for_layout);
    void        scatola_icon_canvas_item_get_bounds_for_layout    (ScatolaIconCanvasItem       *item,
            double *x1, double *y1, double *x2, double *y2);
    void        scatola_icon_canvas_item_get_bounds_for_entire_item (ScatolaIconCanvasItem       *item,
            double *x1, double *y1, double *x2, double *y2);
    void        scatola_icon_canvas_item_update_bounds            (ScatolaIconCanvasItem       *item,
            double i2w_dx, double i2w_dy);
    void        scatola_icon_canvas_item_set_is_visible           (ScatolaIconCanvasItem       *item,
            gboolean                      visible);
    /* whether the entire label text must be visible at all times */
    void        scatola_icon_canvas_item_set_entire_text          (ScatolaIconCanvasItem       *icon_item,
            gboolean                      entire_text);

#ifdef __cplusplus
}
#endif

#endif /* SCATOLA_ICON_CANVAS_ITEM_H */
