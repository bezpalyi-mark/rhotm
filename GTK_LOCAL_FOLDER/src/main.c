#include "get_path.h"

/**
 * @file main.c
 * Main programm
 * @author Team RHotM
 * @version 1.1
 * @date 2019.05.31
 */

int main(int argc, char **argv)
{
	GtkWidget *window;
	GtkWidget *view;

	struct Data data;
	data.argc = argc;
	data.argv = argv;

	gtk_init(&argc, &argv);

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	g_signal_connect(window, "delete_event", gtk_main_quit, NULL);

	view = create_view_and_model_for_data(&data);

	gtk_container_add(GTK_CONTAINER(window), view);

	gtk_widget_show_all(window);

	gtk_main();

	return 0;
}
