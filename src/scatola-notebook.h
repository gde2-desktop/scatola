/*
 *  Copyright © 2002 Christophe Fergeau
 *  Copyright © 2003 Marco Pesenti Gritti
 *  Copyright © 2003, 2004 Christian Persch
 *    (ephy-notebook.c)
 *
 *  Copyright © 2008 Free Software Foundation, Inc.
 *    (scatola-notebook.c)
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 *  $Id: scatola-notebook.h 8210 2008-04-11 20:05:25Z chpe $
 */

#ifndef SCATOLA_NOTEBOOK_H
#define SCATOLA_NOTEBOOK_H

#include <glib.h>

#include <gtk/gtk.h>
#include "scatola-window-slot.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SCATOLA_TYPE_NOTEBOOK		(scatola_notebook_get_type ())
#define SCATOLA_NOTEBOOK(o)		(G_TYPE_CHECK_INSTANCE_CAST ((o), SCATOLA_TYPE_NOTEBOOK, ScatolaNotebook))
#define SCATOLA_NOTEBOOK_CLASS(k)		(G_TYPE_CHECK_CLASS_CAST((k), SCATOLA_TYPE_NOTEBOOK, ScatolaNotebookClass))
#define SCATOLA_IS_NOTEBOOK(o)		(G_TYPE_CHECK_INSTANCE_TYPE ((o), SCATOLA_TYPE_NOTEBOOK))
#define SCATOLA_IS_NOTEBOOK_CLASS(k)	(G_TYPE_CHECK_CLASS_TYPE ((k), SCATOLA_TYPE_NOTEBOOK))
#define SCATOLA_NOTEBOOK_GET_CLASS(o)	(G_TYPE_INSTANCE_GET_CLASS ((o), SCATOLA_TYPE_NOTEBOOK, ScatolaNotebookClass))

    typedef struct _ScatolaNotebookClass	ScatolaNotebookClass;
    typedef struct _ScatolaNotebook		ScatolaNotebook;
    typedef struct _ScatolaNotebookPrivate	ScatolaNotebookPrivate;

    struct _ScatolaNotebook
    {
        GtkNotebook parent;

        /*< private >*/
        ScatolaNotebookPrivate *priv;
    };

    struct _ScatolaNotebookClass
    {
        GtkNotebookClass parent_class;

        /* Signals */
        void	 (* tab_close_request)  (ScatolaNotebook *notebook,
                                         ScatolaWindowSlot *slot);
    };

    GType		scatola_notebook_get_type		(void);

    int		scatola_notebook_add_tab	(ScatolaNotebook *nb,
                                     ScatolaWindowSlot *slot,
                                     int position,
                                     gboolean jump_to);

    void		scatola_notebook_set_show_tabs	(ScatolaNotebook *nb,
            gboolean show_tabs);

    void		scatola_notebook_set_dnd_enabled (ScatolaNotebook *nb,
            gboolean enabled);
    void		scatola_notebook_sync_tab_label (ScatolaNotebook *nb,
            ScatolaWindowSlot *slot);
    void		scatola_notebook_sync_loading   (ScatolaNotebook *nb,
            ScatolaWindowSlot *slot);

    void		scatola_notebook_reorder_current_child_relative (ScatolaNotebook *notebook,
            int offset);
    void		scatola_notebook_set_current_page_relative (ScatolaNotebook *notebook,
            int offset);

    gboolean        scatola_notebook_can_reorder_current_child_relative (ScatolaNotebook *notebook,
            int offset);
    gboolean        scatola_notebook_can_set_current_page_relative (ScatolaNotebook *notebook,
            int offset);

#ifdef __cplusplus
}
#endif

#endif /* SCATOLA_NOTEBOOK_H */

