#include "game.hpp"
#include "exception.hpp"

namespace Quoridor {

static const std::vector<int> pawn_idx_list = {0, 2, 1, 3};

Game::Game(size_t board_size) : board_size_(board_size), pawn_data_list_(),
    cur_pawn_idx_(-1)
{
}

Game::~Game()
{
}

void Game::set_pawns(std::vector<std::shared_ptr<Pawn>> &pawn_list)
{
    size_t pawn_num = pawn_list.size();
    if ((pawn_num != 2) || (pawn_num != 4)) {
        throw Exception("Invalid number of players");
    }

    pawn_data_t pawn_data;

    for (size_t i = 0; i < pawn_list.size(); ++i) {
        pawn_data.pawn = pawn_list[i];

        int pawn_idx = pawn_idx_list[i];
        switch (pawn_idx) {
        case 0:
            pawn_data.start_node.set_row(0);
            pawn_data.start_node.set_col(board_size_ / 2);
            break;
        case 1:
            pawn_data.start_node.set_row(board_size_ / 2);
            pawn_data.start_node.set_col(0);
            break;
        case 2:
            pawn_data.start_node.set_row(board_size_ - 1);
            pawn_data.start_node.set_col(board_size_ / 2);
            break;
        case 3:
            pawn_data.start_node.set_row(board_size_ / 2);
            pawn_data.start_node.set_col(board_size_ - 1);
            break;
        }

        // @todo add goal nodes

        pawn_data_list_[pawn_idx] = pawn_data;
    }
}

std::shared_ptr<Pawn> Game::next_pawn()
{
    auto it = pawn_data_list_.upper_bound(cur_pawn_idx_);
    if (it == pawn_data_list_.end()) {
        it = pawn_data_list_.begin();
    }
    return it->second.pawn;
}

int Game::move_pawn(std::shared_ptr<Pawn> pawn, const WalkMove &move)
{
}

int Game::add_wall(std::shared_ptr<Pawn> pawn, const WallMove &move)
{
}

bool Game::is_finished() const
{
}

}  // namespace Quoridor
