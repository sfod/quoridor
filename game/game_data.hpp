#pragma once

#include <map>
#include <string>

enum class PlayerType {
    PT_Human,
    PT_AI,
    PT_Invalid
};

static const std::map<std::string, PlayerType> str_to_player_type = {
    {"human", PlayerType::PT_Human},
    {"AI", PlayerType::PT_AI}
};

static const std::map<PlayerType, std::string> player_type_to_str = {
    {PlayerType::PT_Human, "human"},
    {PlayerType::PT_AI, "AI"}
};
