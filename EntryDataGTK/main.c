#include "FirstPart.h"


int main(int argc, char ** argv) {

    struct Data * data;
    GtkWidget *window;
    GtkWidget *view;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(window, "delete_event", gtk_main_quit, NULL); /* dirty */

    view = create_view_and_model(data);

    gtk_container_add(GTK_CONTAINER(window), view);

    gtk_widget_show_all(window);

    gtk_main();

    printf("Path: %s\n", data->path);
    printf("Path: %s\n", data->mask);

    return 0;
}

static GtkWidget *create_view_and_model(struct Data * data)
{
    GtkWidget *view;
    GtkWidget *labelPath, *labelMask;
    GtkButton *submitBtn;
    GtkEntry *pathTxTBox, *maskTxTBox;
    //GtkEntry ** array = (GtkEntry **)malloc(2 * sizeof(GtkEntry *));

    view = gtk_table_new(3,3, false);

    labelPath = gtk_label_new("Entry path");
    gtk_table_attach_defaults(GTK_TABLE(view), labelPath, 0, 1, 0, 1);
    pathTxTBox = GTK_ENTRY(gtk_entry_new());
    gtk_table_attach_defaults(GTK_TABLE(view), pathTxTBox, 1, 2, 0, 1);
    labelMask = gtk_label_new("Entry mask");
    gtk_table_attach_defaults(GTK_TABLE(view), labelMask, 0, 1, 1, 2);
    maskTxTBox = GTK_ENTRY(gtk_entry_new());
    gtk_table_attach_defaults(GTK_TABLE(view), maskTxTBox, 1, 2, 1, 2);
    submitBtn = GTK_BUTTON(gtk_button_new_with_label("Submit"));

    data->pathTxT = pathTxTBox;
    data->maskTxT = maskTxTBox;

    g_signal_connect(submitBtn, "clicked", G_CALLBACK(get_data), data);
    gtk_table_attach_defaults(GTK_TABLE(view), submitBtn, 1,2, 2, 3);

    return view;
}