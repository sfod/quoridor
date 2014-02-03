#ifndef QUORIDOR_PLAYER_FACTORY_HPP_
#define QUORIDOR_PLAYER_FACTORY_HPP_

#include <memory>
#include "iplayer.hpp"

namespace Quoridor {

class PlayerFactory {
public:
    PlayerFactory();
    ~PlayerFactory();

    std::shared_ptr<IPlayer> make_player(const std::string &type);
};

}  /* namespace Quoridor */

#endif  /* QUORIDOR_PLAYER_FACTORY_HPP_ */
