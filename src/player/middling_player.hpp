#ifndef QUORIDOR_MIDDLING_PLAYER_HPP_
#define QUORIDOR_MIDDLING_PLAYER_HPP_

#include <memory>
#include <set>

#include "iplayer.hpp"
#include "game.hpp"
#include "pawn.hpp"

namespace Quoridor {

class MoveVisitor : public boost::static_visitor<double> {
public:
    double operator()(const Node &node, std::shared_ptr<Pawn> pawn,
            const Game &game, int depth, double a, double b) const;
    double operator()(const Wall &wall, std::shared_ptr<Pawn> pawn,
            const Game &game, int depth, double a, double b) const;

private:
    double evaluate() const;
};

class MiddlingPlayer : public IPlayer {
public:
    MiddlingPlayer(std::shared_ptr<Game> game, std::shared_ptr<Pawn> pawn);
    virtual ~MiddlingPlayer();

    virtual move_t get_move();
    virtual bool is_interactive() const { return false; };

private:
    double get_max_move(const Game &game, int depth, double a, double b,
            move_t *best_move);
    double get_min_move(const Game &game, int depth, double a, double b);
    double evaluate(const Game &game) const;

private:
    std::shared_ptr<Game> game_;
    std::shared_ptr<Pawn> pawn_;
    std::set<Node> goal_nodes_;
};

}  /* namespace Quoridor */

#endif  // QUORIDOR_MIDDLING_PLAYER_HPP_
