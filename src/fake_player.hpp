#include "iplayer.hpp"

namespace Quoridor {

class FakePlayer : public IPlayer {
public:
    FakePlayer();
    virtual ~FakePlayer();

    IMove *get_move();
};

}  /* namespace Quoridor */
