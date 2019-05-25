//
// Created by hunterlan on 5/22/19.
//


#include "FirstPart.h"



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

