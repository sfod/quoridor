#include "exception.hpp"
#include "player_factory.hpp"
#include "player.hpp"
#include "fake_player.hpp"

namespace Quoridor {

static std::vector<std::string> player_types = {"fake", "human"};

PlayerFactory::PlayerFactory()
{
}

PlayerFactory::~PlayerFactory()
{
}

std::shared_ptr<IPlayer> PlayerFactory::make_player(const std::string &type,
            std::shared_ptr<Game> game, std::shared_ptr<Pawn> pawn)
{
    if (type == "fake") {
        return std::shared_ptr<IPlayer>(new FakePlayer(game, pawn));
    }
    else if (type == "human") {
        return std::shared_ptr<IPlayer>(new Player(game, pawn));
    }
    else {
	throw Exception("Invalid Player type");
    }
}

const std::vector<std::string> &PlayerFactory::types()
{
    return player_types;
}

}  /* namespace Quoridor */
