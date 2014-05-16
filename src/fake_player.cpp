#include "fake_player.hpp"

#include <ctime>

#include <boost/random/discrete_distribution.hpp>

#include "walk_move.hpp"
#include "wall_move.hpp"
#include "exception.hpp"


namespace Quoridor {

FakePlayer::FakePlayer(std::shared_ptr<Board> board,
        std::shared_ptr<Pawn> pawn)
    : board_(board), pawn_(pawn), goal_nodes_(), gen_()
{
    board_->pawn_goal_nodes(pawn_, &goal_nodes_);
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
        std::list<Pos> min_path;
        for (auto end_node : goal_nodes_) {
            std::list<Pos> path;
            if (board_->get_path(pawn_, end_node, &path)) {
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
        Pos next_node = *node_it;
        if (board_->is_occupied(next_node)) {
            // @todo
        }
        return new WalkMove(next_node);
    }
    else {
        boost::random::discrete_distribution<> dist_2{8, 2};
        boost::random::uniform_int_distribution<> dist_9(0, 8);
        boost::random::discrete_distribution<> dist_8{1, 1, 2, 3, 3, 2, 1, 1};
        return new WallMove(Wall(dist_2(gen_), dist_9(gen_), dist_8(gen_), 2));
    }
}

}  /* namespace Quoridor */
