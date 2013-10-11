#include "game.hpp"
#include "exception.hpp"

namespace Quoridor {

Game::Game(std::shared_ptr<Quoridor::Board> board)
    : board_(board), pawn_list_()
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

int Game::make_move(BoardMoves move, std::shared_ptr<Pawn> pawn)
{
    return board_->make_move(move, pawn);
}

bool Game::is_win(std::shared_ptr<Pawn> pawn) const
{
    return board_->is_at_opposite_side(pawn);
}

}  /* namespace Quoridor */
