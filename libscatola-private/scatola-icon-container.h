/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/* gde2-icon-container.h - Icon container widget.

   Copyright (C) 1999, 2000 Free Software Foundation
   Copyright (C) 2000 Eazel, Inc.

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

   Authors: Ettore Perazzoli <ettore@gnu.org>, Darin Adler <darin@bentspoon.com>
*/

#ifndef SCATOLA_ICON_CONTAINER_H
#define SCATOLA_ICON_CONTAINER_H

#include <eel/eel-canvas.h>
#include <libscatola-private/scatola-icon-info.h>

#define SCATOLA_TYPE_ICON_CONTAINER scatola_icon_container_get_type()
#define SCATOLA_ICON_CONTAINER(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), SCATOLA_TYPE_ICON_CONTAINER, ScatolaIconContainer))
#define SCATOLA_ICON_CONTAINER_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), SCATOLA_TYPE_ICON_CONTAINER, ScatolaIconContainerClass))
#define SCATOLA_IS_ICON_CONTAINER(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SCATOLA_TYPE_ICON_CONTAINER))
#define SCATOLA_IS_ICON_CONTAINER_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), SCATOLA_TYPE_ICON_CONTAINER))
#define SCATOLA_ICON_CONTAINER_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), SCATOLA_TYPE_ICON_CONTAINER, ScatolaIconContainerClass))


#define SCATOLA_ICON_CONTAINER_ICON_DATA(pointer) \
	((ScatolaIconData *) (pointer))

typedef struct ScatolaIconData ScatolaIconData;

typedef void (* ScatolaIconCallback) (ScatolaIconData *icon_data,
                                   gpointer callback_data);

typedef struct
{
    int x;
    int y;
    double scale;
} ScatolaIconPosition;

typedef enum
{
    SCATOLA_ICON_LAYOUT_L_R_T_B,
    SCATOLA_ICON_LAYOUT_R_L_T_B,
    SCATOLA_ICON_LAYOUT_T_B_L_R,
    SCATOLA_ICON_LAYOUT_T_B_R_L
} ScatolaIconLayoutMode;

typedef enum
{
    SCATOLA_ICON_LABEL_POSITION_UNDER,
    SCATOLA_ICON_LABEL_POSITION_BESIDE
} ScatolaIconLabelPosition;

#define	SCATOLA_ICON_CONTAINER_TYPESELECT_FLUSH_DELAY 1000000

typedef struct ScatolaIconContainerDetails ScatolaIconContainerDetails;

typedef struct
{
    EelCanvas canvas;
    ScatolaIconContainerDetails *details;
} ScatolaIconContainer;

