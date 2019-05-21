#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
//#include <gtk/gtk.h>
//
// enum { COL_NAME = 0, NUM_COLS };
//
// int scan_directory(char *pathName, GtkListStore *store, GtkTreeIter * iter) {
//    char newPath[PATH_MAX + 1];
//    DIR *dir = NULL;
//    struct dirent entry;
//    struct dirent *entryPtr = NULL;
//    int retval = 0;
//    unsigned count = 0;
//    int sec_count = 0;
//    /* открыть указанный каталог, если возможно. */
//    dir = opendir(pathName);
//    if (dir == NULL) {
//        printf("Error opening %s: %s", pathName, strerror(errno));
//        return 0;
//    }
//    retval = readdir_r(dir, &entry, &entryPtr);
//    while (entryPtr != NULL) {
//        struct stat entryInfo;
//        if (entry.d_name[0] == '.' || entry.d_name[1] == '.') {
//            retval = readdir_r(dir, &entry, &entryPtr);
//            continue;
//        }
//        (void)strncpy(newPath, pathName, PATH_MAX);
//        (void)strncat(newPath, "/", PATH_MAX);
//        (void)strncat(newPath, entry.d_name, PATH_MAX);
//        if (lstat(newPath, &entryInfo) == 0) {
//            count++;
//            if (S_ISREG(entryInfo.st_mode)) {
//                //                printf("\t%s has %lld bytes\n", newPath,
//                (long
//                //                long)entryInfo.st_size );
//                gtk_list_store_append(store, iter);
//                gtk_list_store_set(store, iter, COL_NAME, entry.d_name, -1);
////                printf("  %s has %lld bytes\n", entry.d_name,
////                       (long long)entryInfo.st_size);
//            } else if (S_ISDIR(entryInfo.st_mode)) {
//                //                printf("new dir: %s/\n", newPath);
//                gtk_list_store_append(store, iter);
//                gtk_list_store_set(store, iter, COL_NAME, entry.d_name, -1);
////                printf("+ %s\n", entry.d_name);
//                count += scan_directory(newPath, store, iter);
//            } else if (S_ISLNK(entryInfo.st_mode)) {
//                char targetName[PATH_MAX + 1];
//                if (readlink(pathName, targetName, PATH_MAX) != -1) {
//                    printf("\t%s -> %s\n", newPath, targetName);
//                } else {
//                    printf("\t%s -> (invalid symbolic link!)\n", newPath);
//                }
//            } else {
//                printf("Error statting %s: %s\n", pathName, strerror(errno));
//            }
//        }
//        retval = readdir_r(dir, &entry, &entryPtr);
//    }
//    (void)closedir(dir);
//    return count;
//}
//
//
// static GtkTreeModel *create_and_fill_model(char * path) {
//  GtkListStore *store;
//  GtkTreeIter iter;
//
//  store = gtk_list_store_new(NUM_COLS, G_TYPE_STRING);
//
////  /* Append a row and fill in some data */
////  gtk_list_store_append(store, &iter);
////  gtk_list_store_set(store, &iter, COL_NAME, "Heinz El-Mann", -1);
////
////  /* append another row and fill in some data */
////  gtk_list_store_append(store, &iter);
////  gtk_list_store_set(store, &iter, COL_NAME, "Jane Doe", -1);
////
////  /* ... and a third row */
////  gtk_list_store_append(store, &iter);
////  gtk_list_store_set(store, &iter, COL_NAME, "Joe Bungop", -1);
//
//  scan_directory(path, store, &iter);
//
//  return GTK_TREE_MODEL(store);
//}
//
// static GtkWidget *create_view_and_model(char * path) {
//  GtkCellRenderer *renderer;
//  GtkTreeModel *model;
//  GtkWidget *view;
//
//  view = gtk_tree_view_new();
//
//  /* --- Column #1 --- */
//
//  renderer = gtk_cell_renderer_text_new();
//  gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(view), -1, "Name",
//                                              renderer, "text", COL_NAME,
//                                              NULL);
//
//  /* --- Column #2 --- */
//
////  renderer = gtk_cell_renderer_text_new();
////  gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(view), -1,
///"Age", /                                              renderer, "text",
///COL_AGE, NULL);
//
//  model = create_and_fill_model(path);
//
//  gtk_tree_view_set_model(GTK_TREE_VIEW(view), model);
//
//  /* The tree view has acquired its own reference to the
//   *  model, so we can drop ours. That way the model will
//   *  be freed automatically when the tree view is destroyed */
//
//  g_object_unref(model);
//
//  return view;
//}
//
// int main(int argc, char **argv) {
//  GtkWidget *window;
//  GtkWidget *view;
//
//  gtk_init(&argc, &argv);
//
//  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
//  g_signal_connect(window, "delete_event", gtk_main_quit, NULL); /* dirty */
//
//  view = create_view_and_model("/home/mark/for_scan");
//
//  gtk_container_add(GTK_CONTAINER(window), view);
//
//  gtk_widget_show_all(window);
//
//  gtk_main();
//
//  return 0;
//}

