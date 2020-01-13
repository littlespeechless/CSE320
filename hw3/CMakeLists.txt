cmake_minimum_required(VERSION 3.15)
project(hw3 C)

set(CMAKE_C_STANDARD 11)

include_directories(include)
include_directories(src)
find_package(PkgConfig REQUIRED)
pkg_check_modules(CRITERION REQUIRED criterion)

add_executable(hw3
        include/debug.h
        include/sfmm.h
        src/main.c
        src/sfmm.c
        tests/sfmm_tests.c
        lib/sfutil.o)

target_link_libraries(hw3 PRIVATE ${CRITERION_LINK_LIBRARIES})
target_include_directories(hw3 PRIVATE ${CRITERION_INCLUDE_DIRS})

add_executable(hw3_debug
        include/debug.h
        include/sfmm.h
        src/sfmm.c
        tests/sfmm_tests.c
        lib/sfutil.o)
target_link_libraries(hw3_debug PRIVATE ${CRITERION_LINK_LIBRARIES})
target_include_directories(hw3_debug PRIVATE ${CRITERION_INCLUDE_DIRS})