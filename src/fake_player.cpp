#include "fake_player.hpp"

#include <ctime>

#include <boost/random/discrete_distribution.hpp>

#include "exception.hpp"
#include "walk_move.hpp"
#include "wall_move.hpp"


namespace Quoridor {

FakePlayer::FakePlayer(std::shared_ptr<Board> board,
        std::shared_ptr<Pawn> pawn)
    : board_(board), pawn_(pawn), fin_nodes_(), gen_()
{
    board_->pawn_final_nodes(pawn_, &fin_nodes_);
    gen_.seed(static_cast<unsigned int>(std::time(NULL)));
}

FakePlayer::~FakePlayer()
{
}

IMove *FakePlayer::get_move()
{
    boost::random::uniform_int_distribution<> dist_10(1, 10);

    if (dist_10(gen_) > 7) {
        size_t min_path_len = 81;
        std::list<int> path;
        for (auto end_node : fin_nodes_) {
            std::list<int> nodes;
            if (board_->get_path(pawn_, end_node, &nodes)) {
                if (min_path_len > nodes.size()) {
                    min_path_len = nodes.size();
                    path = nodes;
                }
            }
        }

        if (path.size() == 0) {
            throw Exception("all pathes are blocked");
        }

        auto node_it = path.begin();
        int next_node = *node_it;
        if (board_->is_occupied(next_node)) {
            // @todo
        }
        return new WalkMove(next_node);
    }
    else {
        boost::random::uniform_int_distribution<> dist_2(0, 1);
        boost::random::uniform_int_distribution<> dist_9(0, 8);
        boost::random::uniform_int_distribution<> dist_8(0, 7);
        return new WallMove(Wall(dist_2(gen_), dist_9(gen_), dist_8(gen_), 2));
    }
}

}  /* namespace Quoridor */
