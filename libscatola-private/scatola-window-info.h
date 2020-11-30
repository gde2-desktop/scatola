/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*-

   scatola-window-info.h: Interface for scatola windows

   Copyright (C) 2004 Red Hat Inc.

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public
   License along with this program; if not, write to the
   Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
   Boston, MA 02110-1301, USA.

   Author: Alexander Larsson <alexl@redhat.com>
*/

#ifndef SCATOLA_WINDOW_INFO_H
#define SCATOLA_WINDOW_INFO_H

#include <glib-object.h>
#include <libscatola-private/scatola-view.h>
#include <gtk/gtk.h>

#ifdef __cplusplus
extern "C" {
#endif

    typedef enum
    {
        SCATOLA_WINDOW_SHOW_HIDDEN_FILES_DEFAULT,
        SCATOLA_WINDOW_SHOW_HIDDEN_FILES_ENABLE,
        SCATOLA_WINDOW_SHOW_HIDDEN_FILES_DISABLE
    }
    ScatolaWindowShowHiddenFilesMode;


    typedef enum
    {
        SCATOLA_WINDOW_OPEN_ACCORDING_TO_MODE,
        SCATOLA_WINDOW_OPEN_IN_SPATIAL,
        SCATOLA_WINDOW_OPEN_IN_NAVIGATION
    } ScatolaWindowOpenMode;

    typedef enum
    {
        /* used in spatial mode */
        SCATOLA_WINDOW_OPEN_FLAG_CLOSE_BEHIND = 1<<0,
        /* used in navigation mode */
        SCATOLA_WINDOW_OPEN_FLAG_NEW_WINDOW = 1<<1,
        SCATOLA_WINDOW_OPEN_FLAG_NEW_TAB = 1<<2
    } ScatolaWindowOpenFlags;

    typedef	enum
    {
        SCATOLA_WINDOW_SPATIAL,
        SCATOLA_WINDOW_NAVIGATION,
        SCATOLA_WINDOW_DESKTOP
    } ScatolaWindowType;

#define SCATOLA_TYPE_WINDOW_INFO           (scatola_window_info_get_type ())
#define SCATOLA_WINDOW_INFO(obj)           (G_TYPE_CHECK_INSTANCE_CAST ((obj), SCATOLA_TYPE_WINDOW_INFO, ScatolaWindowInfo))
#define SCATOLA_IS_WINDOW_INFO(obj)        (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SCATOLA_TYPE_WINDOW_INFO))
#define SCATOLA_WINDOW_INFO_GET_IFACE(obj) (G_TYPE_INSTANCE_GET_INTERFACE ((obj), SCATOLA_TYPE_WINDOW_INFO, ScatolaWindowInfoIface))

#ifndef SCATOLA_WINDOW_DEFINED
#define SCATOLA_WINDOW_DEFINED
    /* Using ScatolaWindow for the vtable to make implementing this in
     * ScatolaWindow easier */
    typedef struct ScatolaWindow          ScatolaWindow;
#endif

#ifndef SCATOLA_WINDOW_SLOT_DEFINED
#define SCATOLA_WINDOW_SLOT_DEFINED
    typedef struct ScatolaWindowSlot      ScatolaWindowSlot;
#endif


    typedef ScatolaWindowSlot              ScatolaWindowSlotInfo;
    typedef ScatolaWindow                  ScatolaWindowInfo;

    typedef struct _ScatolaWindowInfoIface ScatolaWindowInfoIface;

    typedef void (* ScatolaWindowGoToCallback) (ScatolaWindow *window,
    					     GError *error,
    					     gpointer user_data);

    struct _ScatolaWindowInfoIface
    {
        GTypeInterface g_iface;

        /* signals: */

        void           (* loading_uri)              (ScatolaWindowInfo *window,
                const char        *uri);
        /* Emitted when the view in the window changes the selection */
        void           (* selection_changed)        (ScatolaWindowInfo *window);
        void           (* title_changed)            (ScatolaWindowInfo *window,
                const char         *title);
        void           (* hidden_files_mode_changed)(ScatolaWindowInfo *window);

        /* VTable: */
        /* A view calls this once after a load_location, once it starts loading the
         * directory. Might be called directly, or later on the mainloop.
         * This can also be called at any other time if the view needs to
         * re-load the location. But the view needs to call load_complete first if
         * its currently loading. */
        void (* report_load_underway) (ScatolaWindowInfo *window,
                                       ScatolaView *view);
        /* A view calls this once after reporting load_underway, when the location
           has been fully loaded, or when the load was stopped
           (by an error or by the user). */
        void (* report_load_complete) (ScatolaWindowInfo *window,
                                       ScatolaView *view);
        /* This can be called at any time when there has been a catastrophic failure of
           the view. It will result in the view being removed. */
        void (* report_view_failed)   (ScatolaWindowInfo *window,
                                       ScatolaView *view);
        void (* report_selection_changed) (ScatolaWindowInfo *window);

        /* Returns the number of selected items in the view */
        int  (* get_selection_count)  (ScatolaWindowInfo    *window);

        /* Returns a list of uris for th selected items in the view, caller frees it */
        GList *(* get_selection)      (ScatolaWindowInfo    *window);

        char * (* get_current_location)  (ScatolaWindowInfo *window);
        void   (* push_status)           (ScatolaWindowInfo *window,
                                          const char *status);
        char * (* get_title)             (ScatolaWindowInfo *window);
        GList *(* get_history)           (ScatolaWindowInfo *window);
        ScatolaWindowType
        (* get_window_type)       (ScatolaWindowInfo *window);
        ScatolaWindowShowHiddenFilesMode
        (* get_hidden_files_mode) (ScatolaWindowInfo *window);
        void   (* set_hidden_files_mode) (ScatolaWindowInfo *window,
                                          ScatolaWindowShowHiddenFilesMode mode);

        ScatolaWindowSlotInfo * (* get_active_slot) (ScatolaWindowInfo *window);
        ScatolaWindowSlotInfo * (* get_extra_slot)  (ScatolaWindowInfo *window);

        gboolean (* get_initiated_unmount) (ScatolaWindowInfo *window);
        void   (* set_initiated_unmount) (ScatolaWindowInfo *window,
                                          gboolean initiated_unmount);

        void   (* view_visible)        (ScatolaWindowInfo *window,
                                        ScatolaView *view);
        void   (* close_window)       (ScatolaWindowInfo *window);
        GtkUIManager *     (* get_ui_manager)   (ScatolaWindowInfo *window);
    };

    GType                             scatola_window_info_get_type                 (void);
    void                              scatola_window_info_report_load_underway     (ScatolaWindowInfo                *window,
            ScatolaView                      *view);
    void                              scatola_window_info_report_load_complete     (ScatolaWindowInfo                *window,
            ScatolaView                      *view);
    void                              scatola_window_info_report_view_failed       (ScatolaWindowInfo                *window,
            ScatolaView                      *view);
    void                              scatola_window_info_report_selection_changed (ScatolaWindowInfo                *window);
    ScatolaWindowSlotInfo *          scatola_window_info_get_active_slot          (ScatolaWindowInfo                *window);
    ScatolaWindowSlotInfo *          scatola_window_info_get_extra_slot           (ScatolaWindowInfo                *window);
    void                              scatola_window_info_view_visible             (ScatolaWindowInfo                *window,
            ScatolaView                      *view);
    void                              scatola_window_info_close                    (ScatolaWindowInfo                *window);
    void                              scatola_window_info_push_status              (ScatolaWindowInfo                *window,
            const char                        *status);
    ScatolaWindowType                scatola_window_info_get_window_type          (ScatolaWindowInfo                *window);
    char *                            scatola_window_info_get_title                (ScatolaWindowInfo                *window);
    GList *                           scatola_window_info_get_history              (ScatolaWindowInfo                *window);
    char *                            scatola_window_info_get_current_location     (ScatolaWindowInfo                *window);
    int                               scatola_window_info_get_selection_count      (ScatolaWindowInfo                *window);
    GList *                           scatola_window_info_get_selection            (ScatolaWindowInfo                *window);
    ScatolaWindowShowHiddenFilesMode scatola_window_info_get_hidden_files_mode    (ScatolaWindowInfo                *window);
    void                              scatola_window_info_set_hidden_files_mode    (ScatolaWindowInfo                *window,
            ScatolaWindowShowHiddenFilesMode  mode);
    gboolean                          scatola_window_info_get_initiated_unmount    (ScatolaWindowInfo                *window);
    void                              scatola_window_info_set_initiated_unmount    (ScatolaWindowInfo                *window,
            gboolean initiated_unmount);
    GtkUIManager *                    scatola_window_info_get_ui_manager           (ScatolaWindowInfo                *window);

#ifdef __cplusplus
}
#endif

#endif /* SCATOLA_WINDOW_INFO_H */
