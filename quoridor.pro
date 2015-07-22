TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++11
QT += qml quick widgets

SOURCES += main.cpp \
    game/game_app.cpp \
    game/game_logic.cpp \
    view/main_menu_view.cpp \
    events/event_manager.cpp \
    events/event_caller.cpp \
    actors/actor.cpp \
    actors/actor_factory.cpp \
    actors/actor_keeper.cpp \
    actors/graph_component.cpp \
    graph/graph.cpp \
    graph/node.cpp \
    view/options_view.cpp \
    view/qt_view.cpp \
    view/game_view.cpp \
    game/player_handler.cpp \
    view/ai_view.cpp \
    actors/ai_component.cpp \
    AI/randomer_brain.cpp \
    graph/board_graph.cpp \
    AI/straight_brain.cpp \
    graph/wall.cpp \
    actors/wall_component.cpp \
    graph/wall_grid.cpp \
    events/event_data_main_menu.cpp \
    events/event_data_options.cpp \
    events/event_data_game.cpp \
    events/event_data_game_terminated.cpp \
    events/event_data_game_finished.cpp \
    events/event_data_quit.cpp \
    events/event_data_request_new_actor.cpp \
    events/event_data_new_actor.cpp \
    events/event_data_request_actor_move.cpp \
    events/event_data_request_set_wall.cpp \
    events/event_data_move_actor.cpp \
    events/event_data_set_wall.cpp \
    events/event_data_set_actor_possible_moves.cpp \
    events/event_data_set_actor_active.cpp

RESOURCES += qml.qrc \
    configs.qrc \
    images.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    game/game_app.hpp \
    view/iview.hpp \
    game/game_logic.hpp \
    view/main_menu_view.hpp \
    events/event_manager.hpp \
    events/event_data.hpp \
    events/event_caller.hpp \
    actors/actor.hpp \
    actors/actor_component.hpp \
    actors/actor_factory.hpp \
    actors/actor_keeper.hpp \
    actors/graph_component.hpp \
    graph/graph.hpp \
    graph/node.hpp \
    view/options_view.hpp \
    view/qt_view.hpp \
    view/game_view.hpp \
    game/player_handler.hpp \
    view/ai_view.hpp \
    AI/brain.hpp \
    actors/ai_component.hpp \
    AI/randomer_brain.hpp \
    game/game_data.hpp \
    graph/board_graph.hpp \
    AI/straight_brain.hpp \
    graph/wall.hpp \
    actors/wall_component.hpp \
    graph/wall_grid.hpp \
    exceptions/exception.hpp \
    events/event_data_main_menu.hpp \
    events/event_data_options.hpp \
    events/event_data_game.hpp \
    events/event_data_game_terminated.hpp \
    events/event_data_game_finished.hpp \
    events/event_data_quit.hpp \
    events/event_data_request_new_actor.hpp \
    events/event_data_new_actor.hpp \
    events/event_data_request_actor_move.hpp \
    events/event_data_request_set_wall.hpp \
    events/event_data_move_actor.hpp \
    events/event_data_set_wall.hpp \
    events/event_data_set_actor_possible_moves.hpp \
    events/event_data_set_actor_active.hpp
