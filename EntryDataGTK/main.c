
#include "get_path.h"
//#include "scroll_window.h"

enum { COL_NAME = 0, COL_SIZE, NUM_COLS };
GtkTreeIter child[1000];

int scan_directory(char *pathName, GtkTreeStore *treestore, int count) {
    char newPath[PATH_MAX + 1];
    DIR *dir = NULL;
    struct dirent entry;
    struct dirent *entryPtr = NULL;
    /* открыть указанный каталог, если возможно. */
    dir = opendir(pathName);
    if (dir == NULL) {
        printf("Error opening %s: %s", pathName, strerror(errno));
        return 0;
    }
    readdir_r(dir, &entry, &entryPtr);
    struct stat entryInfo_begin;
    lstat(pathName, &entryInfo_begin);
    if (count == 0) {
        gtk_tree_store_append(treestore, &child[count], NULL);
        gtk_tree_store_set(treestore, &child[count], COL_NAME, pathName, COL_SIZE,
                           -1, -1);
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
                gtk_tree_store_append(treestore, &child[count + 1], &child[count]);
                gtk_tree_store_set(treestore, &child[count + 1], COL_NAME, entry.d_name,
                                   COL_SIZE, -1, -1);
                scan_directory(newPath, treestore, ++count);
                --count;
            } else if (S_ISREG(entryInfo.st_mode)) {
                gtk_tree_store_append(treestore, &child[count + 1], &child[count]);
                gtk_tree_store_set(treestore, &child[count + 1], COL_NAME, entry.d_name,
                                   COL_SIZE, (guint)entryInfo.st_size, -1);
            } else if (S_ISLNK(entryInfo.st_mode)) {
                char targetName[PATH_MAX + 1];
                if (readlink(pathName, targetName, PATH_MAX) != -1) {
                    printf("\t%s -> %s\n", newPath, targetName);
                } else {
                    printf("\t%s -> (invalid symbolic link!)\n", newPath);
                }
            } else {
                printf("Error statting %s: %s\n", pathName, strerror(errno));
            }
        }
        readdir_r(dir, &entry, &entryPtr);
    }
    (void)closedir(dir);
    return --count;
}

static GtkTreeModel *create_and_fill_model(char *pathName) {
    GtkTreeStore *treestore;
    GtkTreeIter toplevel;
    treestore = gtk_tree_store_new(NUM_COLS, G_TYPE_STRING, G_TYPE_ULONG);
    scan_directory(pathName, treestore, 0);
    return GTK_TREE_MODEL(treestore);
}

void age_cell_data_func(GtkTreeViewColumn *col, GtkCellRenderer *renderer,
                        GtkTreeModel *model, GtkTreeIter *iter,
                        gpointer user_data) {
    gulong size;
    gchar buf[64];

    gtk_tree_model_get(model, iter, COL_SIZE, &size, -1);

    if(size != 0)
        g_snprintf(buf, sizeof(buf), "%ld bites", size);

    g_object_set(renderer, "foreground-set", FALSE, NULL); /* print this normal */

    g_object_set(renderer, "text", buf, NULL);
}

static GtkWidget *create_view_and_model(char *path) {
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
    gtk_tree_view_column_set_cell_data_func(col, renderer, age_cell_data_func,
                                            NULL, NULL);

    model = create_and_fill_model(path);

    gtk_tree_view_set_model(GTK_TREE_VIEW(view), model);

    g_object_unref(model); /* destroy model automatically with view */

    gtk_tree_selection_set_mode(gtk_tree_view_get_selection(GTK_TREE_VIEW(view)),
                                GTK_SELECTION_NONE);


    return view;
}

int main(int argc, char **argv) {
    GtkWidget *window;
    GtkWidget *view;
    GtkWidget *scrolledWin;

    struct Data * data;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(window, "delete_event", gtk_main_quit, NULL); /* dirty */

    scrolledWin = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledWin),
                                    GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);

    view = create_view_and_model_for_data(data);

    gtk_container_add(GTK_CONTAINER(window), view);

    gtk_widget_show_all(window);

    gtk_main();

    gtk_init(&argc, &argv);

    g_signal_connect(window, "delete_event", gtk_main_quit, NULL); /* dirty */

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    view = create_view_and_model(data->path);

    gtk_container_add(GTK_CONTAINER(scrolledWin), view);

    gtk_container_add(GTK_CONTAINER(window), scrolledWin);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
