#include "board.hpp"

#include <cstring>

#include "exception.hpp"


namespace Quoridor {


Wall::Wall(int orientation, int line, int start_pos, int cnt)
    : orientation_(orientation), line_(line), start_pos_(start_pos), cnt_(cnt)
{
}

Wall::~Wall()
{
}


Board::Board(int row_num, int col_num) : row_num_(), col_num_(),
        occ_fields_(), player_pos_(), sides_(), player_sides_(), walls_()
{
    set_size(row_num, col_num);
    sides_.push_back(std::pair<int, int>(0, 0));
    sides_.push_back(std::pair<int, int>(2, 0));
    sides_.push_back(std::pair<int, int>(1, 0));
    sides_.push_back(std::pair<int, int>(3, 0));
}

Board::~Board()
{
}

void Board::set_size(int row_num, int col_num)
{
    if (row_num <= 0) {
        throw Exception();
    }
    if (col_num <= 0) {
        throw Exception();
    }
    row_num_ = row_num;
    col_num_ = col_num;
}

int Board::next_side()
{
    for (auto &side : sides_) {
        if (side.second == 0) {
            side.second = 1;
            return side.first;
        }
    }
    return -1;
}

int Board::add_player(std::shared_ptr<Player> player)
{
    player_sides_[player] = next_side();
    pos_t pos;

    switch (player_sides_[player]) {
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

    occ_fields_[pos] = player;
    player_pos_[player] = pos;

    return 0;
}

void Board::add_occupied(const pos_t &pos, std::shared_ptr<Player> player)
{
    if (occ_fields_.count(pos) > 0) {
        throw Exception();
    }
    occ_fields_[pos] = player;
    player_pos_[player] = pos;
}

void Board::rm_occupied(const pos_t &pos)
{
    occ_fields_.erase(pos);
}

pos_t Board::player_pos(std::shared_ptr<Player> player)
{
    return player_pos_[player];
}

int Board::make_move(int move, std::shared_ptr<Player> player)
{
    pos_t pos = player_pos_[player];

    move = (move + player_sides_[player]) % 4;

    switch (move) {
    case 0:
        if (pos.first == row_num() - 1) {
            return -1;
        }
        do {
            pos.first++;
        } while (occ_fields_.count(pos) > 0);
        if (pos.first > row_num() - 1) {
            return -1;
        }
        break;
    case 1:
        if (pos.second == col_num() - 1) {
            return -1;
        }
        do {
            pos.second++;
        } while (occ_fields_.count(pos) > 0);
        if (pos.second > col_num() - 1) {
            return -1;
        }
        break;
    case 2:
        if (pos.first == 0) {
            return -1;
        }
        do {
            pos.first--;
        } while (occ_fields_.count(pos) > 0);
        if (pos.first < 0) {
            return -1;
        }
        break;
    case 3:
        if (pos.second == 0) {
            return -1;
        }
        do {
            pos.second--;
        } while (occ_fields_.count(pos) > 0);
        if (pos.second < 0) {
            return -1;
        }
        break;
    }

    occ_fields_.erase(player_pos_[player]);
    player_pos_[player] = pos;
    occ_fields_[pos] = player;

    return 0;
}

bool Board::is_at_opposite_side(std::shared_ptr<Player> player)
{
    switch (player_sides_[player]) {
    case 0:
        return player_pos_[player].first == row_num() - 1;
    case 1:
        return player_pos_[player].second == col_num() - 1;
    case 2:
        return player_pos_[player].first == 0;
    case 3:
        return player_pos_[player].second == 0;
    default:
        throw Exception();
    }
}

int Board::add_wall(const Wall &wall)
{
    int line_lim = wall.orientation() ? col_num() : row_num();
    int start_pos_lim = wall.orientation() ? row_num() : col_num();
    if ((wall.line() >= line_lim)
            || (wall.start_pos() + wall.cnt() >= start_pos_lim)) {
        return -1;
    }

    if (wall_intersects(wall)) {
        return -2;
    }

    walls_[wall.orientation()].insert(std::map<int, Wall>::value_type(wall.line(), Wall(wall)));

    return 0;
}

bool Board::wall_intersects(const Wall &wall)
{
    if (walls_[1 - wall.orientation()].count(wall.start_pos() - 1) != 0) {
        return true;
    }
    if (walls_[wall.orientation()].count(wall.start_pos() - 1) != 0) {
        return true;
    }
    if (walls_[wall.orientation()].count(wall.start_pos()) != 0) {
        return true;
    }
    return false;
}

}  /* namespace Quoridor */
