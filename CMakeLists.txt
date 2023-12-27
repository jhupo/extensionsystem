cmake_minimum_required(VERSION 2.8)

set(project_name extension)

include(cmake/commondefs.cmake)

project(${project_name})

add_definitions(-DEXTENSION_LIBRARY)
add_definitions(-DBOOST_LOG_DYN_LINK)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${top_binary_dir})

include_directories(${top_include_dir})

search_file(${top_srcdir} TARGET_FILES)

add_library(${project_name} SHARED ${TARGET_FILES})

target_link_libraries(${project_name} ${thirdparty_libs})

add_subdirectory(${top_examples})