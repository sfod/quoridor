set(MAIN_SOURCES
    main
)
set(CORE_SOURCES
    wall
    wall_move
    walk_move
    pos
    board
    board_graph
    pawn
    player_factory
    player
    fake_player
)
set(STATE_SOURCES
    dragged_window
    main_menu_state
    start_game_state
    game_state
    state_manager
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

set(ADDITIONAL_LIBRARIES boost_program_options boost_log boost_log_setup boost_thread boost_system pthread SDL2 GL CEGUIOpenGLRenderer-0_d CEGUIBase-0_d)

include_directories(${${PROJECT}_SOURCE_DIR}/src)
include_directories(${${PROJECT}_SOURCE_DIR}/)
include_directories(/usr/local/cegui/include/cegui-0/)

link_directories(/usr/local/cegui/lib/)

add_executable(${PROJECT} ${SOURCES})
set_target_properties(${PROJECT} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
target_link_libraries(${PROJECT} ${ADDITIONAL_LIBRARIES})
