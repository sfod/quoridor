#ifndef QUORIDOR_PLAYER_FACTORY_HPP_
#define QUORIDOR_PLAYER_FACTORY_HPP_

#include <memory>
#include "iplayer.hpp"
#include "board.hpp"
#include "pawn.hpp"

namespace Quoridor {

class PlayerFactory {
public:
    PlayerFactory();
    ~PlayerFactory();

    std::shared_ptr<IPlayer> make_player(const std::string &type,
            std::shared_ptr<Board> board, std::shared_ptr<Pawn> pawn);
};

}  /* namespace Quoridor */

#endif  /* QUORIDOR_PLAYER_FACTORY_HPP_ */
