#include "Functions.h"
#include "MainWindow.h"


int main(int argc,
         char *argv[])
{
    gtk_init(&argc, &argv);

    MainWindow mainWindow;

    mainWindow.window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title (GTK_WINDOW (mainWindow.window), "GTK_IMAGEMAGICK");

    mainWindow.vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    mainWindow.label = gtk_label_new("Put the path to pictures: ");
    mainWindow.entryPath = gtk_entry_new();
    mainWindow.labelSave = gtk_label_new("Put the path, where pictures will be saved: ");
    mainWindow.entrySave = gtk_entry_new();
    mainWindow.button = gtk_button_new_with_label("Resize");

    g_signal_connect(G_OBJECT(mainWindow.button), "clicked", G_CALLBACK(PoolThreading), &mainWindow);

    gtk_box_pack_start(GTK_BOX (mainWindow.vbox), mainWindow.label, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX (mainWindow.vbox), mainWindow.entryPath, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX (mainWindow.vbox), mainWindow.labelSave, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX (mainWindow.vbox), mainWindow.entrySave, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX (mainWindow.vbox), mainWindow.button, FALSE, FALSE, 5);
    gtk_container_add(GTK_CONTAINER(mainWindow.window), mainWindow.vbox);

    gtk_widget_show_all(mainWindow.window);

    gtk_main();


	return 0;
}
