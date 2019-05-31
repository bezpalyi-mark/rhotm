#include "get_path.h"
#include "tests.h"


//#define TEST
#ifndef TEST
int main(int argc, char **argv)
{
	GtkWidget *window;
	GtkWidget *view;

	struct Data data;
	data.argc = argc;
	data.argv = argv;

	gtk_init(&argc, &argv);

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	g_signal_connect(window, "delete_event", gtk_main_quit,
			 NULL);


	view = create_view_and_model_for_data(&data);

	gtk_container_add(GTK_CONTAINER(window), view);

	gtk_widget_show_all(window);

	gtk_main();

	return 0;
}

#else
int main(){
    FILE * file = freopen("tests_result.txt", "w", stderr);
    if(test_mask()){
        fprintf(stderr, "\nALL TESTS PASSED\n");
    }
    fclose(file);
}
#endif