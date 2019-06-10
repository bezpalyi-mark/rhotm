#include "gtk_librarys.h"

#define KB 1000
#define MB 1000000
#define GB 1000000000

/**
 * @file gtk_path.h
 * File opsing creating window and getting data
 * @author Team RHotM
 * @version 1.1
 * @date 2019.05.31
 */

/**
 * Function, which getting folders and files
 * @param char * pathName - start path
 * @param GtkTreeStore *treestore - start tree
 * @param int count - count for childs
 * @param struct Data *data - need for getting mask
 */
int scan_directory(char *pathName, GtkTreeStore *treestore, int count,
		   struct Data *data);

/*!
Function, for creating tree. Start function @link {scan_directory}
@param char *pathName - start path
@param struct Data *data - datas, contain mask and other need things.
@return Fillied tree
*/
GtkTreeModel *create_and_fill_model(char *pathName, struct Data *data);

void write_size(GtkTreeViewColumn *col, GtkCellRenderer *renderer,
		GtkTreeModel *model, GtkTreeIter *iter, gpointer user_data);

/*!
Function, which create window, for getting data
@param struct Data *data - for init textBox for struct
@return Window, with textbox and buttons
*/
GtkWidget *create_view_and_model_for_data(struct Data *data);

/*!
Function, which get data from text box
@param GtkButton *btn - signal for button
@param struct Data *datas - for init data from textBox to char *
*/

void get_data(GtkButton *btn, struct Data *datas);

GtkWidget *create_view_and_model(char *path, struct Data *data);
