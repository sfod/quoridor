#include "iplayer.hpp"

namespace Quoridor {

class Player : public IPlayer {
public:
    Player();
    ~Player();

    virtual IMove *get_move();
};

}  /* namespace Quoridor */
