//
// Created by mark on 25/05/19.
//

#include "get_path.h"
#include "mask.h"

enum { COL_NAME = 0, COL_SIZE, NUM_COLS };
GtkTreeIter child[1000];

int scan_directory(char *pathName, GtkTreeStore *treestore, int count, struct Data * data)
{
	char newPath[PATH_MAX + 1];
	DIR *dir = NULL;
	struct dirent entry;
	struct dirent *entryPtr = NULL;
	/* открыть указанный каталог, если возможно. */
	dir = opendir(pathName);
	if (dir == NULL) {
		printf("Error opening %s: %s", pathName, strerror(errno));
        GtkWidget *dialog;
        dialog = gtk_message_dialog_new (NULL, GTK_DIALOG_MODAL,
                                         GTK_MESSAGE_INFO, GTK_BUTTONS_OK,
                                         "No such directory!");
        gtk_window_set_title (GTK_WINDOW (dialog), "Error");
        gtk_dialog_run (GTK_DIALOG (dialog));
        gtk_widget_destroy (dialog);
		return 0;
	}
	readdir_r(dir, &entry, &entryPtr);
	struct stat entryInfo_begin;
	lstat(pathName, &entryInfo_begin);
	if (count == 0) {
		gtk_tree_store_append(treestore, &child[count], NULL);
		gtk_tree_store_set(treestore, &child[count], COL_NAME, pathName,
				   COL_SIZE, -1, -1);
	}
	while (entryPtr != NULL) {
		struct stat entryInfo;
		if (entry.d_name[0] == '.' || entry.d_name[1] == '.') {
			readdir_r(dir, &entry, &entryPtr);
			continue;
		}
		(void)strncpy(newPath, pathName, PATH_MAX);
		(void)strncat(newPath, "/", PATH_MAX);
		(void)strncat(newPath, entry.d_name, PATH_MAX);
		if (lstat(newPath, &entryInfo) == 0) {
			if (S_ISDIR(entryInfo.st_mode)) {
				gtk_tree_store_append(treestore,
						      &child[count + 1],
						      &child[count]);
				gtk_tree_store_set(treestore, &child[count + 1],
						   COL_NAME, entry.d_name,
						   COL_SIZE, -1, -1);
				scan_directory(newPath, treestore, ++count, data);
				--count;
			} else if (S_ISREG(entryInfo.st_mode)) {
			    if(mask(entry.d_name, data->mask)) {
                    gtk_tree_store_append(treestore,
                                          &child[count + 1],
                                          &child[count]);
                    gtk_tree_store_set(treestore, &child[count + 1],
                                       COL_NAME, entry.d_name,
                                       COL_SIZE,
                                       (gulong) entryInfo.st_size,
                                       -1);
                }
			}
		}
		readdir_r(dir, &entry, &entryPtr);
	}
	(void)closedir(dir);
	return --count;
}

GtkTreeModel *create_and_fill_model(char *pathName, struct Data * data)
{
	GtkTreeStore *treestore;
	GtkTreeIter toplevel;
	treestore = gtk_tree_store_new(NUM_COLS, G_TYPE_STRING, G_TYPE_ULONG);
	if(!scan_directory(pathName, treestore, 0, data)){
        return  0;
	}
	return GTK_TREE_MODEL(treestore);
}

void write_size(GtkTreeViewColumn *col, GtkCellRenderer *renderer, GtkTreeModel *model,
                GtkTreeIter *iter, gpointer user_data)
{
	gulong size;
	gchar buf[64];

	gtk_tree_model_get(model, iter, COL_SIZE, &size, -1);
	if (size >= KB && size < MB) {
		size /= KB;
		g_snprintf(buf, sizeof(buf), "%ld KB", size);
	} else if (size >= MB && size < GB) {
		size /= MB;
		g_snprintf(buf, sizeof(buf), "%ld MB", size);
	} else if (size < KB) {
		g_snprintf(buf, sizeof(buf), "%ld bites", size);
	}

	g_object_set(renderer, "foreground-set", FALSE,
		     NULL); /* print this normal */

	g_object_set(renderer, "text", buf, NULL);
}

