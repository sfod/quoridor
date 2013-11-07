#ifndef QUORIDOR_UI_CONSOLE_HPP_
#define QUORIDOR_UI_CONSOLE_HPP_

#include "iplayer.hpp"
#include "irunnable.hpp"
#include "game.hpp"
#include "walk_move.hpp"

namespace Quoridor {
namespace UI {

class Console : public IRunnable {
public:
    explicit Console(int player_num);
    virtual ~Console();

    void run();
    int set_player(int i, std::shared_ptr<IPlayer> player);
    void display() const;

private:
    void init_board_repr();
    void redraw_pawn(char p, const pos_t &old_pos, const pos_t &new_pos);
    void draw_wall(const Wall &wall);
    void print_walk_move(std::shared_ptr<Pawn> pawn, const std::string &dir,
            const pos_t &old_pos, const pos_t &new_pos);
    void print_wall_move(std::shared_ptr<Pawn> pawn, const Wall &wall);

private:
    Game game_;
    std::vector<std::shared_ptr<IPlayer>> players_;
    std::vector<std::vector<char>> repr_;
};

}  /* namespace UI */
}  /* namespace Quoridor */

#endif  /* QUORIDOR_UI_CONSOLE_HPP_ */
