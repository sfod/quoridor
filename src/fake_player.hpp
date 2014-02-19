#include "iplayer.hpp"

namespace Quoridor {

class FakePlayer : public IPlayer {
public:
    FakePlayer();
    virtual ~FakePlayer();

    IMove *get_move();
    virtual bool is_interactive() const { return false; };
};

}  /* namespace Quoridor */
