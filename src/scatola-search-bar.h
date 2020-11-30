/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */
/*
 * Copyright (C) 2005 Novell, Inc.
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
 * Author: Anders Carlsson <andersca@imendio.com>
 *
 */

#ifndef SCATOLA_SEARCH_BAR_H
#define SCATOLA_SEARCH_BAR_H

#include <gtk/gtk.h>
#include <libscatola-private/scatola-query.h>

#define SCATOLA_TYPE_SEARCH_BAR scatola_search_bar_get_type()
#define SCATOLA_SEARCH_BAR(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), SCATOLA_TYPE_SEARCH_BAR, ScatolaSearchBar))
#define SCATOLA_SEARCH_BAR_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), SCATOLA_TYPE_SEARCH_BAR, ScatolaSearchBarClass))
#define SCATOLA_IS_SEARCH_BAR(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SCATOLA_TYPE_SEARCH_BAR))
#define SCATOLA_IS_SEARCH_BAR_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), SCATOLA_TYPE_SEARCH_BAR))
#define SCATOLA_SEARCH_BAR_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), SCATOLA_TYPE_SEARCH_BAR, ScatolaSearchBarClass))

typedef struct ScatolaSearchBarDetails ScatolaSearchBarDetails;

typedef struct ScatolaSearchBar
{
    GtkEventBox parent;
    ScatolaSearchBarDetails *details;
} ScatolaSearchBar;

typedef struct
{
    GtkEventBoxClass parent_class;

    void (* activate) (ScatolaSearchBar *bar);
    void (* cancel)   (ScatolaSearchBar *bar);
    void (* focus_in) (ScatolaSearchBar *bar);
} ScatolaSearchBarClass;

GType      scatola_search_bar_get_type     	(void);
GtkWidget* scatola_search_bar_new          	(void);

GtkWidget *    scatola_search_bar_borrow_entry  (ScatolaSearchBar *bar);
void           scatola_search_bar_return_entry  (ScatolaSearchBar *bar);
void           scatola_search_bar_grab_focus    (ScatolaSearchBar *bar);
ScatolaQuery *scatola_search_bar_get_query     (ScatolaSearchBar *bar);
void           scatola_search_bar_clear         (ScatolaSearchBar *bar);

#endif /* SCATOLA_SEARCH_BAR_H */
