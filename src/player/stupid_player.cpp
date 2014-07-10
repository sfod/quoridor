#include "stupid_player.hpp"

#include <ctime>

#include <boost/random/discrete_distribution.hpp>

#include "logger.hpp"
#include "exception.hpp"

static boost::log::sources::severity_logger<boost::log::trivial::severity_level> lg;

namespace Quoridor {

StupidPlayer::StupidPlayer(std::shared_ptr<Game> game,
        std::shared_ptr<Pawn> pawn)
    : game_(game), pawn_(pawn), goal_nodes_(), gen_()
{
    lg.add_attribute("Tag", blattrs::constant<std::string>("stupid player"));

    goal_nodes_ = game_->pawn_data(pawn_).goal_nodes;
    gen_.seed(static_cast<unsigned int>(std::time(NULL)));

    BOOST_LOG_DEBUG(lg) << "new StupidPlayer (" << pawn_->color() << ")";
    for (auto node : goal_nodes_) {
        BOOST_LOG_DEBUG(lg) << "goal node: " << node;
    }
}

StupidPlayer::~StupidPlayer()
{
}

move_t StupidPlayer::get_move()
{
    boost::random::discrete_distribution<> dist{0.6, 0.6};

    if (dist(gen_) == 0) {
        std::list<Node> path;
        size_t path_len = game_->shortest_path(game_->pawn_data(pawn_).node,
                goal_nodes_, &path);
        if (path_len == 0) {
            throw Exception("all pathes are blocked");
        }
        return *path.begin();
    }
    else {
        boost::random::discrete_distribution<> dist_2{8, 2};
        boost::random::uniform_int_distribution<> dist_9(0, 8);
        boost::random::discrete_distribution<> dist_8{1, 1, 2, 3, 3, 2, 1, 1};
        Wall::Orientation wall_orient =
            (dist_2(gen_) == 0) ? Wall::kHorizontal : Wall::kVertical;
        return Wall(wall_orient, dist_9(gen_), dist_8(gen_), 2);
    }
}

}  /* namespace Quoridor */
