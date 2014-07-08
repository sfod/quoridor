#include "mock_player.hpp"

#include <ctime>

#include <boost/random/discrete_distribution.hpp>

#include "logger.hpp"
#include "walk_move.hpp"
#include "wall_move.hpp"
#include "exception.hpp"

static boost::log::sources::severity_logger<boost::log::trivial::severity_level> lg;

namespace Quoridor {

MockPlayer::MockPlayer(std::shared_ptr<Game> game,
        std::shared_ptr<Pawn> pawn)
    : game_(game), pawn_(pawn), goal_nodes_(), moves_()
{
    lg.add_attribute("Tag", blattrs::constant<std::string>("mock player"));

    goal_nodes_ = game_->pawn_data(pawn_).goal_nodes;

    BOOST_LOG_DEBUG(lg) << "created new MockPlayer (" << pawn_->color() << ")";
    for (auto node : goal_nodes_) {
        BOOST_LOG_DEBUG(lg) << "goal node: " << node.row() << ":" << node.col();
    }
}

MockPlayer::~MockPlayer()
{
}

IMove *MockPlayer::get_move()
{
    if (moves_.size() == 0) {
        throw Exception("no more moves");
    }
    IMove *move = moves_.front();
    moves_.pop_front();
    return move;
}

void MockPlayer::push_move(IMove *move)
{
    moves_.push_back(move);
}

}  /* namespace Quoridor */
