#!/bin/bash

rm -rf out
rm -rf dist
rm -rf html

mkdir dist

clang -std=c11 `pkg-config --cflags --libs gtk+-3.0` -o dist/main.out src/*.c

clang-format -i src/*.c src/get_path.h src/gtk_librarys.h
clang-tidy src/*.c src/get_path.h src/gtk_librarys.h
