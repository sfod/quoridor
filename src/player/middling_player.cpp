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

double MiddlingPlayer::get_max_move(const Game &game, int lvl)
{
    double best_val = -100;

    std::set<Node> moves;
    game.possible_moves(pawn_, &moves, NULL);
    for (auto node : moves) {
        Game game = *game_;
        game.move_pawn(node);
        if (game.is_finished()) {
            return 1.0f;
        }
        else if (lvl < kMaxLevel) {
            game.switch_pawn();
            best_val = std::max(best_val, get_min_move(game, lvl + 1));
        }
        else {
            return evaluate(game);
        }
    }

    return best_val;
}

double MiddlingPlayer::get_min_move(const Game &game, int lvl)
{
    double best_val = 100;

    std::set<Node> moves;
    game.possible_moves(pawn_, &moves, NULL);
    for (auto node : moves) {
        Game game = *game_;
        game.move_pawn(node);
        if (game.is_finished()) {
            return -1.0f;
        }
        else if (lvl < kMaxLevel) {
            game.switch_pawn();
            best_val = std::min(best_val, get_max_move(game, lvl + 1));
        }
        else {
            return evaluate(game);
        }
    }

    return best_val;
}

double MiddlingPlayer::evaluate(const Game &game) const
{
    double max_k = 0;

    for (auto goal_node : goal_nodes_) {
        std::list<Node> path;
        game.get_path(pawn_, goal_node, &path);
        double k = 1 / static_cast<double>(path.size());
        max_k = std::max(k, max_k);
    }

    double rival_max_k = 0;
    for (auto pawn_data : game.pawn_data_list()) {
        if (pawn_data.pawn == pawn_) {
            continue;
        }
        for (auto goal_node : pawn_data.goal_nodes) {
            std::list<Node> path;
            game.get_path(pawn_data.pawn, goal_node, &path);
            double k = 1 / static_cast<double>(path.size());
            rival_max_k = std::max(k, rival_max_k);
        }
    }

    return max_k - rival_max_k;
}

}  /* namespace Quoridor */
