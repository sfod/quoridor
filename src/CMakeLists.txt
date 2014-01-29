set(MAIN_SOURCES
    main
)
set(CORE_SOURCES
    wall
    wall_move
    walk_move
    board
    board_graph
    game
    pawn
    player_factory
    player
    fake_player
)
set(UI_SOURCES
    runner
    ui_factory
    ui
    ncurses_ui_impl
)

set(SOURCES)
foreach (source ${MAIN_SOURCES})
    LIST(APPEND SOURCES ${source}.cpp)
    add_library(${source}_cxx_obj OBJECT ${source}.cpp)
endforeach()
foreach (source ${CORE_SOURCES})
    LIST(APPEND SOURCES ${source}.cpp)
    add_library(${source}_cxx_obj OBJECT ${source}.cpp)
endforeach()
foreach (source ${UI_SOURCES})
    LIST(APPEND SOURCES UI/${source}.cpp)
    add_library(${source}_cxx_obj OBJECT UI/${source}.cpp)
endforeach()

set(ADDITIONAL_LIBRARIES boost_program_options ncurses)

include_directories(${${PROJECT}_SOURCE_DIR}/src)
include_directories(${${PROJECT}_SOURCE_DIR}/)

add_executable(${PROJECT} ${SOURCES})
set_target_properties(${PROJECT} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
target_link_libraries(${PROJECT} ${ADDITIONAL_LIBRARIES})
