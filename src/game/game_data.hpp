#pragma once

#include <map>
#include <QString>

enum class PlayerType {
    PT_Human,
    PT_AI,
    PT_Invalid
};

static const std::map<QString, PlayerType> str_to_player_type = {
    {"human", PlayerType::PT_Human},
    {"AI", PlayerType::PT_AI}
};

static const std::map<PlayerType, QString> player_type_to_str = {
    {PlayerType::PT_Human, "human"},
    {PlayerType::PT_AI, "AI"}
};
