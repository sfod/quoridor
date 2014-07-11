#include "middling_player.hpp"

#include <ctime>
#include <limits>
#include <thread>

#include "logger.hpp"
#include "exception.hpp"

static boost::log::sources::severity_logger<boost::log::trivial::severity_level> lg;
static int kLookForward = 2;
static int kMinimaxNodes = 0;

namespace Quoridor {

MiddlingPlayer::MiddlingPlayer(std::shared_ptr<Game> game,
        std::shared_ptr<Pawn> pawn)
    : game_(game), pawn_(pawn), goal_nodes_(), interrupted_(false)
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

void MiddlingPlayer::get_move(std::function<void(move_t)> callback)
{
    std::thread(&MiddlingPlayer::calc_move, this, callback).detach();
}

void MiddlingPlayer::interrupt()
{
    interrupted_ = true;
}

void MiddlingPlayer::calc_move(std::function<void(move_t)> callback)
{
    move_t move;
    kMinimaxNodes = 0;
    Game game = *game_;
    double v = get_max_move(game, 0,
            -std::numeric_limits<double>::infinity(),
            std::numeric_limits<double>::infinity(), &move);

    if (!interrupted_) {
        BOOST_LOG_DEBUG(lg) << "best move: " << move << " (k " << v
            << ", analyzed " << kMinimaxNodes << " nodes)";
        callback(move);
    }
}

double MiddlingPlayer::get_max_move(const Game &game, int depth,
        double a, double b, move_t *best_move)
{
    ++kMinimaxNodes;

    std::vector<move_t> moves = game.possible_moves(pawn_);
    for (auto move : moves) {
        Game game_cp = game;
        if (Node *node = boost::get<Node>(&move)) {
            game_cp.move_pawn(*node);
            if (game_cp.is_finished()) {
                if (best_move != NULL) {
                    *best_move = *node;
                }
                return std::numeric_limits<double>::infinity();
            }
        }
        else if (Wall *wall = boost::get<Wall>(&move)) {
            if (game_cp.add_wall(*wall) < 0) {
                continue;
            }
        }

        if (depth < kLookForward) {
            game_cp.switch_pawn();
            double val = get_min_move(game_cp, depth + 1, a, b);
            if (val > a) {
                a = val;
                if (best_move != NULL) {
                    if (Node *node = boost::get<Node>(&move)) {
                        *best_move = *node;
                    }
                    else if (Wall *wall = boost::get<Wall>(&move)) {
                        *best_move = *wall;
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

    std::vector<move_t> moves = game.possible_moves(pawn_);
    for (auto move : moves) {
        Game game_cp = game;
        if (Node *node = boost::get<Node>(&move)) {
            game_cp.move_pawn(*node);
            if (game_cp.is_finished()) {
                return -std::numeric_limits<double>::infinity();
            }
        }
        else if (Wall *wall = boost::get<Wall>(&move)) {
            if (game_cp.add_wall(*wall) < 0) {
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
    double own_k = 0;
    double rival_k = 0;
    for (auto pawn_data : game.pawn_data_list()) {
        double &k = (pawn_data.pawn == pawn_) ? own_k : rival_k;
        size_t len = game.shortest_path(pawn_data.node, pawn_data.goal_nodes, NULL);
        k = game.node_num() - len + pawn_data.wall_num * 2;
    }
    return own_k - rival_k;
}

}  /* namespace Quoridor */
