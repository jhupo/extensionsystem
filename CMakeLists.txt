cmake_minimum_required(VERSION 2.8)

include(cmake/commondefs.cmake)

project(extension)

add_definitions(-DEXTENSION_LIBRARY)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${top_binary_dir})

include_directories(${top_include_dir})

search_file(${top_srcdir} TARGET_FILES)

add_library(${PROJECT_NAME} SHARED ${TARGET_FILES})