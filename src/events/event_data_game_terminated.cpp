#include "event_data_game_terminated.hpp"
#include <functional>

template<> EventType EventData_GameTerminated::Base::event_type_ = std::hash<std::string>()("GameTerminated");
