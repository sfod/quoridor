#include "event_data_options.hpp"
#include <functional>

template<> EventType EventData_Options::Base::event_type_ = std::hash<std::string>()("Options");
