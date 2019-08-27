cmake_minimum_required(VERSION 3.14)
project(GTK_IMAGE)

set(CMAKE_CXX_STANDARD 17)

add_executable(GTK_IMAGE src/main.cpp src/Functions.cpp src/Functions.cpp src/Functions.h src/MainWindow.h)

add_custom_target(
        build ALL
        COMMAND rm -rf out dist html
        COMMAND mkdir out dist html
        COMMAND clang++ -lm -std=c++17 `Magick++-config --cppflags --cxxflags --ldflags --libs` `pkg-config --cflags --libs gtk+-3.0` -Wall -Wextra -Werror -Wpedantic src/*.cpp -lstdc++fs -o dist/main.out -lpthread
        COMMAND clang-format -i src/main.cpp
        COMMAND clang-tidy src/main.cpp -fix-errors  -checks=* -- -fblocks
        COMMAND doxygen Doxyfile
)

find_package(ImageMagick COMPONENTS Magick++)
include_directories(${ImageMagick_INCLUDE_DIRS})
target_link_libraries(GTK_IMAGE ${ImageMagick_LIBRARIES})

target_link_libraries(GTK_IMAGE stdc++fs)
target_link_libraries(GTK_IMAGE pthread)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})

add_definitions(${GTK3_CFLAGS_OTHER})

target_link_libraries(GTK_IMAGE ${GTK3_LIBRARIES})
