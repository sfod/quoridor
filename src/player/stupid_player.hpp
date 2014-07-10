#ifndef QUORIDOR_STUPID_PLAYER_HPP_
#define QUORIDOR_STUPID_PLAYER_HPP_

#include <memory>
#include <set>

#include <boost/random/mersenne_twister.hpp>

#include "iplayer.hpp"
#include "game.hpp"
#include "pawn.hpp"

namespace Quoridor {

class StupidPlayer : public IPlayer {
public:
    StupidPlayer(std::shared_ptr<Game> game, std::shared_ptr<Pawn> pawn);
    virtual ~StupidPlayer();

    virtual move_t get_move();
    virtual bool is_interactive() const { return false; };

private:
    std::shared_ptr<Game> game_;
    std::shared_ptr<Pawn> pawn_;
    std::set<Node> goal_nodes_;
    boost::random::mt19937 gen_;
};

}  /* namespace Quoridor */

#endif  // QUORIDOR_STUPID_PLAYER_HPP_
