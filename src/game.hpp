#ifndef QUORIDOR_GAME_HPP_
#define QUORIDOR_GAME_HPP_

#include <memory>
#include <utility>
#include <vector>

#include "board.hpp"
#include "pawn.hpp"
#include "imove.hpp"

namespace Quoridor {

class Game {
public:
    Game();
    virtual ~Game();

    void add_pawn(std::shared_ptr<Pawn> pawn);
    pos_t pawn_pos(std::shared_ptr<Pawn> pawn) const;

    int make_move(IMove *move, std::shared_ptr<Pawn> pawn);
    bool is_win(std::shared_ptr<Pawn> pawn) const;

    std::vector<std::shared_ptr<Pawn>> pawn_list() const { return pawn_list_; }

private:
    std::shared_ptr<Quoridor::Board> board_;
    std::vector<std::shared_ptr<Pawn>> pawn_list_;
};

}  /* namespace Quoridor */

#endif  /* QUORIDOR_GAME_HPP_ */
