set(MAIN_SOURCE_CXX main.cpp)
set(BOARD_SOURCES_CXX board)
set(GAME_SOURCES_CXX game)
set(PLAYER_SOURCES_CXX player)

set(SOURCES)
set(HEADERS)

foreach (source ${BOARD_SOURCES_CXX})
    LIST(APPEND SOURCES ${source}.cpp)
    LIST(APPEND HEADERS ${source}.hpp)
    add_library(${source}_cxx_obj OBJECT ${source}.cpp)
endforeach()

foreach (source ${GAME_SOURCES_CXX})
    LIST(APPEND SOURCES ${source}.cpp)
    LIST(APPEND HEADERS ${source}.hpp)
    add_library(${source}_cxx_obj OBJECT ${source}.cpp)
endforeach()

foreach (source ${PLAYER_SOURCES_CXX})
    LIST(APPEND SOURCES ${source}.cpp)
    LIST(APPEND HEADERS ${source}.hpp)
    add_library(${source}_cxx_obj OBJECT ${source}.cpp)
endforeach()

add_executable(${PROJECT} ${MAIN_SOURCE_CXX} ${SOURCES})
set_target_properties(${PROJECT} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ../bin)
