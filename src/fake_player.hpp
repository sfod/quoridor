#include "iplayer.hpp"

namespace Quoridor {

class FakePlayer : public IPlayer {
public:
    FakePlayer();
    ~FakePlayer();

    IMove *get_move();
};

}  /* namespace Quoridor */
