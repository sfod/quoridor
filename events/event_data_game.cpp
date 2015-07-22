#include "event_data_game.hpp"
#include <functional>

template<> EventType EventData_Game::Base::event_type_ = std::hash<std::string>()("Game");
