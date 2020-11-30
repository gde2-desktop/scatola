#include <gtk/gtk.h>
#include <libscatola-private/scatola-directory.h>
#include <libscatola-private/scatola-search-directory.h>
#include <libscatola-private/scatola-file.h>
#include <unistd.h>

void *client1, *client2;

#if 0
static gboolean
quit_cb (gpointer data)
{
	gtk_main_quit ();

	return FALSE;
}
#endif

static void
files_added (ScatolaDirectory *directory,
	     GList *added_files)
{
#if 0
	GList *list;

	for (list = added_files; list != NULL; list = list->next) {
		ScatolaFile *file = list->data;

		g_print (" - %s\n", scatola_file_get_uri (file));
	}
#endif

	g_print ("files added: %d files\n",
		 g_list_length (added_files));
}

static void
files_changed (ScatolaDirectory *directory,
	       GList *changed_files)
{
#if 0
	GList *list;

	for (list = changed_files; list != NULL; list = list->next) {
		ScatolaFile *file = list->data;

		g_print (" - %s\n", scatola_file_get_uri (file));
	}
#endif
	g_print ("files changed: %d\n",
		 g_list_length (changed_files));
}

static gboolean
force_reload (ScatolaDirectory *directory)
{
	g_print ("forcing reload!\n");

	scatola_directory_force_reload (directory);

	return FALSE;
}

static void
done_loading (ScatolaDirectory *directory)
{
	static int i = 0;

	g_print ("done loading\n");

	if (i == 0) {
		g_timeout_add (5000, (GSourceFunc)force_reload, directory);
		i++;
	} else {
	}
}

int
main (int argc, char **argv)
{
	ScatolaDirectory *directory;
	ScatolaQuery *query;
	client1 = g_new0 (int, 1);
	client2 = g_new0 (int, 1);

	gtk_init (&argc, &argv);

	query = scatola_query_new ();
	scatola_query_set_text (query, "richard hult");
	directory = scatola_directory_get_by_uri ("x-scatola-search://0/");
	scatola_search_directory_set_query (SCATOLA_SEARCH_DIRECTORY (directory), query);
	g_object_unref (query);

	g_signal_connect (directory, "files-added", G_CALLBACK (files_added), NULL);
	g_signal_connect (directory, "files-changed", G_CALLBACK (files_changed), NULL);
	g_signal_connect (directory, "done-loading", G_CALLBACK (done_loading), NULL);
	scatola_directory_file_monitor_add (directory, client1, TRUE,
					     SCATOLA_FILE_ATTRIBUTE_INFO, 
					     NULL, NULL);


	gtk_main ();
	return 0;
}