typedef struct
{
    EelCanvasClass parent_slot;

    /* Operations on the container. */
    int          (* button_press) 	          (ScatolaIconContainer *container,
            GdkEventButton *event);
    void         (* context_click_background) (ScatolaIconContainer *container,
            GdkEventButton *event);
    void         (* middle_click) 		  (ScatolaIconContainer *container,
                                           GdkEventButton *event);

    /* Operations on icons. */
    void         (* activate)	  	  (ScatolaIconContainer *container,
                                       ScatolaIconData *data);
    void         (* activate_alternate)       (ScatolaIconContainer *container,
            ScatolaIconData *data);
    void         (* context_click_selection)  (ScatolaIconContainer *container,
            GdkEventButton *event);
    void	     (* move_copy_items)	  (ScatolaIconContainer *container,
                                           const GList *item_uris,
                                           GdkPoint *relative_item_points,
                                           const char *target_uri,
                                           GdkDragAction action,
                                           int x,
                                           int y);
    void	     (* handle_netscape_url)	  (ScatolaIconContainer *container,
            const char *url,
            const char *target_uri,
            GdkDragAction action,
            int x,
            int y);
    void	     (* handle_uri_list)    	  (ScatolaIconContainer *container,
            const char *uri_list,
            const char *target_uri,
            GdkDragAction action,
            int x,
            int y);
    void	     (* handle_text)		  (ScatolaIconContainer *container,
                                           const char *text,
                                           const char *target_uri,
                                           GdkDragAction action,
                                           int x,
                                           int y);
    void	     (* handle_raw)		  (ScatolaIconContainer *container,
                                       char *raw_data,
                                       int length,
                                       const char *target_uri,
                                       const char *direct_save_uri,
                                       GdkDragAction action,
                                       int x,
                                       int y);

    /* Queries on the container for subclass/client.
     * These must be implemented. The default "do nothing" is not good enough.
     */
    char *	     (* get_container_uri)	  (ScatolaIconContainer *container);

    /* Queries on icons for subclass/client.
     * These must be implemented. The default "do nothing" is not
     * good enough, these are _not_ signals.
     */
    ScatolaIconInfo *(* get_icon_images)     (ScatolaIconContainer *container,
                                           ScatolaIconData *data,
                                           int icon_size,
                                           GList **emblem_pixbufs,
                                           char **embedded_text,
                                           gboolean for_drag_accept,
                                           gboolean need_large_embeddded_text,
                                           gboolean *embedded_text_needs_loading,
                                           gboolean *has_window_open);
    void         (* get_icon_text)            (ScatolaIconContainer *container,
            ScatolaIconData *data,
            char **editable_text,
            char **additional_text,
            gboolean include_invisible);
    char *       (* get_icon_description)     (ScatolaIconContainer *container,
            ScatolaIconData *data);
    int          (* compare_icons)            (ScatolaIconContainer *container,
            ScatolaIconData *icon_a,
            ScatolaIconData *icon_b);
    int          (* compare_icons_by_name)    (ScatolaIconContainer *container,
            ScatolaIconData *icon_a,
            ScatolaIconData *icon_b);
    void         (* freeze_updates)           (ScatolaIconContainer *container);
    void         (* unfreeze_updates)         (ScatolaIconContainer *container);
    void         (* start_monitor_top_left)   (ScatolaIconContainer *container,
            ScatolaIconData *data,
            gconstpointer client,
            gboolean large_text);
    void         (* stop_monitor_top_left)    (ScatolaIconContainer *container,
            ScatolaIconData *data,
            gconstpointer client);
    void         (* prioritize_thumbnailing)  (ScatolaIconContainer *container,
            ScatolaIconData *data);

    /* Queries on icons for subclass/client.
     * These must be implemented => These are signals !
     * The default "do nothing" is not good enough.
     */
    gboolean     (* can_accept_item)	  (ScatolaIconContainer *container,
                                           ScatolaIconData *target,
                                           const char *item_uri);
    gboolean     (* get_stored_icon_position) (ScatolaIconContainer *container,
            ScatolaIconData *data,
            ScatolaIconPosition *position);
    char *       (* get_icon_uri)             (ScatolaIconContainer *container,
            ScatolaIconData *data);
    char *       (* get_icon_drop_target_uri) (ScatolaIconContainer *container,
            ScatolaIconData *data);

    /* If icon data is NULL, the layout timestamp of the container should be retrieved.
     * That is the time when the container displayed a fully loaded directory with
     * all icon positions assigned.
     *
     * If icon data is not NULL, the position timestamp of the icon should be retrieved.
     * That is the time when the file (i.e. icon data payload) was last displayed in a
     * fully loaded directory with all icon positions assigned.
     */
    gboolean     (* get_stored_layout_timestamp) (ScatolaIconContainer *container,
            ScatolaIconData *data,
            time_t *time);
    /* If icon data is NULL, the layout timestamp of the container should be stored.
     * If icon data is not NULL, the position timestamp of the container should be stored.
     */
    gboolean     (* store_layout_timestamp) (ScatolaIconContainer *container,
            ScatolaIconData *data,
            const time_t *time);

    /* Notifications for the whole container. */
    void	     (* band_select_started)	  (ScatolaIconContainer *container);
    void	     (* band_select_ended)	  (ScatolaIconContainer *container);
    void         (* selection_changed) 	  (ScatolaIconContainer *container);
    void         (* layout_changed)           (ScatolaIconContainer *container);

    /* Notifications for icons. */
    void         (* icon_position_changed)    (ScatolaIconContainer *container,
            ScatolaIconData *data,
            const ScatolaIconPosition *position);
    void         (* icon_text_changed)        (ScatolaIconContainer *container,
            ScatolaIconData *data,
            const char *text);
    void         (* renaming_icon)            (ScatolaIconContainer *container,
            GtkWidget *renaming_widget);
    void	     (* icon_stretch_started)     (ScatolaIconContainer *container,
            ScatolaIconData *data);
    void	     (* icon_stretch_ended)       (ScatolaIconContainer *container,
            ScatolaIconData *data);
    int	     (* preview)		  (ScatolaIconContainer *container,
                                   ScatolaIconData *data,
                                   gboolean start_flag);
    void         (* icon_added)               (ScatolaIconContainer *container,
            ScatolaIconData *data);
    void         (* icon_removed)             (ScatolaIconContainer *container,
            ScatolaIconData *data);
    void         (* cleared)                  (ScatolaIconContainer *container);
    gboolean     (* start_interactive_search) (ScatolaIconContainer *container);
} ScatolaIconContainerClass;