GtkWidget *create_view_and_model(char *path, struct Data * data)
{
	GtkTreeViewColumn *col;
	GtkCellRenderer *renderer;
	GtkWidget *view;
	GtkTreeModel *model;

	view = gtk_tree_view_new();

	/* --- Column #1 --- */

	col = gtk_tree_view_column_new();

	gtk_tree_view_column_set_title(col, "Path");

	/* pack tree view column into tree view */
	gtk_tree_view_append_column(GTK_TREE_VIEW(view), col);

	renderer = gtk_cell_renderer_text_new();

	/* pack cell renderer into tree view column */
	gtk_tree_view_column_pack_start(col, renderer, TRUE);

	/* connect 'text' property of the cell renderer to
   *  model column that contains the first name */
	gtk_tree_view_column_add_attribute(col, renderer, "text", COL_NAME);

	/* --- Column #2 --- */

	col = gtk_tree_view_column_new();

	gtk_tree_view_column_set_title(col, "Size");

	/* pack tree view column into tree view */
	gtk_tree_view_append_column(GTK_TREE_VIEW(view), col);

	renderer = gtk_cell_renderer_text_new();

	/* pack cell renderer into tree view column */
	gtk_tree_view_column_pack_start(col, renderer, TRUE);

	/* pack cell renderer into tree view column */
	gtk_tree_view_column_pack_start(col, renderer, TRUE);

	/* connect a cell data function */
    gtk_tree_view_column_set_cell_data_func(col, renderer,
                                            write_size, NULL, NULL);

    if(!(model = create_and_fill_model(path, data))){
        return NULL;
    }

	gtk_tree_view_set_model(GTK_TREE_VIEW(view), model);

	g_object_unref(model); /* destroy model automatically with view */

	gtk_tree_selection_set_mode(
		gtk_tree_view_get_selection(GTK_TREE_VIEW(view)),
		GTK_SELECTION_NONE);

	return view;
}

void get_data(GtkButton *btn, struct Data *datas)
{
	GtkWidget *window;
	GtkWidget *view;
	GtkWidget *scrolledWin;

	if (datas->pathTxT != NULL ||
	    strlen(gtk_entry_get_text(datas->pathTxT) != 0)) {
		datas->path = gtk_entry_get_text(datas->pathTxT);
	} else {
		datas->path = " ";
	}
	if (datas->maskTxT != NULL ||
	    strlen(gtk_entry_get_text(datas->maskTxT) != 0)) {
		datas->mask = gtk_entry_get_text(datas->maskTxT);
	} else {
		datas->path = " ";
	}

	// g_signal_connect(window, "delete_event", gtk_main_quit, NULL); /* dirty */
    if(strcmp(datas->path, "") == 0)
    {
        GtkWidget *dialog;
        dialog = gtk_message_dialog_new (NULL, GTK_DIALOG_MODAL,
                                         GTK_MESSAGE_INFO, GTK_BUTTONS_OK,
                                         "Entry path is required!");
        gtk_window_set_title (GTK_WINDOW (dialog), "Error");
        gtk_dialog_run (GTK_DIALOG (dialog));
        gtk_widget_destroy (dialog);
    }
    else {
        gtk_init(&datas->argc, &datas->argv);

        window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        gtk_widget_set_size_request(window, 800, 600);

        if(!(view = create_view_and_model(datas->path, datas))){
            return;
        }
        scrolledWin = gtk_scrolled_window_new(NULL, NULL);
        gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledWin),
                                       GTK_POLICY_AUTOMATIC,
                                       GTK_POLICY_AUTOMATIC);

        gtk_container_add(GTK_CONTAINER(scrolledWin), view);

        gtk_container_add(GTK_CONTAINER(window), scrolledWin);

        gtk_widget_show_all(window);
    }
}

GtkWidget *create_view_and_model_for_data(struct Data *data)
{
	GtkWidget *view;
	GtkWidget *labelPath, *labelMask;
	GtkButton *submitBtn;
	GtkEntry *pathTxTBox, *maskTxTBox;

	view = gtk_table_new(3, 3, false);

	labelPath = gtk_label_new("Entry path");
	gtk_table_attach_defaults(GTK_TABLE(view), labelPath, 0, 1, 0, 1);
	pathTxTBox = GTK_ENTRY(gtk_entry_new());
	gtk_table_attach_defaults(GTK_TABLE(view), (GtkWidget *)pathTxTBox, 1,
				  2, 0, 1);
	labelMask = gtk_label_new("Entry mask");
	gtk_table_attach_defaults(GTK_TABLE(view), labelMask, 0, 1, 1, 2);
	maskTxTBox = GTK_ENTRY(gtk_entry_new());
	gtk_table_attach_defaults(GTK_TABLE(view), (GtkWidget *)maskTxTBox, 1,
				  2, 1, 2);
	submitBtn = GTK_BUTTON(gtk_button_new_with_label("Submit"));

	data->pathTxT = pathTxTBox;
	data->maskTxT = maskTxTBox;

	g_signal_connect(submitBtn, "clicked", G_CALLBACK(get_data), data);
	gtk_table_attach_defaults(GTK_TABLE(view), submitBtn, 1, 2, 2, 3);

	return view;
}
