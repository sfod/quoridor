#include "middling_player.hpp"

#include <ctime>

#include <boost/random/discrete_distribution.hpp>

#include "logger.hpp"
#include "walk_move.hpp"
#include "wall_move.hpp"
#include "exception.hpp"

static boost::log::sources::severity_logger<boost::log::trivial::severity_level> lg;
static int kMaxLevel = 3;

namespace Quoridor {

MiddlingPlayer::MiddlingPlayer(std::shared_ptr<Game> game,
        std::shared_ptr<Pawn> pawn)
    : game_(game), pawn_(pawn), goal_nodes_()
{
    lg.add_attribute("Tag", blattrs::constant<std::string>("middling player"));

    goal_nodes_ = game_->pawn_data(pawn_).goal_nodes;

    BOOST_LOG_DEBUG(lg) << "created new MiddlingPlayer (" << pawn_->color() << ")";
    for (auto node : goal_nodes_) {
        BOOST_LOG_DEBUG(lg) << "goal node: " << node.row() << ":" << node.col();
    }
}

MiddlingPlayer::~MiddlingPlayer()
{
}

IMove *MiddlingPlayer::get_move()
{
    double v = get_max_move(*game_, 0);
    BOOST_LOG_DEBUG(lg) << "max move val is " << v;
    IMove *move;
    for (auto goal_node : goal_nodes_) {
        std::list<Node> path;
        game_->get_path(pawn_, goal_node, &path);
        auto node_it = path.begin();
        Node next_node = *node_it;
        move = new WalkMove(next_node);
    }
    return move;
}

move_val_t MiddlingPlayer::get_max_move(const Game &game, int lvl)
{
    move_val_t mv;
    mv.v = -1000;

    std::set<Node> moves;
    game.possible_moves(pawn_, &moves, NULL);
    for (auto node : moves) {
        Game game = *game_;
        mv.n = node;
        game.switch_pawn();
        game.move_pawn(node);
        if (lvl < kMaxLevel) {
            mv.v = std::max(mv.v, get_min_move(game, lvl + 1).v);
        }
        else {
            return evaluate(game);
        }
    }

    return mv;
}

move_val_t MiddlingPlayer::get_min_move(const Game &game, int lvl)
{
    double best_val = 1000;

    std::set<Node> moves;
    game.possible_moves(pawn_, &moves, NULL);
    for (auto node : moves) {
        Game game = *game_;
        game.switch_pawn();
        game.move_pawn(node);
        evaluate(game);
        if (lvl < kMaxLevel) {
            best_val = std::min(best_val, get_max_move(game, lvl + 1));
        }
        else {
            return -evaluate(game);
        }
    }

    return best_val;
}

double MiddlingPlayer::evaluate(const Game &game) const
{
    double val = 73;

    for (auto goal_node : goal_nodes_) {
        std::list<Node> path;
        game.get_path(game.cur_pawn_data().pawn, goal_node, &path);
        double k = 1 / static_cast<double>(path.size());
        if (k < val) {
            val = k;
        }
    }
    return val;
}

}  /* namespace Quoridor */
