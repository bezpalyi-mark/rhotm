cmake_minimum_required(VERSION 3.14)
project(GTK_IMAGE)

set(CMAKE_CXX_STANDARD 14)

add_executable(GTK_IMAGE src/main.cpp)

add_custom_target(
        build ALL
        COMMAND rm -rf out dist html
        COMMAND mkdir out dist html
        COMMAND clang++ -lm -std=c++14 `Magick++-config --cppflags --cxxflags --ldflags --libs` -Wall -Wextra -Werror -Wpedantic src/*.cpp -o dist/main.out
        COMMAND clang-format -i src/main.cpp
        COMMAND clang-tidy src/main.cpp -fix-errors  -checks=* -- -fblocks
        COMMAND doxygen Doxyfile
)

find_package(ImageMagick COMPONENTS Magick++)
include_directories(${ImageMagick_INCLUDE_DIRS})
target_link_libraries(GTK_IMAGE ${ImageMagick_LIBRARIES})