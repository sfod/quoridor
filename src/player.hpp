#include "iplayer.hpp"

namespace Quoridor {

class Player : public IPlayer {
public:
    Player();
    ~Player();

    virtual IMove *get_move();
    virtual bool is_interactive() const { return true; }
};

}  /* namespace Quoridor */
