/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*-

   scatola-desktop-link.h: Class that handles the links on the desktop

   Copyright (C) 2003 Red Hat, Inc.

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

#ifndef SCATOLA_DESKTOP_LINK_H
#define SCATOLA_DESKTOP_LINK_H

#include <libscatola-private/scatola-file.h>
#include <gio/gio.h>

#define SCATOLA_TYPE_DESKTOP_LINK scatola_desktop_link_get_type()
#define SCATOLA_DESKTOP_LINK(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), SCATOLA_TYPE_DESKTOP_LINK, ScatolaDesktopLink))
#define SCATOLA_DESKTOP_LINK_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), SCATOLA_TYPE_DESKTOP_LINK, ScatolaDesktopLinkClass))
#define SCATOLA_IS_DESKTOP_LINK(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SCATOLA_TYPE_DESKTOP_LINK))
#define SCATOLA_IS_DESKTOP_LINK_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), SCATOLA_TYPE_DESKTOP_LINK))
#define SCATOLA_DESKTOP_LINK_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), SCATOLA_TYPE_DESKTOP_LINK, ScatolaDesktopLinkClass))

typedef struct ScatolaDesktopLinkDetails ScatolaDesktopLinkDetails;

typedef struct
{
    GObject parent_slot;
    ScatolaDesktopLinkDetails *details;
} ScatolaDesktopLink;

typedef struct
{
    GObjectClass parent_slot;
} ScatolaDesktopLinkClass;

typedef enum
{
    SCATOLA_DESKTOP_LINK_HOME,
    SCATOLA_DESKTOP_LINK_COMPUTER,
    SCATOLA_DESKTOP_LINK_TRASH,
    SCATOLA_DESKTOP_LINK_MOUNT,
    SCATOLA_DESKTOP_LINK_NETWORK
} ScatolaDesktopLinkType;

GType   scatola_desktop_link_get_type (void);

ScatolaDesktopLink *   scatola_desktop_link_new                     (ScatolaDesktopLinkType  type);
ScatolaDesktopLink *   scatola_desktop_link_new_from_mount          (GMount                 *mount);
ScatolaDesktopLinkType scatola_desktop_link_get_link_type           (ScatolaDesktopLink     *link);
char *                  scatola_desktop_link_get_file_name           (ScatolaDesktopLink     *link);
char *                  scatola_desktop_link_get_display_name        (ScatolaDesktopLink     *link);
GIcon *                 scatola_desktop_link_get_icon                (ScatolaDesktopLink     *link);
GFile *                 scatola_desktop_link_get_activation_location (ScatolaDesktopLink     *link);
char *                  scatola_desktop_link_get_activation_uri      (ScatolaDesktopLink     *link);
gboolean                scatola_desktop_link_get_date                (ScatolaDesktopLink     *link,
        ScatolaDateType         date_type,
        time_t                  *date);
GMount *                scatola_desktop_link_get_mount               (ScatolaDesktopLink     *link);
gboolean                scatola_desktop_link_can_rename              (ScatolaDesktopLink     *link);
gboolean                scatola_desktop_link_rename                  (ScatolaDesktopLink     *link,
        const char              *name);


#endif /* SCATOLA_DESKTOP_LINK_H */
