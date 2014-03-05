set(MAIN_SOURCES
    main
)
set(CORE_SOURCES
    wall
    wall_move
    walk_move
    board
    board_graph
    pawn
    player_factory
    player
    fake_player
)
set(STATE_SOURCES
    main_menu_state
    start_game_state
    game_state
    state_manager
)
set(UI_SOURCES
    ui_factory
)
set(UI_NCURSES_SOURCES
    ncurses_menu
    ncurses_window
    ncurses_ui
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
foreach (source ${STATE_SOURCES})
    LIST(APPEND SOURCES state/${source}.cpp)
    add_library(${source}_cxx_obj OBJECT state/${source}.cpp)
endforeach()
foreach (source ${UI_SOURCES})
    LIST(APPEND SOURCES UI/${source}.cpp)
    add_library(${source}_cxx_obj OBJECT UI/${source}.cpp)
endforeach()
foreach (source ${UI_NCURSES_SOURCES})
    LIST(APPEND SOURCES UI/ncurses/${source}.cpp)
    add_library(${source}_cxx_obj OBJECT UI/ncurses/${source}.cpp)
endforeach()

set(ADDITIONAL_LIBRARIES boost_program_options boost_log boost_log_setup boost_thread menu ncurses pthread)

include_directories(${${PROJECT}_SOURCE_DIR}/src)
include_directories(${${PROJECT}_SOURCE_DIR}/)

add_executable(${PROJECT} ${SOURCES})
set_target_properties(${PROJECT} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
target_link_libraries(${PROJECT} ${ADDITIONAL_LIBRARIES})
