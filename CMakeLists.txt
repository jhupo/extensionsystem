cmake_minimum_required(VERSION 2.8)

include(cmake/commondefs.cmake)

project(extension)

search_file(${top_include_dir} TARGET_FILES)

set(CMAKE_BINARY_DIR ${top_builddir})

set(EXECUTABLE_OUTPUT_PATH ${top_binary_dir})



