//
// Created by mark on 25/05/19.
//

#include "gtk_librarys.h"

struct Data
{
    char * path;
    char * mask;
    GtkEntry * pathTxT, *maskTxT;
};

GtkWidget *create_view_and_model_for_data(struct Data * data);
void get_data(GtkButton *btn, struct Data * datas);


