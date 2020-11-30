/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*-

   scatola-search-directory-file.c: Subclass of ScatolaFile to help implement the
   searches

   Copyright (C) 2005 Novell, Inc.

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

   Author: Anders Carlsson <andersca@imendio.com>
*/

#include <config.h>
#include "scatola-search-directory-file.h"

#include "scatola-directory-notify.h"
#include "scatola-directory-private.h"
#include "scatola-file-attributes.h"
#include "scatola-file-private.h"
#include "scatola-file-utilities.h"
#include <eel/eel-glib-extensions.h>
#include "scatola-search-directory.h"
#include <gtk/gtk.h>
#include <glib/gi18n.h>
#include <string.h>

struct ScatolaSearchDirectoryFileDetails
{
    ScatolaSearchDirectory *search_directory;
};

G_DEFINE_TYPE(ScatolaSearchDirectoryFile, scatola_search_directory_file, SCATOLA_TYPE_FILE);


static void
search_directory_file_monitor_add (ScatolaFile *file,
                                   gconstpointer client,
                                   ScatolaFileAttributes attributes)
{
    /* No need for monitoring, we always emit changed when files
       are added/removed, and no other metadata changes */

    /* Update display name, in case this didn't happen yet */
    scatola_search_directory_file_update_display_name (SCATOLA_SEARCH_DIRECTORY_FILE (file));
}

static void
search_directory_file_monitor_remove (ScatolaFile *file,
                                      gconstpointer client)
{
    /* Do nothing here, we don't have any monitors */
}

static void
search_directory_file_call_when_ready (ScatolaFile *file,
                                       ScatolaFileAttributes file_attributes,
                                       ScatolaFileCallback callback,
                                       gpointer callback_data)

{
    /* Update display name, in case this didn't happen yet */
    scatola_search_directory_file_update_display_name (SCATOLA_SEARCH_DIRECTORY_FILE (file));

    /* All data for directory-as-file is always uptodate */
    (* callback) (file, callback_data);
}

static void
search_directory_file_cancel_call_when_ready (ScatolaFile *file,
        ScatolaFileCallback callback,
        gpointer callback_data)
{
    /* Do nothing here, we don't have any pending calls */
}

static gboolean
search_directory_file_check_if_ready (ScatolaFile *file,
                                      ScatolaFileAttributes attributes)
{
    return TRUE;
}

static gboolean
search_directory_file_get_item_count (ScatolaFile *file,
                                      guint *count,
                                      gboolean *count_unreadable)
{
    GList *file_list;

    if (count)
    {
        file_list = scatola_directory_get_file_list (file->details->directory);

        *count = g_list_length (file_list);

        scatola_file_list_free (file_list);
    }

    return TRUE;
}

static ScatolaRequestStatus
search_directory_file_get_deep_counts (ScatolaFile *file,
                                       guint *directory_count,
                                       guint *file_count,
                                       guint *unreadable_directory_count,
                                       goffset *total_size)
{
    ScatolaFile *dir_file;
    GList *file_list, *l;
    guint dirs, files;
    GFileType type;

    file_list = scatola_directory_get_file_list (file->details->directory);

    dirs = files = 0;
    for (l = file_list; l != NULL; l = l->next)
    {
        dir_file = SCATOLA_FILE (l->data);
        type = scatola_file_get_file_type (dir_file);
        if (type == G_FILE_TYPE_DIRECTORY)
        {
            dirs++;
        }
        else
        {
            files++;
        }
    }

    if (directory_count != NULL)
    {
        *directory_count = dirs;
    }
    if (file_count != NULL)
    {
        *file_count = files;
    }
    if (unreadable_directory_count != NULL)
    {
        *unreadable_directory_count = 0;
    }
    if (total_size != NULL)
    {
        /* FIXME: Maybe we want to calculate this? */
        *total_size = 0;
    }

    scatola_file_list_free (file_list);

    return SCATOLA_REQUEST_DONE;
}

static char *
search_directory_file_get_where_string (ScatolaFile *file)
{
    return g_strdup (_("Search"));
}

void
scatola_search_directory_file_update_display_name (ScatolaSearchDirectoryFile *search_file)
{
    ScatolaFile *file;
    ScatolaSearchDirectory *search_dir;
    ScatolaQuery *query;
    char *display_name;
    gboolean changed;


    display_name = NULL;
    file = SCATOLA_FILE (search_file);
    if (file->details->directory)
    {
        search_dir = SCATOLA_SEARCH_DIRECTORY (file->details->directory);
        query = scatola_search_directory_get_query (search_dir);

        if (query != NULL)
        {
            display_name = scatola_query_to_readable_string (query);
            g_object_unref (query);
        }
    }

    if (display_name == NULL)
    {
        display_name = g_strdup (_("Search"));
    }

    changed = scatola_file_set_display_name (file, display_name, NULL, TRUE);
    if (changed)
    {
        scatola_file_emit_changed (file);
    }
}

static void
scatola_search_directory_file_init (ScatolaSearchDirectoryFile *search_file)
{
    ScatolaFile *file;

    file = SCATOLA_FILE (search_file);

    file->details->got_file_info = TRUE;
    file->details->mime_type = eel_ref_str_get_unique ("x-directory/normal");
    file->details->type = G_FILE_TYPE_DIRECTORY;
    file->details->size = 0;

    file->details->file_info_is_up_to_date = TRUE;

    file->details->custom_icon = NULL;
    file->details->activation_uri = NULL;
    file->details->got_link_info = TRUE;
    file->details->link_info_is_up_to_date = TRUE;

    file->details->directory_count = 0;
    file->details->got_directory_count = TRUE;
    file->details->directory_count_is_up_to_date = TRUE;

    scatola_file_set_display_name (file, _("Search"), NULL, TRUE);
}

static void
scatola_search_directory_file_class_init (ScatolaSearchDirectoryFileClass *klass)
{
    ScatolaFileClass *file_class;

    file_class = SCATOLA_FILE_CLASS (klass);

    file_class->default_file_type = G_FILE_TYPE_DIRECTORY;

    file_class->monitor_add = search_directory_file_monitor_add;
    file_class->monitor_remove = search_directory_file_monitor_remove;
    file_class->call_when_ready = search_directory_file_call_when_ready;
    file_class->cancel_call_when_ready = search_directory_file_cancel_call_when_ready;
    file_class->check_if_ready = search_directory_file_check_if_ready;
    file_class->get_item_count = search_directory_file_get_item_count;
    file_class->get_deep_counts = search_directory_file_get_deep_counts;
    file_class->get_where_string = search_directory_file_get_where_string;
}
