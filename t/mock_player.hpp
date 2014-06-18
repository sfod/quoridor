#ifndef QUORIDOR_MOCK_PLAYER_HPP_
#define QUORIDOR_MOCK_PLAYER_HPP_

#include <memory>
#include <set>

#include "player/iplayer.hpp"
#include "game.hpp"
#include "pawn.hpp"


namespace Quoridor {

class MockPlayer : public IPlayer {
public:
    MockPlayer(std::shared_ptr<Game> game, std::shared_ptr<Pawn> pawn);
    virtual ~MockPlayer();

    virtual IMove *get_move();
    virtual bool is_interactive() const { return false; };

    void push_move(IMove *move);

private:
    std::shared_ptr<Game> game_;
    std::shared_ptr<Pawn> pawn_;
    std::set<Node> goal_nodes_;
    std::list<IMove*> moves_;
};

}  /* namespace Quoridor */

#endif  // QUORIDOR_MOCK_PLAYER_HPP_
