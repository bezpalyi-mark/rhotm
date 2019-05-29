//
// Created by mark on 25/05/19.
//

#include "gtk_librarys.h"
#define KB 1000
#define MB 1000000
#define GB 1000000000
struct Data {
	char *path;
	char *mask;
	GtkEntry *pathTxT, *maskTxT;
	int argc;
	char **argv;
};

int scan_directory(char *pathName, GtkTreeStore *treestore, int count);
GtkTreeModel *create_and_fill_model(char *pathName);
void age_cell_data_func(GtkCellRenderer *renderer, GtkTreeModel *model,
			GtkTreeIter *iter);
GtkWidget *create_view_and_model_for_data(struct Data *data);
void get_data(GtkButton *btn, struct Data *datas);
GtkWidget *create_view_and_model(char *path);
