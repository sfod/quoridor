cmake_minimum_required(VERSION 2.8)

message("Starting cmake")

project(quoridor CXX)
set(PROJECT quoridor)

add_definitions(-DBOOST_LOG_DYN_LINK)
set(CMAKE_CXX_FLAGS "-W -Wall -Wextra -Werror -pedantic -std=c++0x -O0 -ggdb")

option(DEBUG "debug mode" OFF)
if (DEBUG)
    message("Turn on debugging")
    add_definitions(-DDEBUG)
endif()

option(USE_BOARD_GRAPH_CACHE "use cache" ON)
if (USE_BOARD_GRAPH_CACHE)
    message("Compile with cache support")
    add_definitions(-DUSE_BOARD_GRAPH_CACHE)
endif()

enable_testing()

add_subdirectory(src)
add_subdirectory(t)
