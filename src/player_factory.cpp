#include "exception.hpp"
#include "player_factory.hpp"
#include "player.hpp"
#include "fake_player.hpp"

namespace Quoridor {

PlayerFactory::PlayerFactory()
{
}

PlayerFactory::~PlayerFactory()
{
}

std::shared_ptr<IPlayer> PlayerFactory::make_player(const std::string &type)
{
    if (type == "fake") {
        return std::shared_ptr<IPlayer>(new FakePlayer());
    }
    else if (type == "human") {
        return std::shared_ptr<IPlayer>(new Player());
    }
    else {
	throw Exception("Invalid Player type");
    }
}

}  /* namespace Quoridor */
