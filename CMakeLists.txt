cmake_minimum_required(VERSION 3.10)

project(orz)

add_definitions(-D_GNU_SOURCE)

set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_C_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)
set_source_files_properties(src/orz_endian.c PROPERTIES COMPILE_FLAGS "-fvisibility=default")

add_library(orz SHARED 
    src/orz_util.c
    src/orz_algorithm.c
    src/orz_endian.c
    src/orz_tostring.c
    src/orz_retval.c
    src/orz_date.c
)


target_include_directories(orz PRIVATE include)


if (ORZ_TEST) 
    add_subdirectory(test)
    enable_testing()
endif()

install(TARGETS orz LIBRARY DESTINATION lib)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ DESTINATION include)

