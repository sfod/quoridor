#pragma once

#include <stdexcept>

class invalid_argument_error : public std::runtime_error {
public:
    invalid_argument_error() : std::runtime_error("") {}
};

class graph_state_error : public std::runtime_error {
public:
    graph_state_error() : std::runtime_error("") {}
};

class qml_error : public std::runtime_error {
public:
    qml_error() : std::runtime_error("") {}
};

class qml_missing_element_error : public std::runtime_error {
public:
    qml_missing_element_error() : std::runtime_error("") {}
};

class qml_connect_error : public std::runtime_error {
public:
    qml_connect_error() : std::runtime_error("") {}
};

class open_file_error : public std::runtime_error {
public:
    open_file_error() : std::runtime_error("") {}
};

class invalid_json_error : public std::runtime_error {
public:
    invalid_json_error() : std::runtime_error("") {}
};

class event_error : public std::runtime_error {
public:
    event_error() : std::runtime_error("") {}
};

class actor_error : public std::runtime_error {
public:
    actor_error() : std::runtime_error("") {}
};
