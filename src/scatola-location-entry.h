/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/*
 * Scatola
 *
 * Copyright (C) 2000 Eazel, Inc.
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
 * You should have received a copy of the GNU General Public
 * License along with this program; see the file COPYING.  If not,
 * write to the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 * Author: Maciej Stachowiak <mjs@eazel.com>
 *         Ettore Perazzoli <ettore@gnu.org>
 */

#ifndef SCATOLA_LOCATION_ENTRY_H
#define SCATOLA_LOCATION_ENTRY_H

#include <libscatola-private/scatola-entry.h>

#define SCATOLA_TYPE_LOCATION_ENTRY scatola_location_entry_get_type()
#define SCATOLA_LOCATION_ENTRY(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), SCATOLA_TYPE_LOCATION_ENTRY, ScatolaLocationEntry))
#define SCATOLA_LOCATION_ENTRY_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), SCATOLA_TYPE_LOCATION_ENTRY, ScatolaLocationEntryClass))
#define SCATOLA_IS_LOCATION_ENTRY(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SCATOLA_TYPE_LOCATION_ENTRY))
#define SCATOLA_IS_LOCATION_ENTRY_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), SCATOLA_TYPE_LOCATION_ENTRY))
#define SCATOLA_LOCATION_ENTRY_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), SCATOLA_TYPE_LOCATION_ENTRY, ScatolaLocationEntryClass))

typedef struct ScatolaLocationEntryDetails ScatolaLocationEntryDetails;

typedef struct ScatolaLocationEntry
{
    ScatolaEntry parent;
    ScatolaLocationEntryDetails *details;
} ScatolaLocationEntry;

typedef struct
{
    ScatolaEntryClass parent_class;
} ScatolaLocationEntryClass;

typedef enum
{
    SCATOLA_LOCATION_ENTRY_ACTION_GOTO,
    SCATOLA_LOCATION_ENTRY_ACTION_CLEAR
} ScatolaLocationEntryAction;

GType      scatola_location_entry_get_type     	(void);
GtkWidget* scatola_location_entry_new          	(void);
void       scatola_location_entry_set_special_text     (ScatolaLocationEntry *entry,
        const char            *special_text);
void       scatola_location_entry_set_secondary_action (ScatolaLocationEntry *entry,
        ScatolaLocationEntryAction secondary_action);
void       scatola_location_entry_update_current_location (ScatolaLocationEntry *entry,
        const char *path);

#endif /* SCATOLA_LOCATION_ENTRY_H */
