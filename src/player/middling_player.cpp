#include "middling_player.hpp"

#include <ctime>

#include <boost/random/discrete_distribution.hpp>

#include "logger.hpp"
#include "walk_move.hpp"
#include "wall_move.hpp"
#include "exception.hpp"

static boost::log::sources::severity_logger<boost::log::trivial::severity_level> lg;
static int kLookForward = 1;
static int kMinimaxNodes = 0;

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
    IMove *move = NULL;
    kMinimaxNodes = 0;
    double v = get_max_move(*game_, 0, -100, 100, &move);
    BOOST_LOG_DEBUG(lg) << "got k " << v << " (analyzed " << kMinimaxNodes
        << " nodes)";
    if (WalkMove *m = dynamic_cast<WalkMove*>(move)) {
        BOOST_LOG_DEBUG(lg) << "best move is " << m->node().row() << ":"
            << m->node().col();
    }
    else if (WallMove *m = dynamic_cast<WallMove*>(move)) {
        BOOST_LOG_DEBUG(lg) << "best move is " << m->wall();
    }
    return move;
}

double MiddlingPlayer::get_max_move(const Game &game, int depth,
        double a, double b, IMove **best_move)
{
    ++kMinimaxNodes;

    std::vector<IMove*> moves;
    game.possible_moves(pawn_, &moves);
    for (auto move : moves) {
        Game game_cp = game;
        if (WalkMove *m = dynamic_cast<WalkMove*>(move)) {
            game_cp.move_pawn(m->node());
            if (game_cp.is_finished()) {
                if (best_move != NULL) {
                    *best_move = new WalkMove(*m);
                }
                a = 1.0f;
                return a;
            }
        }
        else if (WallMove *m = dynamic_cast<WallMove*>(move)) {
            if (game_cp.add_wall(m->wall()) < 0) {
                continue;
            }
        }

        if (depth < kLookForward) {
            game_cp.switch_pawn();
            double val = get_min_move(game_cp, depth + 1, a, b);
            if (val > a) {
                a = val;
                if (best_move != NULL) {
                    if (WalkMove *m = dynamic_cast<WalkMove*>(move)) {
                        *best_move = new WalkMove(*m);
                    }
                    else if (WallMove *m = dynamic_cast<WallMove*>(move)) {
                        *best_move = new WallMove(*m);
                    }
                }
            }
            if (b <= a) {
                return a;
            }
        }
        else {
            return evaluate(game_cp);
        }
    }

    return a;
}

double MiddlingPlayer::get_min_move(const Game &game, int depth,
        double a, double b)
{
    ++kMinimaxNodes;

    std::vector<IMove*> moves;
    game.possible_moves(pawn_, &moves);
    for (auto move : moves) {
        Game game_cp = game;
        if (WalkMove *m = dynamic_cast<WalkMove*>(move)) {
            game_cp.move_pawn(m->node());
            if (game_cp.is_finished()) {
                b = -1.0f;
                return b;
            }
        }
        else if (WallMove *m = dynamic_cast<WallMove*>(move)) {
            if (game_cp.add_wall(m->wall()) < 0) {
                continue;
            }
        }

        if (depth < kLookForward) {
            game_cp.switch_pawn();
            b = std::min(b, get_max_move(game_cp, depth + 1, a, b, NULL));
            if (b <= a) {
                return b;
            }
        }
        else {
            return evaluate(game_cp);
        }
    }

    return b;
}

double MiddlingPlayer::evaluate(const Game &game) const
{
    double max_k = 0;

    for (auto goal_node : goal_nodes_) {
        std::list<Node> path;
        if (game.get_path(pawn_, goal_node, &path)) {
            double k = 1 / static_cast<double>(path.size());
            max_k = std::max(k, max_k);
        }
    }

    double rival_max_k = 0;
    for (auto pawn_data : game.pawn_data_list()) {
        if (pawn_data.pawn == pawn_) {
            continue;
        }
        for (auto goal_node : pawn_data.goal_nodes) {
            std::list<Node> path;
            if (game.get_path(pawn_data.pawn, goal_node, &path)) {
                double k = 1 / static_cast<double>(path.size());
                rival_max_k = std::max(k, rival_max_k);
            }
        }
    }

    return max_k - rival_max_k;
}

}  /* namespace Quoridor */
