cmake_minimum_required(VERSION 2.8)

message("Starting cmake")

set(PROJECT quoridor)

set(CMAKE_CXX_FLAGS "-W -Wall -Wextra -Werror -pedantic -std=c++0x -O0 -ggdb")

enable_testing()

add_subdirectory(src)
add_subdirectory(t)