/* GtkObject */
GType             scatola_icon_container_get_type                      (void);
GtkWidget *       scatola_icon_container_new                           (void);


/* adding, removing, and managing icons */
void              scatola_icon_container_clear                         (ScatolaIconContainer  *view);
gboolean          scatola_icon_container_add                           (ScatolaIconContainer  *view,
        ScatolaIconData       *data);
void              scatola_icon_container_layout_now                    (ScatolaIconContainer *container);
gboolean          scatola_icon_container_remove                        (ScatolaIconContainer  *view,
        ScatolaIconData       *data);
void              scatola_icon_container_for_each                      (ScatolaIconContainer  *view,
        ScatolaIconCallback    callback,
        gpointer                callback_data);
void              scatola_icon_container_request_update                (ScatolaIconContainer  *view,
        ScatolaIconData       *data);
void              scatola_icon_container_request_update_all            (ScatolaIconContainer  *container);
void              scatola_icon_container_reveal                        (ScatolaIconContainer  *container,
        ScatolaIconData       *data);
gboolean          scatola_icon_container_is_empty                      (ScatolaIconContainer  *container);
ScatolaIconData *scatola_icon_container_get_first_visible_icon        (ScatolaIconContainer  *container);
void              scatola_icon_container_scroll_to_icon                (ScatolaIconContainer  *container,
        ScatolaIconData       *data);

void              scatola_icon_container_begin_loading                 (ScatolaIconContainer  *container);
void              scatola_icon_container_end_loading                   (ScatolaIconContainer  *container,
        gboolean                all_icons_added);

/* control the layout */
gboolean          scatola_icon_container_is_auto_layout                (ScatolaIconContainer  *container);
void              scatola_icon_container_set_auto_layout               (ScatolaIconContainer  *container,
        gboolean                auto_layout);
gboolean          scatola_icon_container_is_tighter_layout             (ScatolaIconContainer  *container);
void              scatola_icon_container_set_tighter_layout            (ScatolaIconContainer  *container,
        gboolean                tighter_layout);

gboolean          scatola_icon_container_is_keep_aligned               (ScatolaIconContainer  *container);
void              scatola_icon_container_set_keep_aligned              (ScatolaIconContainer  *container,
        gboolean                keep_aligned);
void              scatola_icon_container_set_layout_mode               (ScatolaIconContainer  *container,
        ScatolaIconLayoutMode  mode);
void              scatola_icon_container_set_label_position            (ScatolaIconContainer  *container,
        ScatolaIconLabelPosition pos);
void              scatola_icon_container_sort                          (ScatolaIconContainer  *container);
void              scatola_icon_container_freeze_icon_positions         (ScatolaIconContainer  *container);

int               scatola_icon_container_get_max_layout_lines           (ScatolaIconContainer  *container);
int               scatola_icon_container_get_max_layout_lines_for_pango (ScatolaIconContainer  *container);

void              scatola_icon_container_set_highlighted_for_clipboard (ScatolaIconContainer  *container,
        GList                  *clipboard_icon_data);

/* operations on all icons */
void              scatola_icon_container_unselect_all                  (ScatolaIconContainer  *view);
void              scatola_icon_container_select_all                    (ScatolaIconContainer  *view);


