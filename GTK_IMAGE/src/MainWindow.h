//
// Created by hunterlan on 6/12/19.
//

#ifndef GTK_IMAGE_MAINWINDOW_H
#define GTK_IMAGE_MAINWINDOW_H

#include <iostream>
#include <Magick++.h>
#include <gtk/gtk.h>
#include <string>
#include <thread>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

const int WIDTH = 1920;
const int HEIGHT = 1080;
const int COUNT_THREADS = 10;

using namespace std;
using namespace Magick;

struct MainWindow{
    GtkWidget * window, * vbox;
    GtkWidget * label, * entryPath, * button, * labelSave, * entrySave;
};

#endif //GTK_IMAGE_MAINWINDOW_H
