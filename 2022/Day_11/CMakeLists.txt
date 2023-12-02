cmake_minimum_required(VERSION 3.24)
project(Day_11 C)

set(CMAKE_C_STANDARD 11)
add_executable(Day_11 main.c)

add_library(cgs STATIC IMPORTED)
include_directories(include/cgs)

find_library(CGS_LIBRARY cgs lib)

target_link_libraries(Day_11 LINK_PUBLIC ${CGS_LIBRARY})

