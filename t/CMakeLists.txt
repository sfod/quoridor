include_directories(${${PROJECT}_SOURCE_DIR})
include_directories(${${PROJECT}_SOURCE_DIR}/src)

# board unittests
set(TEST_TARGET board_unittest)
set(TEST_TARGET_ADDITIONAL_LIBS gtest)
add_executable(${TEST_TARGET}
    $<TARGET_OBJECTS:pawn_cxx_obj>
    $<TARGET_OBJECTS:pos_cxx_obj>
    $<TARGET_OBJECTS:wall_cxx_obj>
    $<TARGET_OBJECTS:board_graph_cxx_obj>
    $<TARGET_OBJECTS:board_cxx_obj>
    board_unittest.cpp)
target_link_libraries(${TEST_TARGET} ${TEST_TARGET_ADDITIONAL_LIBS})
set_target_properties(${TEST_TARGET} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
add_test(NAME board_unittest
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    COMMAND ./board_unittest)

# node utils unittests
set(TEST_TARGET node_utils_unittest)
set(TEST_TARGET_ADDITIONAL_LIBS gtest)
add_executable(${TEST_TARGET}
    $<TARGET_OBJECTS:node_cxx_obj>
    $<TARGET_OBJECTS:node_utils_cxx_obj>
    node_utils_unittest.cpp)
target_link_libraries(${TEST_TARGET} ${TEST_TARGET_ADDITIONAL_LIBS})
set_target_properties(${TEST_TARGET} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
add_test(NAME node_utils_unittest
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    COMMAND ./node_utils_unittest)
