#include "fake_player.hpp"

#include <ctime>

#include <boost/random/discrete_distribution.hpp>

#include "walk_move.hpp"
#include "wall_move.hpp"
#include "exception.hpp"

namespace Quoridor {

FakePlayer::FakePlayer(std::shared_ptr<Game> game,
        std::shared_ptr<Pawn> pawn)
    : game_(game), pawn_(pawn), goal_nodes_(), gen_()
{
    goal_nodes_ = game_->pawn_data(pawn_).goal_nodes;
    gen_.seed(static_cast<unsigned int>(std::time(NULL)));
}

FakePlayer::~FakePlayer()
{
}

IMove *FakePlayer::get_move()
{
    boost::random::discrete_distribution<> dist{0.6, 0.6};

    if (dist(gen_) == 0) {
        size_t min_path_len = 81;
        std::list<Node> min_path;
        for (auto end_node : goal_nodes_) {
            std::list<Node> path;
            if (game_->get_path(pawn_, end_node, &path)) {
                if (min_path_len > path.size()) {
                    min_path_len = path.size();
                    min_path = path;
                }
            }
        }

        if (min_path.size() == 0) {
            throw Exception("all pathes are blocked");
        }

        auto node_it = min_path.begin();
        Node next_node = *node_it;
        return new WalkMove(next_node);
    }
    else {
        boost::random::discrete_distribution<> dist_2{8, 2};
        boost::random::uniform_int_distribution<> dist_9(0, 8);
        boost::random::discrete_distribution<> dist_8{1, 1, 2, 3, 3, 2, 1, 1};
        Wall::Orientation wall_orient = dist_2(gen_) == 0 ? Wall::kHorizontal : Wall::kVertical;
        return new WallMove(Wall(wall_orient, dist_9(gen_), dist_8(gen_), 2));
    }
}

}  /* namespace Quoridor */
