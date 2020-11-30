/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/* ScatolaUndoStackManager - Manages undo of file operations (header)
 *
 * Copyright (C) 2007-2010 Amos Brocco
 * Copyright (C) 2011 Stefano Karapetsas
 *
 * Authors: Amos Brocco <amos.brocco@unifr.ch>,
 *          Stefano Karapetsas <stefano@karapetsas.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#ifndef SCATOLA_UNDOSTACK_MANAGER_H
#define SCATOLA_UNDOSTACK_MANAGER_H

#include <glib.h>
#include <glib-object.h>
#include <gtk/gtk.h>
#include <gio/gio.h>

/* Begin action structures */

typedef enum
{
  SCATOLA_UNDOSTACK_COPY,
  SCATOLA_UNDOSTACK_DUPLICATE,
  SCATOLA_UNDOSTACK_MOVE,
  SCATOLA_UNDOSTACK_RENAME,
  SCATOLA_UNDOSTACK_CREATEEMPTYFILE,
  SCATOLA_UNDOSTACK_CREATEFILEFROMTEMPLATE,
  SCATOLA_UNDOSTACK_CREATEFOLDER,
  SCATOLA_UNDOSTACK_MOVETOTRASH,
  SCATOLA_UNDOSTACK_CREATELINK,
  SCATOLA_UNDOSTACK_DELETE,
  SCATOLA_UNDOSTACK_RESTOREFROMTRASH,
  SCATOLA_UNDOSTACK_SETPERMISSIONS,
  SCATOLA_UNDOSTACK_RECURSIVESETPERMISSIONS,
  SCATOLA_UNDOSTACK_CHANGEOWNER,
  SCATOLA_UNDOSTACK_CHANGEGROUP
} ScatolaUndoStackActionType;

typedef struct _ScatolaUndoStackActionData ScatolaUndoStackActionData;

typedef struct _ScatolaUndoStackMenuData ScatolaUndoStackMenuData;

struct _ScatolaUndoStackMenuData {
  char* undo_label;
  char* undo_description;
  char* redo_label;
  char* redo_description;
};

/* End action structures */

typedef void
(*ScatolaUndostackFinishCallback)(gpointer data);

typedef struct _ScatolaUndoStackManagerPrivate ScatolaUndoStackManagerPrivate;

typedef struct _ScatolaUndoStackManager
{
  GObject parent_instance;

  ScatolaUndoStackManagerPrivate* priv;

} ScatolaUndoStackManager;

typedef struct _ScatolaUndoStackManagerClass
{
  GObjectClass parent_class;
  
} ScatolaUndoStackManagerClass;

#define TYPE_SCATOLA_UNDOSTACK_MANAGER (scatola_undostack_manager_get_type())

#define SCATOLA_UNDOSTACK_MANAGER(object) \
 (G_TYPE_CHECK_INSTANCE_CAST((object), TYPE_SCATOLA_UNDOSTACK_MANAGER, ScatolaUndoStackManager))

#define SCATOLA_UNDOSTACK_MANAGER_CLASS(klass) \
 (G_TYPE_CHECK_CLASS_CAST((klass), TYPE_SCATOLA_UNDOSTACK_MANAGER, ScatolaUndoStackManagerClass))

#define IS_SCATOLA_UNDOSTACK_MANAGER(object) \
 (G_TYPE_CHECK_INSTANCE_TYPE((object), TYPE_SCATOLA_UNDOSTACK_MANAGER))

#define IS_SCATOLA_UNDOSTACK_MANAGER_CLASS(klass) \
 (G_TYPE_CHECK_CLASS_TYPE((klass), TYPE_SCATOLA_UNDOSTACK_MANAGER))

#define SCATOLA_UNDOSTACK_MANAGER_GET_CLASS(object) \
 (G_TYPE_INSTANCE_GET_CLASS((object), TYPE_SCATOLA_UNDOSTACK_MANAGER, ScatolaUndoStackManagerClass))
 
GType
scatola_undostack_manager_get_type (void);

void
scatola_undostack_manager_add_action(ScatolaUndoStackManager* manager,
    ScatolaUndoStackActionData* action);

void
scatola_undostack_manager_undo(ScatolaUndoStackManager* manager,
    GtkWidget *parent_view, ScatolaUndostackFinishCallback cb);

void
scatola_undostack_manager_redo(ScatolaUndoStackManager* manager,
    GtkWidget *parent_view, ScatolaUndostackFinishCallback cb);

ScatolaUndoStackActionData*
scatola_undostack_manager_data_new(ScatolaUndoStackActionType type,
    gint items_count);

gboolean
scatola_undostack_manager_is_undo_redo(ScatolaUndoStackManager* manager);

void
scatola_undostack_manager_trash_has_emptied(ScatolaUndoStackManager* manager);

ScatolaUndoStackManager*
scatola_undostack_manager_instance(void);

void
scatola_undostack_manager_data_set_src_dir(ScatolaUndoStackActionData* data,
    GFile* src);

void
scatola_undostack_manager_data_set_dest_dir(ScatolaUndoStackActionData* data,
    GFile* dest);

void
scatola_undostack_manager_data_add_origin_target_pair(
    ScatolaUndoStackActionData* data, GFile* origin, GFile* target);

void
scatola_undostack_manager_data_set_create_data(
    ScatolaUndoStackActionData* data, char* target_uri, char* template_uri);

void
scatola_undostack_manager_data_set_rename_information(
    ScatolaUndoStackActionData* data, GFile* old_file, GFile* new_file);

guint64
scatola_undostack_manager_get_file_modification_time(GFile* file);

void
scatola_undostack_manager_data_add_trashed_file(
    ScatolaUndoStackActionData* data, GFile* file, guint64 mtime);

void
scatola_undostack_manager_request_menu_update(ScatolaUndoStackManager* manager);

void
scatola_undostack_manager_data_add_file_permissions(
    ScatolaUndoStackActionData* data, GFile* file, guint32 permission);
    
void
scatola_undostack_manager_data_set_recursive_permissions(
    ScatolaUndoStackActionData* data, guint32 file_permissions, guint32 file_mask,
	guint32 dir_permissions, guint32 dir_mask);
    
void
scatola_undostack_manager_data_set_file_permissions(
    ScatolaUndoStackActionData* data, char* uri, guint32 current_permissions, guint32 new_permissions);
    
void
scatola_undostack_manager_data_set_owner_change_information(
    ScatolaUndoStackActionData* data, char* uri, const char* current_user, const char* new_user);
    
void
scatola_undostack_manager_data_set_group_change_information(
    ScatolaUndoStackActionData* data, char* uri, const char* current_group, const char* new_group);    

#endif /* SCATOLA_UNDOSTACK_MANAGER_H */
