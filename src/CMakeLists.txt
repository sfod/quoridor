set(MAIN_SOURCES
    main
)
set(CORE_SOURCES
    game
    wall
    wall_grid
    wall_move
    walk_move
    node
    board_graph
    pawn
)
set(PLAYER_SOURCES
    player_factory
    player
    stupid_player
    middling_player
)
set(STATE_SOURCES
    main_menu_state
    start_game_state
    game_state
    state_manager
    pos_utils
)
set(CEGUI_EXT_SOURCES
    drag_event
    draggable_window
)

set(SOURCES)
foreach (source ${MAIN_SOURCES})
    add_library(${source}_cxx_obj OBJECT ${source}.cpp)
    LIST(APPEND SOURCES $<TARGET_OBJECTS:${source}_cxx_obj>)
endforeach()
foreach (source ${CORE_SOURCES})
    add_library(${source}_cxx_obj OBJECT ${source}.cpp)
    LIST(APPEND SOURCES $<TARGET_OBJECTS:${source}_cxx_obj>)
endforeach()
foreach (source ${PLAYER_SOURCES})
    add_library(${source}_cxx_obj OBJECT player/${source}.cpp)
    LIST(APPEND SOURCES $<TARGET_OBJECTS:${source}_cxx_obj>)
endforeach()
foreach (source ${STATE_SOURCES})
    add_library(${source}_cxx_obj OBJECT state/${source}.cpp)
    LIST(APPEND SOURCES $<TARGET_OBJECTS:${source}_cxx_obj>)
endforeach()
foreach (source ${CEGUI_EXT_SOURCES})
    add_library(${source}_cxx_obj OBJECT cegui_ext/${source}.cpp)
    LIST(APPEND SOURCES $<TARGET_OBJECTS:${source}_cxx_obj>)
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
