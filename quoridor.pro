TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++11
QT += qml quick widgets

INCLUDEPATH += src/

SOURCES += \
    src/main.cpp \
    src/game/game_app.cpp \
    src/game/game_logic.cpp \
    src/view/main_menu_view.cpp \
    src/events/event_manager.cpp \
    src/events/event_caller.cpp \
    src/actors/actor.cpp \
    src/actors/actor_factory.cpp \
    src/actors/actor_keeper.cpp \
    src/actors/graph_component.cpp \
    src/graph/graph.cpp \
    src/graph/node.cpp \
    src/view/options_view.cpp \
    src/view/qt_view.cpp \
    src/view/game_view.cpp \
    src/game/player_handler.cpp \
    src/view/ai_view.cpp \
    src/actors/ai_component.cpp \
    src/AI/randomer_brain.cpp \
    src/graph/board_graph.cpp \
    src/AI/straight_brain.cpp \
    src/graph/wall.cpp \
    src/actors/wall_component.cpp \
    src/graph/wall_grid.cpp \
    src/events/event_data_main_menu.cpp \
    src/events/event_data_options.cpp \
    src/events/event_data_game.cpp \
    src/events/event_data_game_terminated.cpp \
    src/events/event_data_game_finished.cpp \
    src/events/event_data_quit.cpp \
    src/events/event_data_request_new_actor.cpp \
    src/events/event_data_new_actor.cpp \
    src/events/event_data_request_actor_move.cpp \
    src/events/event_data_request_set_wall.cpp \
    src/events/event_data_move_actor.cpp \
    src/events/event_data_set_wall.cpp \
    src/events/event_data_set_actor_possible_moves.cpp \
    src/events/event_data_set_actor_active.cpp

RESOURCES += qml.qrc \
    configs.qrc \
    images.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    src/game/game_app.hpp \
    src/view/iview.hpp \
    src/game/game_logic.hpp \
    src/view/main_menu_view.hpp \
    src/events/event_manager.hpp \
    src/events/event_data.hpp \
    src/events/event_caller.hpp \
    src/actors/actor.hpp \
    src/actors/actor_component.hpp \
    src/actors/actor_factory.hpp \
    src/actors/actor_keeper.hpp \
    src/actors/graph_component.hpp \
    src/graph/graph.hpp \
    src/graph/node.hpp \
    src/view/options_view.hpp \
    src/view/qt_view.hpp \
    src/view/game_view.hpp \
    src/game/player_handler.hpp \
    src/view/ai_view.hpp \
    src/AI/brain.hpp \
    src/actors/ai_component.hpp \
    src/AI/randomer_brain.hpp \
    src/game/game_data.hpp \
    src/graph/board_graph.hpp \
    src/AI/straight_brain.hpp \
    src/graph/wall.hpp \
    src/actors/wall_component.hpp \
    src/graph/wall_grid.hpp \
    src/exceptions/exception.hpp \
    src/events/event_data_main_menu.hpp \
    src/events/event_data_options.hpp \
    src/events/event_data_game.hpp \
    src/events/event_data_game_terminated.hpp \
    src/events/event_data_game_finished.hpp \
    src/events/event_data_quit.hpp \
    src/events/event_data_request_new_actor.hpp \
    src/events/event_data_new_actor.hpp \
    src/events/event_data_request_actor_move.hpp \
    src/events/event_data_request_set_wall.hpp \
    src/events/event_data_move_actor.hpp \
    src/events/event_data_set_wall.hpp \
    src/events/event_data_set_actor_possible_moves.hpp \
    src/events/event_data_set_actor_active.hpp
