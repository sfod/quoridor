#include "game.hpp"

#include <iostream>
#include "exception.hpp"

namespace Quoridor {

Game::Game(Board *board) : board_(board), player_list_()
{
}

Game::~Game()
{
}

void Game::main_loop()
{
    while (true) {
        for (auto *player: player_list_) {
            make_move(player);
            if (is_win(player)) {
                std::cout << player->name() << " win" << std::endl;
            }
        }
    }
}

void Game::add_player(Player *player)
{
    int board_side = board_->next_side();
    player->set_board_side(board_side);
    pos_t pos;

    switch (board_side) {
    case 0:
        pos.first = 0;
        pos.second = 4;
        break;
    case 1:
        pos.first = 4;
        pos.second = 0;
        break;
    case 2:
        pos.first = 8;
        pos.second = 4;
        break;
    case 3:
        pos.first = 4;
        pos.second = 8;
        break;
    default:
        throw Exception();
    }
    player->set_pos(pos);
    board_->add_occupied(pos);
    player_list_.push_back(player);
}

void Game::make_move(Player *player)
{
    int move;
    while (true) {
        std::cout << "make move ";
        std::cin >> move;
        if ((move >= 0) && (move <= 4)) {
            break;
        }
    }

    pos_t fin_pos;
    move = (move + player->board_side()) % 4;
    if (board_->make_move(move, player->pos(), &fin_pos) == 0) {
        std::cout << player->name() << ": ("
            << player->pos().first << "," << player->pos().second << " => ("
            << fin_pos.first << "," << fin_pos.second << ")" << std::endl;

        board_->rm_occupied(player->pos());
        player->set_pos(fin_pos);
        board_->add_occupied(fin_pos);
    }
}

bool Game::is_win(const Player *player) const
{
    return board_->is_at_opposite_side(player->board_side(), player->pos());
}

}  /* namespace Quoridor */
