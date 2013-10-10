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

void Game::make_move(std::shared_ptr<Pawn> pawn)
{
    BoardMoves move;
    while (true) {
        std::cout << "make move ";
        // std::cin >> move;
        move = kForward;
        if ((move >= 0) && (move <= 4)) {
            break;
        }
    }

    pos_t start_pos = board_->pawn_pos(pawn);
    board_->make_move(move, pawn);
    pos_t end_pos = board_->pawn_pos(pawn);

    std::cout << pawn->color()
        << ": (" << start_pos.first << "," << start_pos.second <<
        ") => (" << end_pos.first << "," << end_pos.second << ")" << std::endl;
}

bool Game::is_win(std::shared_ptr<Pawn> pawn) const
{
    return board_->is_at_opposite_side(pawn);
}

}  /* namespace Quoridor */
