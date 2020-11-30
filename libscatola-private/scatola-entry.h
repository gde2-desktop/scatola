/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/* ScatolaEntry: one-line text editing widget. This consists of bug fixes
 * and other improvements to GtkEntry, and all the changes could be rolled
 * into GtkEntry some day.
 *
 * Copyright (C) 2000 Eazel, Inc.
 *
 * Author: John Sullivan <sullivan@eazel.com>
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

#ifndef SCATOLA_ENTRY_H
#define SCATOLA_ENTRY_H

#include <gtk/gtk.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SCATOLA_TYPE_ENTRY scatola_entry_get_type()
#define SCATOLA_ENTRY(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), SCATOLA_TYPE_ENTRY, ScatolaEntry))
#define SCATOLA_ENTRY_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), SCATOLA_TYPE_ENTRY, ScatolaEntryClass))
#define SCATOLA_IS_ENTRY(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SCATOLA_TYPE_ENTRY))
#define SCATOLA_IS_ENTRY_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), SCATOLA_TYPE_ENTRY))
#define SCATOLA_ENTRY_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), SCATOLA_TYPE_ENTRY, ScatolaEntryClass))

    typedef struct ScatolaEntryDetails ScatolaEntryDetails;

    typedef struct
    {
        GtkEntry parent;
        ScatolaEntryDetails *details;
    } ScatolaEntry;

    typedef struct
    {
        GtkEntryClass parent_class;

        void (*user_changed)      (ScatolaEntry *entry);
        void (*selection_changed) (ScatolaEntry *entry);
    } ScatolaEntryClass;

    GType       scatola_entry_get_type                 (void);
    GtkWidget  *scatola_entry_new                      (void);
    void        scatola_entry_set_text                 (ScatolaEntry *entry,
            const char    *text);
    void        scatola_entry_select_all               (ScatolaEntry *entry);
    void        scatola_entry_select_all_at_idle       (ScatolaEntry *entry);
    void        scatola_entry_set_special_tab_handling (ScatolaEntry *entry,
            gboolean       special_tab_handling);

#ifdef __cplusplus
}
#endif

#endif /* SCATOLA_ENTRY_H */
