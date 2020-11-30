/* -*- Mode: C; tab-width: 8; indent-tabs-mode: 8; c-basic-offset: 8 -*- */

/*
 * Scatola
 *
 * Copyright (C) 1999, 2000 Red Hat, Inc.
 * Copyright (C) 1999, 2000 Eazel, Inc.
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
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
 */

/* scatola-main.c
 */

#ifndef SCATOLA_MAIN_H
#define SCATOLA_MAIN_H

#include <gtk/gtk.h>

#if GTK_CHECK_VERSION(3, 0, 0)
void     scatola_main_event_loop_register    (GtkWidget *object);
#else
void     scatola_main_event_loop_register    (GtkObject *object);
#endif
gboolean scatola_main_is_event_loop_mainstay (GtkWidget *object);
void     scatola_main_event_loop_quit        (gboolean explicit);

#endif /* SCATOLA_MAIN_H */

