#!/bin/bash

rm -rf out
rm -rf dist
rm -rf html

mkdir dist

clang -std=c11 `pkg-config --cflags --libs gtk+-3.0` -Wall -Wextra -lm -o dist/main src/*.c
clang -std=c11 `pkg-config --cflags --libs gtk+-3.0` -Wall -Wextra -lm -o dist/test test/testmain.c src/get_path.c src/mask.c test/tests.c

clang-format -i src/*.c src/get_path.h src/gtk_librarys.h
clang-tidy src/*.c src/get_path.h src/gtk_librarys.h
doxygen Doxyfile
