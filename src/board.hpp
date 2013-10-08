#ifndef QUORIDOR_BOARD_HPP_
#define QUORIDOR_BOARD_HPP_

#include <cstdlib>

#include <map>
#include <memory>
#include <utility>
#include <vector>

#include "player.hpp"


namespace Quoridor {

typedef std::pair<int, int> pos_t;

class Wall {
public:
    Wall(int orientation, int line, int start_pos, int cnt);
    virtual ~Wall();

    int orientation() const { return orientation_; }
    int line() const { return line_; }
    int start_pos() const { return start_pos_; }
    int cnt() const { return cnt_; }

private:
    int orientation_;
    int line_;
    int start_pos_;
    int cnt_;
};

class Board {
public:
    Board(int row_num, int col_num);
    virtual ~Board();

    void set_size(int row_num, int col_num);
    int row_num() const { return row_num_; }
    int col_num() const { return col_num_; }
    int next_side();

    int add_player(std::shared_ptr<Player> player);
    void add_occupied(const pos_t &pos, std::shared_ptr<Player> player);
    void rm_occupied(const pos_t &pos);
    pos_t player_pos(std::shared_ptr<Player> player);

    int make_move(int move, std::shared_ptr<Player> player);
    bool is_at_opposite_side(std::shared_ptr<Player> player);

    int add_wall(const Wall &wall);
    bool wall_intersects(const Wall &wall);

private:
    int row_num_;
    int col_num_;
    std::map<pos_t, std::shared_ptr<Player>> occ_fields_;
    std::map<std::shared_ptr<Player>, pos_t> player_pos_;
    std::vector<std::pair<int, int>> sides_;
    std::map<std::shared_ptr<Player>, int> player_sides_;
    std::map<int, std::map<int, Wall>> walls_;
};

}  /* namespace Quoridor */

#endif  /* QUORIDOR_BOARD_HPP_ */
