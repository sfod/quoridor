#ifndef QUORIDOR_MOCK_PLAYER_HPP_
#define QUORIDOR_MOCK_PLAYER_HPP_

#include <list>
#include "player/iplayer.hpp"

namespace Quoridor {

class MockPlayer : public IPlayer {
public:
    MockPlayer();
    virtual ~MockPlayer();

    virtual move_t get_move();
    virtual bool is_interactive() const { return false; };

    void push_move(move_t move);

private:
    std::list<move_t> moves_;
};

}  /* namespace Quoridor */

#endif  // QUORIDOR_MOCK_PLAYER_HPP_
