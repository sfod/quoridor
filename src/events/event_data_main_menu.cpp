#include "event_data_main_menu.hpp"
#include <functional>

template<> EventType EventData_MainMenu::Base::event_type_ = std::hash<std::string>()("MainMenu");
