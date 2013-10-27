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

std::shared_ptr<IPlayer> PlayerFactory::make_player(int type)
{
    switch (type) {
    case 1:
        return std::shared_ptr<IPlayer>(new FakePlayer());
    case 0:
    default:
        return std::shared_ptr<IPlayer>(new Player());
    }
}

}  /* namespace Quoridor */
