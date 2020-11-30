#ifndef SCATOLA_ICON_INFO_H
#define SCATOLA_ICON_INFO_H

#include <glib-object.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <gdk/gdk.h>
#include <gio/gio.h>
#include <gtk/gtk.h>

#ifdef __cplusplus
extern "C" {
#endif

    /* Names for Scatola's different zoom levels, from tiniest items to largest items */
    typedef enum {
        SCATOLA_ZOOM_LEVEL_SMALLEST,
        SCATOLA_ZOOM_LEVEL_SMALLER,
        SCATOLA_ZOOM_LEVEL_SMALL,
        SCATOLA_ZOOM_LEVEL_STANDARD,
        SCATOLA_ZOOM_LEVEL_LARGE,
        SCATOLA_ZOOM_LEVEL_LARGER,
        SCATOLA_ZOOM_LEVEL_LARGEST
    }
    ScatolaZoomLevel;

#define SCATOLA_ZOOM_LEVEL_N_ENTRIES (SCATOLA_ZOOM_LEVEL_LARGEST + 1)

    /* Nominal icon sizes for each Scatola zoom level.
     * This scheme assumes that icons are designed to
     * fit in a square space, though each image needn't
     * be square. Since individual icons can be stretched,
     * each icon is not constrained to this nominal size.
     */
#define SCATOLA_ICON_SIZE_SMALLEST	16
#define SCATOLA_ICON_SIZE_SMALLER	24
#define SCATOLA_ICON_SIZE_SMALL	32
#define SCATOLA_ICON_SIZE_STANDARD	48
#define SCATOLA_ICON_SIZE_LARGE	72
#define SCATOLA_ICON_SIZE_LARGER	96
#define SCATOLA_ICON_SIZE_LARGEST     192

    /* Maximum size of an icon that the icon factory will ever produce */
#define SCATOLA_ICON_MAXIMUM_SIZE     320

    typedef struct _ScatolaIconInfo      ScatolaIconInfo;
    typedef struct _ScatolaIconInfoClass ScatolaIconInfoClass;


#define SCATOLA_TYPE_ICON_INFO                 (scatola_icon_info_get_type ())
#define SCATOLA_ICON_INFO(obj)                 (G_TYPE_CHECK_INSTANCE_CAST ((obj), SCATOLA_TYPE_ICON_INFO, ScatolaIconInfo))
#define SCATOLA_ICON_INFO_CLASS(klass)         (G_TYPE_CHECK_CLASS_CAST ((klass), SCATOLA_TYPE_ICON_INFO, ScatolaIconInfoClass))
#define SCATOLA_IS_ICON_INFO(obj)              (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SCATOLA_TYPE_ICON_INFO))
#define SCATOLA_IS_ICON_INFO_CLASS(klass)      (G_TYPE_CHECK_CLASS_TYPE ((klass), SCATOLA_TYPE_ICON_INFO))
#define SCATOLA_ICON_INFO_GET_CLASS(obj)       (G_TYPE_INSTANCE_GET_CLASS ((obj), SCATOLA_TYPE_ICON_INFO, ScatolaIconInfoClass))


    GType    scatola_icon_info_get_type (void) G_GNUC_CONST;

    ScatolaIconInfo *    scatola_icon_info_new_for_pixbuf               (GdkPixbuf         *pixbuf);
    ScatolaIconInfo *    scatola_icon_info_lookup                       (GIcon             *icon,
            int                size);
    ScatolaIconInfo *    scatola_icon_info_lookup_from_name             (const char        *name,
            int                size);
    ScatolaIconInfo *    scatola_icon_info_lookup_from_path             (const char        *path,
            int                size);
    gboolean              scatola_icon_info_is_fallback                  (ScatolaIconInfo  *icon);
    GdkPixbuf *           scatola_icon_info_get_pixbuf                   (ScatolaIconInfo  *icon);
    GdkPixbuf *           scatola_icon_info_get_pixbuf_nodefault         (ScatolaIconInfo  *icon);
    GdkPixbuf *           scatola_icon_info_get_pixbuf_nodefault_at_size (ScatolaIconInfo  *icon,
            gsize              forced_size);
    GdkPixbuf *           scatola_icon_info_get_pixbuf_at_size           (ScatolaIconInfo  *icon,
            gsize              forced_size);
    gboolean              scatola_icon_info_get_embedded_rect            (ScatolaIconInfo  *icon,
            GdkRectangle      *rectangle);
    gboolean              scatola_icon_info_get_attach_points            (ScatolaIconInfo  *icon,
            GdkPoint         **points,
            gint              *n_points);
    const char* scatola_icon_info_get_display_name(ScatolaIconInfo* icon);
    const char* scatola_icon_info_get_used_name(ScatolaIconInfo* icon);

    void                  scatola_icon_info_clear_caches                 (void);

    /* Relationship between zoom levels and icons sizes. */
    guint scatola_get_icon_size_for_zoom_level          (ScatolaZoomLevel  zoom_level);
    float scatola_get_relative_icon_size_for_zoom_level (ScatolaZoomLevel  zoom_level);

    guint scatola_icon_get_larger_icon_size             (guint              size);
    guint scatola_icon_get_smaller_icon_size            (guint              size);

    gint  scatola_get_icon_size_for_stock_size          (GtkIconSize        size);
    guint scatola_icon_get_emblem_size_for_icon_size    (guint              size);

gboolean scatola_icon_theme_can_render              (GThemedIcon *icon);
GIcon * scatola_user_special_directory_get_gicon (GUserDirectory directory);



#ifdef __cplusplus
}
#endif

#endif /* SCATOLA_ICON_INFO_H */

