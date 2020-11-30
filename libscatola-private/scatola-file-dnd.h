/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */
/* scatola-file-drag.h - Drag & drop handling code that operated on
   ScatolaFile objects.

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

   Authors: Pavel Cisler <pavel@eazel.com>,
*/

#ifndef SCATOLA_FILE_DND_H
#define SCATOLA_FILE_DND_H

#include <libscatola-private/scatola-dnd.h>
#include <libscatola-private/scatola-file.h>

#define SCATOLA_FILE_DND_ERASE_KEYWORD "erase"

gboolean scatola_drag_can_accept_item              (ScatolaFile *drop_target_item,
        const char   *item_uri);
gboolean scatola_drag_can_accept_items             (ScatolaFile *drop_target_item,
        const GList  *items);
gboolean scatola_drag_can_accept_info              (ScatolaFile *drop_target_item,
        ScatolaIconDndTargetType drag_type,
        const GList *items);
void     scatola_drag_file_receive_dropped_keyword (ScatolaFile *file,
        const char   *keyword);

#endif /* SCATOLA_FILE_DND_H */