#include <gtk/gtk.h>

enum { COL_FIRST_NAME = 0, COL_LAST_NAME, COL_YEAR_BORN, NUM_COLS };

int scan_directory(char *pathName, GtkTreeStore *treestore,
                   GtkTreeIter *toplevel, GtkTreeIter *child) {
  char newPath[PATH_MAX + 1];
  DIR *dir = NULL;
  struct dirent entry;
  struct dirent *entryPtr = NULL;
  int retval = 0;
  unsigned count = 0;
  int sec_count = 0;
  /* открыть указанный каталог, если возможно. */
  dir = opendir(pathName);
  if (dir == NULL) {
    printf("Error opening %s: %s", pathName, strerror(errno));
    return 0;
  }
  retval = readdir_r(dir, &entry, &entryPtr);
  while (entryPtr != NULL) {
    struct stat entryInfo;
    if (entry.d_name[0] == '.' || entry.d_name[1] == '.') {
      retval = readdir_r(dir, &entry, &entryPtr);
      continue;
    }
    (void)strncpy(newPath, pathName, PATH_MAX);
    (void)strncat(newPath, "/", PATH_MAX);
    (void)strncat(newPath, entry.d_name, PATH_MAX);
    if (lstat(newPath, &entryInfo) == 0) {
      count++;
      if (S_ISDIR(entryInfo.st_mode)) {
        //                printf("new dir: %s/\n", newPath);
        gtk_tree_store_append(treestore, toplevel, NULL);
        gtk_tree_store_set(treestore, toplevel, COL_FIRST_NAME, entry.d_name,
                           -1);
        //                printf("+ %s\n", entry.d_name);
        count += scan_directory(newPath, treestore, toplevel, child);
      } else if (S_ISREG(entryInfo.st_mode)) {
        //                printf("\t%s has %lld bytes\n", newPath, (long
        //                long)entryInfo.st_size );
        gtk_tree_store_append(treestore, child, toplevel);
        gtk_tree_store_set(treestore, child, COL_FIRST_NAME, entry.d_name, -1);
        //                printf("  %s has %lld bytes\n", entry.d_name,
        //                       (long long)entryInfo.st_size);
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
    retval = readdir_r(dir, &entry, &entryPtr);
  }
  (void)closedir(dir);
  return count;
}

static GtkTreeModel *create_and_fill_model(char *pathName) {
  GtkTreeStore *treestore;
  GtkListStore *liststore;
  GtkTreeIter toplevel, child, toplevel2, child2;

  treestore =
      gtk_tree_store_new(NUM_COLS, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_UINT);
  liststore =
          gtk_list_store_new(NUM_COLS, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_UINT);

      /* Append a top level row and leave it empty */
      gtk_tree_store_append(treestore, &toplevel, NULL);
      gtk_tree_store_set(treestore, &toplevel,
                         COL_FIRST_NAME, "Maria",
                         COL_LAST_NAME, "Incognito",
                         -1);

      /* Append a second top level row, and fill it with some data */
      gtk_tree_store_append(treestore, &toplevel, NULL);
      gtk_tree_store_set(treestore, &toplevel,
                         COL_FIRST_NAME, "Jane",
                         COL_LAST_NAME, "Average",
                         COL_YEAR_BORN, (guint) 1962,
                         -1);

      /* Append a child to the second top level row, and fill in some data */
      gtk_tree_store_append(treestore, &child, &toplevel);
      gtk_tree_store_set(treestore, &child,
                         COL_FIRST_NAME, "Janita",
                         COL_LAST_NAME, "Average",
                         COL_YEAR_BORN, (guint) 1985,
                         -1);
    gtk_tree_store_append(treestore, &child, &toplevel);
    gtk_tree_store_set(treestore, &child,
                       COL_FIRST_NAME, "Janita2",
                       COL_LAST_NAME, "Average",
                       COL_YEAR_BORN, (guint) 1985,
                       -1);
    gtk_tree_store_append(treestore, &child, &child);
    gtk_tree_store_set(treestore, &child,
                       COL_FIRST_NAME, "Pod_Janita",
                       COL_LAST_NAME, "Average",
                       COL_YEAR_BORN, (guint) 1985,
                       -1);
    gtk_tree_store_append(treestore, &child, &child);
    gtk_tree_store_set(treestore, &child,
                       COL_FIRST_NAME, "Pod_Janita2",
                       COL_LAST_NAME, "Average",
                       COL_YEAR_BORN, (guint) 1985,
                       -1);
//  scan_directory(pathName, treestore, &toplevel, &child);
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

  view = create_view_and_model("/home/mark/for_scan");

  gtk_container_add(GTK_CONTAINER(window), view);

  gtk_widget_show_all(window);

  gtk_main();

  return 0;
}
