
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(top_dir ${CMAKE_CURRENT_SOURCE_DIR})
set(top_binary_dir ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(top_builddir ${CMAKE_CURRENT_BINARY_DIR}/build)
set(top_include_dir ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(top_srcdir ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(top_testdir ${CMAKE_CURRENT_SOURCE_DIR}/test)
set(top_3rdpartydir ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty)


function(search_file dir result)
    file(GLOB_RECURSE __FILES_TARGET__
        "${dir}/*.hpp"
        "${dir}/*.h"
        "${dir}/*.cpp"
        "${dir}/*.cc"
        "${dir}/*.c"
    )
    SET(${result} ${__FILES_TARGET__} PARENT_SCOPE)
endfunction()