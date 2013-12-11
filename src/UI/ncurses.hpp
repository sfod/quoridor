#ifndef QUORIDOR_UI_NCURSES_HPP_
#define QUORIDOR_UI_NCURSES_HPP_

#include <memory>

#include "iplayer.hpp"
#include "irunnable.hpp"
#include "game.hpp"

namespace Quoridor {
namespace UI {

class Ncurses : public IRunnable {
public:
    explicit Ncurses(int player_num);
    virtual ~Ncurses();

    void run();
    int set_player(int i, std::shared_ptr<IPlayer> player);

private:
    void display() const;
    void init_board_repr();
    void redraw_pawn(char p, const pos_t &old_pos, const pos_t &new_pos);
    void draw_wall(const Wall &wall);

private:
    Game game_;
    std::vector<std::shared_ptr<IPlayer>> players_;
    std::vector<std::vector<char>> repr_;
};

}  /* namespace UI */
}  /* namespace Quoridor */

#endif  /* QUORIDOR_UI_NCURSES_HPP_ */
