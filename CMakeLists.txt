cmake_minimum_required(VERSION 3.22)
project(proyecto_sha256)

set(CMAKE_CXX_STANDARD 17)

add_executable(proyecto_sha256
        main.cpp circular_list.h node.h list.h)
