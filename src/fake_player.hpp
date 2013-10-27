#include "iplayer.hpp"

namespace Quoridor {

class FakePlayer : public IPlayer {
public:
    FakePlayer();
    ~FakePlayer();

    int get_move(BoardMoves *move);
};

}  /* namespace Quoridor */
