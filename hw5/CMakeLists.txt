cmake_minimum_required(VERSION 3.15)
project(hw5)

set(CMAKE_C_STANDARD 99)

add_definitions(-DCOLOR -DVERBOSE)

include_directories(.)
include_directories(include)
#include_directories(lib)
#include_directories(src)


add_executable(hw5
        include/client_registry.h
        include/debug.h
        include/csapp.h
        include/exchange.h
        include/protocol.h
        include/server.h
        include/trader.h
        include/csapp.h
        src/csapp.c
        src/main.c
        src/protocol.c src/client_registry.c src/server.c include/client.h src/trader.c include/trader_struct.h src/exchange.c src/matchmaking.c include/matchmaking.h include/exchange_struct.h)

target_link_directories(hw5 PUBLIC "include")

target_link_libraries(hw5 ${CMAKE_SOURCE_DIR}/lib/bourse.a)
target_link_libraries(hw5 ${CMAKE_SOURCE_DIR}/lib/bourse_debug.a)
target_link_libraries(hw5 criterion pthread)

add_executable(hw5_debug
        include/client_registry.h
        include/debug.h
        include/exchange.h
        include/protocol.h
        include/server.h
        include/trader.h
        include/csapp.h
        src/csapp.c
        tests/bourse_tests.c "src/protocol.c" src/client_registry.c src/server.c include/client.h src/trader.c include/trader_struct.h src/exchange.c src/matchmaking.c include/matchmaking.h include/exchange_struct.h)

target_link_libraries(hw5_debug PRIVATE ${CRITERION_LINK_LIBRARIES})
target_include_directories(hw5_debug PRIVATE ${CRITERION_INCLUDE_DIRS})
target_link_libraries(hw5 criterion pthread)
