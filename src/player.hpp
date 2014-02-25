#include <memory>
#include "iplayer.hpp"
#include "board.hpp"
#include "pawn.hpp"

namespace Quoridor {

class Player : public IPlayer {
public:
    Player(std::shared_ptr<Board> board, std::shared_ptr<Pawn> pawn);
    ~Player();

    virtual IMove *get_move();
    virtual bool is_interactive() const { return true; }
};

}  /* namespace Quoridor */
