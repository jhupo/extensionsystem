
set(top_dir ${CMAKE_CURRENT_SOURCE_DIR})
set(top_binary_dir ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(top_builddir ${CMAKE_CURRENT_SOURCE_DIR}/build)
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


if(NOT cmake_build_type)
    set(cmake_build_type "Debug")
endif()

if(cmake_build_type STREQUAL "Debug")
    set(cmake_build_type "Debug")
elseif(cmake_build_type STREQUAL "Release")
    set(cmake_build_type "Release")
elseif(cmake_build_type STREQUAL "RelWithDebInfo")
    set(cmake_build_type "RelWithDebInfo")
else()
    message(FATAL_ERROR "cmake_build_type must be Debug, Release, RelWithDebInfo or MinSizeRel")
endif()

add_compile_options(-fPIC)

set(cmake_cxx_flags_debug "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
set(cmake_cxx_flags_release "$ENV{CXXFLAGS} -DNODEBUG -O2 -Wall")
set(cmake_cxx_flags_relwithdebinfo "$ENV{CXXFLAGS} -Wall -O2 -g -DNDEBUG")

set(cmake_c_flags_debug "$ENV{CFLAGS} -O0 -Wall -g -ggdb")
set(cmake_c_flags_release "$ENV{CFLAGS} -DNODEBUG -O2 -Wall")
set(cmake_c_flags_relwithdebinfo "$ENV{CXXFLAGS} -Wall -O2 -g -DNDEBUG")


set(thirdparty_libs 
                    boost_thread
                    boost_chrono
                    boost_log
                    boost_log_setup
                    boost_regex
                    boost_filesystem
                    m
                    dl
                    pthread)