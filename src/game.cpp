#include "game.hpp"
#include <unistd.h>
#include <iostream>
#include "exception.hpp"

namespace Quoridor {

Game::Game(std::shared_ptr<Quoridor::Board> board)
    : board_(board), player_list_()
{
}

Game::~Game()
{
}

void Game::main_loop()
{
    bool is_run = true;
    while (is_run) {
        for (auto player: player_list_) {
            make_move(player);
            if (is_win(player)) {
                std::cout << player->name() << " win" << std::endl;
                is_run = false;
                break;
            }
        }
        usleep(500000);
    }
}

void Game::add_player(std::shared_ptr<Player> player)
{
    board_->add_player(player);
    player_list_.push_back(player);
}

void Game::make_move(std::shared_ptr<Player> player)
{
    int move;
    while (true) {
        std::cout << "make move ";
        // std::cin >> move;
        move = 0;
        if ((move >= 0) && (move <= 4)) {
            break;
        }
    }

    pos_t start_pos = board_->player_pos(player);
    board_->make_move(move, player);
    pos_t end_pos = board_->player_pos(player);

    std::cout << player->name()
        << ": (" << start_pos.first << "," << start_pos.second <<
        ") => (" << end_pos.first << "," << end_pos.second << ")" << std::endl;
}

bool Game::is_win(std::shared_ptr<Player> player) const
{
    return board_->is_at_opposite_side(player);
}

}  /* namespace Quoridor */
