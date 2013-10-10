set(MAIN_SOURCES
    main
    board
    game
    pawn
)

set(SOURCES)
foreach (source ${MAIN_SOURCES})
    LIST(APPEND SOURCES ${source}.cpp)
    add_library(${source}_cxx_obj OBJECT ${source}.cpp)
endforeach()

set(ADDITIONAL_LIBRARIES)

include_directories(${${PROJECT}_SOURCE_DIR}/src)
include_directories(${${PROJECT}_SOURCE_DIR}/)

add_executable(${PROJECT} ${SOURCES})
set_target_properties(${PROJECT} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
target_link_libraries(${PROJECT} ${ADDITIONAL_LIBRARIES})
