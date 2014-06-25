include_directories(${${PROJECT}_SOURCE_DIR})
include_directories(${${PROJECT}_SOURCE_DIR}/src)
include_directories(${${PROJECT}_SOURCE_DIR}/src/state)
include_directories(/usr/local/cegui/include/cegui-0/)

# game test
set(TEST_TARGET game_test)
set(TEST_TARGET_ADDITIONAL_LIBS boost_log boost_log_setup boost_thread boost_system pthread)
add_executable(${TEST_TARGET}
    $<TARGET_OBJECTS:game_cxx_obj>
    $<TARGET_OBJECTS:wall_cxx_obj>
    $<TARGET_OBJECTS:wall_grid_cxx_obj>
    $<TARGET_OBJECTS:wall_move_cxx_obj>
    $<TARGET_OBJECTS:walk_move_cxx_obj>
    $<TARGET_OBJECTS:node_cxx_obj>
    $<TARGET_OBJECTS:board_graph_cxx_obj>
    $<TARGET_OBJECTS:pawn_cxx_obj>
    mock_player.cpp
    game_test.cpp)
target_link_libraries(${TEST_TARGET} ${TEST_TARGET_ADDITIONAL_LIBS})
set_target_properties(${TEST_TARGET} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
add_test(NAME game_test
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    COMMAND ./game_test)

# board graph unittests
set(TEST_TARGET board_graph_unittest)
set(TEST_TARGET_ADDITIONAL_LIBS gtest)
add_executable(${TEST_TARGET}
    $<TARGET_OBJECTS:node_cxx_obj>
    $<TARGET_OBJECTS:board_graph_cxx_obj>
    ${TEST_TARGET}.cpp)
target_link_libraries(${TEST_TARGET} ${TEST_TARGET_ADDITIONAL_LIBS})
set_target_properties(${TEST_TARGET} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
add_test(NAME ${TEST_TARGET}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    COMMAND ./${TEST_TARGET})

# wall grid unittests
set(TEST_TARGET wall_grid_unittest)
set(TEST_TARGET_ADDITIONAL_LIBS gtest)
add_executable(${TEST_TARGET}
    $<TARGET_OBJECTS:node_cxx_obj>
    $<TARGET_OBJECTS:wall_cxx_obj>
    $<TARGET_OBJECTS:wall_grid_cxx_obj>
    wall_grid_unittest.cpp)
target_link_libraries(${TEST_TARGET} ${TEST_TARGET_ADDITIONAL_LIBS})
set_target_properties(${TEST_TARGET} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
add_test(NAME wall_grid_unittest
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    COMMAND ./wall_grid_unittest)

# pos utils unittests
set(TEST_TARGET pos_utils_unittest)
set(TEST_TARGET_ADDITIONAL_LIBS gtest)
add_executable(${TEST_TARGET}
    $<TARGET_OBJECTS:node_cxx_obj>
    $<TARGET_OBJECTS:wall_cxx_obj>
    $<TARGET_OBJECTS:pos_utils_cxx_obj>
    pos_utils_unittest.cpp)
target_link_libraries(${TEST_TARGET} ${TEST_TARGET_ADDITIONAL_LIBS})
set_target_properties(${TEST_TARGET} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
add_test(NAME pos_utils_unittest
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    COMMAND ./pos_utils_unittest)