/* operations on the selection */
GList     *       scatola_icon_container_get_selection                 (ScatolaIconContainer  *view);
void			  scatola_icon_container_invert_selection				(ScatolaIconContainer  *view);
void              scatola_icon_container_set_selection                 (ScatolaIconContainer  *view,
        GList                  *selection);
GArray    *       scatola_icon_container_get_selected_icon_locations   (ScatolaIconContainer  *view);
gboolean          scatola_icon_container_has_stretch_handles           (ScatolaIconContainer  *container);
gboolean          scatola_icon_container_is_stretched                  (ScatolaIconContainer  *container);
void              scatola_icon_container_show_stretch_handles          (ScatolaIconContainer  *container);
void              scatola_icon_container_unstretch                     (ScatolaIconContainer  *container);
void              scatola_icon_container_start_renaming_selected_item  (ScatolaIconContainer  *container,
        gboolean                select_all);

/* options */
ScatolaZoomLevel scatola_icon_container_get_zoom_level                (ScatolaIconContainer  *view);
void              scatola_icon_container_set_zoom_level                (ScatolaIconContainer  *view,
        int                     new_zoom_level);
void              scatola_icon_container_set_single_click_mode         (ScatolaIconContainer  *container,
        gboolean                single_click_mode);
void              scatola_icon_container_enable_linger_selection       (ScatolaIconContainer  *view,
        gboolean                enable);
gboolean          scatola_icon_container_get_is_fixed_size             (ScatolaIconContainer  *container);
void              scatola_icon_container_set_is_fixed_size             (ScatolaIconContainer  *container,
        gboolean                is_fixed_size);
gboolean          scatola_icon_container_get_is_desktop                (ScatolaIconContainer  *container);
void              scatola_icon_container_set_is_desktop                (ScatolaIconContainer  *container,
        gboolean                is_desktop);
void              scatola_icon_container_reset_scroll_region           (ScatolaIconContainer  *container);
void              scatola_icon_container_set_font                      (ScatolaIconContainer  *container,
        const char             *font);
void              scatola_icon_container_set_font_size_table           (ScatolaIconContainer  *container,
        const int               font_size_table[SCATOLA_ZOOM_LEVEL_LARGEST + 1]);
void              scatola_icon_container_set_margins                   (ScatolaIconContainer  *container,
        int                     left_margin,
        int                     right_margin,
        int                     top_margin,
        int                     bottom_margin);
void              scatola_icon_container_set_use_drop_shadows          (ScatolaIconContainer  *container,
        gboolean                use_drop_shadows);
char*             scatola_icon_container_get_icon_description          (ScatolaIconContainer  *container,
        ScatolaIconData       *data);
gboolean          scatola_icon_container_get_allow_moves               (ScatolaIconContainer  *container);
void              scatola_icon_container_set_allow_moves               (ScatolaIconContainer  *container,
        gboolean                allow_moves);
void		  scatola_icon_container_set_forced_icon_size		(ScatolaIconContainer  *container,
        int                     forced_icon_size);
void		  scatola_icon_container_set_all_columns_same_width	(ScatolaIconContainer  *container,
        gboolean                all_columns_same_width);

gboolean	  scatola_icon_container_is_layout_rtl			(ScatolaIconContainer  *container);
gboolean	  scatola_icon_container_is_layout_vertical		(ScatolaIconContainer  *container);

gboolean          scatola_icon_container_get_store_layout_timestamps   (ScatolaIconContainer  *container);
void              scatola_icon_container_set_store_layout_timestamps   (ScatolaIconContainer  *container,
        gboolean                store_layout);

void              scatola_icon_container_widget_to_file_operation_position (ScatolaIconContainer *container,
        GdkPoint              *position);


#define CANVAS_WIDTH(container,allocation) ((allocation.width	  \
				- container->details->left_margin \
				- container->details->right_margin) \
				/  EEL_CANVAS (container)->pixels_per_unit)

#define CANVAS_HEIGHT(container,allocation) ((allocation.height \
			 - container->details->top_margin \
			 - container->details->bottom_margin) \
			 / EEL_CANVAS (container)->pixels_per_unit)

#endif /* SCATOLA_ICON_CONTAINER_H */
