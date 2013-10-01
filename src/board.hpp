#ifndef QUORIDOR_BOARD_HPP_
#define QUORIDOR_BOARD_HPP_

#include <cstdlib>

#include <map>
#include <memory>
#include <utility>
#include <vector>

#include "player.hpp"


typedef std::pair<int, int> pos_t;

namespace Quoridor {

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

private:
    int row_num_;
    int col_num_;
    std::map<pos_t, std::shared_ptr<Player>> occ_fields_;
    std::map<std::shared_ptr<Player>, pos_t> player_pos_;
    std::vector<std::pair<int, int>> sides_;
    std::map<std::shared_ptr<Player>, int> player_sides_;
};

}  /* namespace Quoridor */

#endif  /* QUORIDOR_BOARD_HPP_ */
