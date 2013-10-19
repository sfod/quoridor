#include "iplayer.hpp"

namespace Quoridor {

class Player : public IPlayer {
public:
    Player();
    ~Player();

    int get_move(BoardMoves *move);
};

}  /* namespace Quoridor */
