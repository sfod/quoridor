#ifndef QUORIDOR_MIDDLING_PLAYER_HPP_
#define QUORIDOR_MIDDLING_PLAYER_HPP_

#include <memory>
#include <set>

#include "iplayer.hpp"
#include "game.hpp"
#include "pawn.hpp"

namespace Quoridor {

struct move_val_t {
    Node n;
    double v;
};

class MiddlingPlayer : public IPlayer {
public:
    MiddlingPlayer(std::shared_ptr<Game> game, std::shared_ptr<Pawn> pawn);
    virtual ~MiddlingPlayer();

    virtual IMove *get_move();
    virtual bool is_interactive() const { return false; };

private:
    move_val_t get_max_move(const Game &game, int lvl);
    move_val_t get_min_move(const Game &game, int lvl);
    double evaluate(const Game &game) const;

private:
    std::shared_ptr<Game> game_;
    std::shared_ptr<Pawn> pawn_;
    std::set<Node> goal_nodes_;
};

}  /* namespace Quoridor */

#endif  // QUORIDOR_MIDDLING_PLAYER_HPP_
