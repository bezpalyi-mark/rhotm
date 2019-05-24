//
// Created by hunterlan on 5/22/19.
//

#ifndef FIRSTPART_H
#define FIRSTPART_H

#include <gtk/gtk.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

struct Data
{
    char * path;
    char * mask;
    GtkEntry * pathTxT, *maskTxT;
};

static GtkWidget *create_view_and_model();
void get_data(GtkButton *btn, struct Data * datas);

#endif //FIRSTPART_H
