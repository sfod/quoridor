#include "middling_player.hpp"

#include <ctime>

#include <boost/random/discrete_distribution.hpp>

#include "logger.hpp"
#include "walk_move.hpp"
#include "wall_move.hpp"
#include "exception.hpp"

static boost::log::sources::severity_logger<boost::log::trivial::severity_level> lg;

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
    Node n;

    std::set<Node> moves;
    game_->possible_moves(pawn_, &moves, NULL);
    for (auto node : moves) {
        Game game = *game_;
        game.move_pawn(node);
        n = node;
    }

    return new WalkMove(n);
}

void MiddlingPlayer::get_max_move()
{
}

void MiddlingPlayer::get_min_move()
{
}

double MiddlingPlayer::evaluate() const
{
    return 0;
}

}  /* namespace Quoridor */
