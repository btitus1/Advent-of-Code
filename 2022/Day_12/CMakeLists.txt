cmake_minimum_required(VERSION 3.24)
project(Day_12 C)

set(CMAKE_C_STANDARD 11)
add_executable(Day_12 main.c)

add_library(cgs STATIC IMPORTED)
include_directories(include/cgs)
include_directories(include/fruity)

find_library(CGS_LIBRARY cgs lib)
find_library(FRUITY_LIBRARY fruity lib)

target_link_libraries(Day_12 LINK_PUBLIC ${CGS_LIBRARY} ${FRUITY_LIBRARY})

