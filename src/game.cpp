#include "game.hpp"
#include "exception.hpp"
#include "walk_move.hpp"
#include "wall_move.hpp"

namespace Quoridor {

Game::Game() : board_(new Board(9, 9)), pawn_list_()
{
}

Game::~Game()
{
}

void Game::add_pawn(std::shared_ptr<Pawn> pawn)
{
    board_->add_pawn(pawn);
    pawn_list_.push_back(pawn);
}

pos_t Game::pawn_pos(std::shared_ptr<Pawn> pawn) const
{
    return board_->pawn_pos(pawn);
}

int Game::make_move(IMove *move, std::shared_ptr<Pawn> pawn)
{
    if (WalkMove *walk_move = dynamic_cast<WalkMove *>(move)) {
        return board_->make_walking_move(pawn, walk_move->node());
    }
    else if (WallMove *wall_move = dynamic_cast<WallMove *>(move)) {
        return board_->add_wall(wall_move->wall());
    }
    else {
        return -1;
    }
}

bool Game::is_win(std::shared_ptr<Pawn> pawn) const
{
    return board_->is_at_opposite_side(pawn);
}

}  /* namespace Quoridor */
