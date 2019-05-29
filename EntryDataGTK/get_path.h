//
// Created by mark on 25/05/19.
//

#include "gtk_librarys.h"
#define KB 1000
#define MB 1000000
#define GB 1000000000
struct Data
{
    char * path;
    char * mask;
    GtkEntry * pathTxT, *maskTxT;
};

GtkWidget *create_view_and_model_for_data(struct Data * data);
void get_data(GtkButton *btn, struct Data * datas);


