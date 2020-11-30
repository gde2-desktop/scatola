

#ifndef __SCATOLA_RECENT_H__
#define __SCATOLA_RECENT_H__

#include <gtk/gtk.h>
#include <libscatola-private/scatola-file.h>
#include <gio/gio.h>

void scatola_recent_add_file (ScatolaFile *file,
                           GAppInfo *application);

#endif
