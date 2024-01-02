cmake_minimum_required(VERSION 2.8.12)

set(project_name examples)

project(${project_name})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${top_binary_dir})

add_executable(examples main.cpp)

include_directories(${top_include_dir})

target_link_libraries(examples ${top_binary_dir}/libextension.so)

target_link_libraries(${project_name} ${thirdparty_libs})