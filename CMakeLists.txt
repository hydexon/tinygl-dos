cmake_minimum_required(VERSION 3.5)

project(TinySDGL)

add_compile_options(-5s -s -d2 -zq -fr=nul -za99)
add_compile_options(-wcd=202 -wcd=303 -wcd=138 -wcd=201)

string(REPLACE "-bt=dos" "-bt=dos4g" CMAKE_C_FLAGS ${CMAKE_C_FLAGS})

include_directories(./include)
add_subdirectory(src)
add_subdirectory(examples)
