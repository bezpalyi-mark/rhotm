#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>


#include <gtk/gtk.h>

enum { COL_FIRST_NAME = 0, COL_LAST_NAME, COL_YEAR_BORN, NUM_COLS };
GtkTreeIter child[100];

int scan_directory(char *pathName, GtkTreeStore *treestore,
                   GtkTreeIter *toplevel, int count) {
    char newPath[PATH_MAX + 1];
    DIR *dir = NULL;
    struct dirent entry;

    struct dirent *entryPtr = NULL;
    int sec_count = 0;
    /* открыть указанный каталог, если возможно. */
    dir = opendir(pathName);
    if (dir == NULL) {
        printf("Error opening %s: %s", pathName, strerror(errno));
        return 0;
    }
    if(count == 0)
    {
        gtk_tree_store_append(treestore, &child[count], NULL);
        gtk_tree_store_set(treestore, &child[count], COL_FIRST_NAME, pathName,
                           -1);
    }
    readdir_r(dir, &entry, &entryPtr);
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
                gtk_tree_store_append(treestore, &child[count+1], &child[count]);
                gtk_tree_store_set(treestore, &child[count+1], COL_FIRST_NAME, entry.d_name,
                                   -1);
                scan_directory(newPath, treestore, toplevel, ++count);
                --count;
            } else if (S_ISREG(entryInfo.st_mode)) {
                gtk_tree_store_append(treestore, &child[count+1], &child[count]);
                gtk_tree_store_set(treestore, &child[count+1], COL_FIRST_NAME, entry.d_name, -1);
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
    return count;
}

static GtkTreeModel *create_and_fill_model(char *pathName) {
    GtkTreeStore *treestore;
    GtkTreeIter toplevel, child, toplevel2, child2;
    treestore =
            gtk_tree_store_new(NUM_COLS, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_UINT);

    scan_directory(pathName, treestore, &toplevel, 0);
    return GTK_TREE_MODEL(treestore);
}

void age_cell_data_func(GtkTreeViewColumn *col, GtkCellRenderer *renderer,
                        GtkTreeModel *model, GtkTreeIter *iter,
                        gpointer user_data) {
    guint year_born;
    guint year_now = 2003; /* to save code not relevant for the example */
    gchar buf[64];

    gtk_tree_model_get(model, iter, COL_YEAR_BORN, &year_born, -1);

    if (year_born <= year_now && year_born > 0) {
        guint age = year_now - year_born;

        g_snprintf(buf, sizeof(buf), "%u years old", age);

        g_object_set(renderer, "foreground-set", FALSE,
                     NULL); /* print this normal */
    } else {
        g_snprintf(buf, sizeof(buf), "age unknown");

        /* make red */
        g_object_set(renderer, "foreground", "Red", "foreground-set", TRUE, NULL);
    }

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

    gtk_tree_view_column_set_title(col, "First Name");

    /* pack tree view column into tree view */
    gtk_tree_view_append_column(GTK_TREE_VIEW(view), col);

    renderer = gtk_cell_renderer_text_new();

    /* pack cell renderer into tree view column */
    gtk_tree_view_column_pack_start(col, renderer, TRUE);

    /* connect 'text' property of the cell renderer to
     *  model column that contains the first name */
    gtk_tree_view_column_add_attribute(col, renderer, "text", COL_FIRST_NAME);

    /* --- Column #2 --- */

    col = gtk_tree_view_column_new();

    gtk_tree_view_column_set_title(col, "Last Name");

    /* pack tree view column into tree view */
    gtk_tree_view_append_column(GTK_TREE_VIEW(view), col);

    renderer = gtk_cell_renderer_text_new();

    /* pack cell renderer into tree view column */
    gtk_tree_view_column_pack_start(col, renderer, TRUE);

    /* connect 'text' property of the cell renderer to
     *  model column that contains the last name */
    gtk_tree_view_column_add_attribute(col, renderer, "text", COL_LAST_NAME);

    /* set 'weight' property of the cell renderer to
     *  bold print (we want all last names in bold) */
    g_object_set(renderer, "weight", PANGO_WEIGHT_BOLD, "weight-set", TRUE, NULL);

    /* --- Column #3 --- */

    col = gtk_tree_view_column_new();

    gtk_tree_view_column_set_title(col, "Age");

    /* pack tree view column into tree view */
    gtk_tree_view_append_column(GTK_TREE_VIEW(view), col);

    renderer = gtk_cell_renderer_text_new();

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

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    g_signal_connect(window, "delete_event", gtk_main_quit, NULL); /* dirty */

    view = create_view_and_model("/home/hunterlan/NovaLab/GitLab");

    gtk_container_add(GTK_CONTAINER(window), view);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
