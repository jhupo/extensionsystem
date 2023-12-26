cmake_minimum_required(VERSION 2.8)

set(project_name examples)

include(../cmake/commondefs.cmake)

project(${project_name})

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${top_binary_dir})

include_directories("../include")

add_executable(examples main.cpp)
