//
// Created by mark on 25/05/19.
//

#include "get_path.h"

void get_data(GtkButton *btn, struct Data * datas)
{


    if(datas->pathTxT != NULL || strlen(gtk_entry_get_text(datas->pathTxT) != 0))
    {
        datas->path = gtk_entry_get_text(datas->pathTxT);
    }
    else
    {
        datas->path = " ";
    }
    if(datas->maskTxT != NULL || strlen(gtk_entry_get_text(datas->maskTxT) != 0))
    {
        datas->mask = gtk_entry_get_text(datas->maskTxT);
    }
    else
    {
        datas->path = " ";
    }

    gtk_main_quit();

}

GtkWidget *create_view_and_model_for_data(struct Data * data)
{
    GtkWidget *view;
    GtkWidget *labelPath, *labelMask;
    GtkButton *submitBtn;
    GtkEntry *pathTxTBox, *maskTxTBox;

    view = gtk_table_new(3,3, false);

    labelPath = gtk_label_new("Entry path");
    gtk_table_attach_defaults(GTK_TABLE(view), labelPath, 0, 1, 0, 1);
    pathTxTBox = GTK_ENTRY(gtk_entry_new());
    gtk_table_attach_defaults(GTK_TABLE(view), (GtkWidget *) pathTxTBox, 1, 2, 0, 1);
    labelMask = gtk_label_new("Entry mask");
    gtk_table_attach_defaults(GTK_TABLE(view), labelMask, 0, 1, 1, 2);
    maskTxTBox = GTK_ENTRY(gtk_entry_new());
    gtk_table_attach_defaults(GTK_TABLE(view), (GtkWidget *) maskTxTBox, 1, 2, 1, 2);
    submitBtn = GTK_BUTTON(gtk_button_new_with_label("Submit"));

    data->pathTxT = pathTxTBox;
    data->maskTxT = maskTxTBox;

    g_signal_connect(submitBtn, "clicked", G_CALLBACK(get_data), data);
    gtk_table_attach_defaults(GTK_TABLE(view), submitBtn, 1,2, 2, 3);

    return view;
}
