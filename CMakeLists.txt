cmake_minimum_required(VERSION 2.8)

message("Starting cmake")

project(quoridor CXX)
set(PROJECT quoridor)

add_definitions(-DBOOST_LOG_DYN_LINK)
set(CMAKE_CXX_FLAGS "-W -Wall -Wextra -Werror -pedantic -std=c++0x -O0 -ggdb")

option(DEBUG "debug mode" OFF)
if (DEBUG)
    message("turn on debugging")
    add_definitions(-DDEBUG)
endif()

enable_testing()

add_subdirectory(src)
add_subdirectory(t)
