cmake_minimum_required(VERSION 3.15)
project(hw4)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)
include_directories(lib)
include_directories(src)

add_definitions(-DCOLOR -DVERBOSE)

add_executable(hw4
        include/debug.h
        include/jobber.h
        include/sf_readline.h
        include/task.h
        lib/sf_event.o
        src/jobs.c
        src/main.c
        src/sf_readline.c
        include/handler.h
        src/task.c
        src/handler.c)


target_link_libraries(hw4 PRIVATE ${CRITERION_LINK_LIBRARIES})
target_include_directories(hw4 PRIVATE ${CRITERION_INCLUDE_DIRS})

add_executable(hw4_debug
        include/debug.h
        include/jobber.h
        include/sf_readline.h
        include/task.h
        lib/sf_event.o
        src/jobs.c
        src/main.c
        src/sf_readline.c
        include/handler.h
        src/task.c
        src/handler.c
        tests/hw4_tests.c)

target_link_libraries(hw4_debug PRIVATE ${CRITERION_LINK_LIBRARIES})
target_include_directories(hw4_debug PRIVATE ${CRITERION_INCLUDE_DIRS})
