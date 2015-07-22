#include "event_data_quit.hpp"
#include <functional>

template<> EventType EventData_Quit::Base::event_type_ = std::hash<std::string>()("Quit");
