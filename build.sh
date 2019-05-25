#!/bin/bash

rm -rf out
rm -rf dist
rm -rf html

mkdir dist

clang -std=c11 `pkg-config --cflags --libs gtk+-3.0` -o dist/main.out src/main_with_size.c

clang-format -i src/main.c
