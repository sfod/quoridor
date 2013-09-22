#include "game.hpp"

#include <iostream>

namespace Quoridor {

Game::Game(const Board &board) : board_(board), player_list_()
{
}

Game::~Game()
{
}

void Game::add_player(const Player &player)
{
    player_list_.push_back(player);
}

void Game::main_loop()
{
    while (true) {
        for (auto &player: player_list_) {
            make_move(player);
        }
    }
}

void Game::make_move(Player &player)
{
    do {
        std::cout << player.name() << ": ";
    } while (player.make_move() != 0);
}

}  /* namespace Quoridor */
