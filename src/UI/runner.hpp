#ifndef QUORIDOR_RUNNER_HPP_
#define QUORIDOR_RUNNER_HPP_

#include <memory>

#include "irunnable.hpp"
#include "game.hpp"
#include "iplayer.hpp"
#include "UI/ui.hpp"
#include "UI/ui_factory.hpp"

namespace Quoridor {
namespace UI {

class Runner : public IRunnable {
public:
    Runner();
    virtual ~Runner();

    virtual void create_ui(UIFactory &uif, const std::string &ui_type);
    virtual void set_player(int i, std::shared_ptr<IPlayer> player);
    virtual void run();

private:
    void init_board_repr() const;
    void redraw_pawn(char p, const pos_t &old_pos, const pos_t &new_pos) const;
    void draw_wall(const Wall &wall) const;

private:
    Game game_;
    std::vector<std::shared_ptr<IPlayer>> players_;
    std::shared_ptr<UIImpl> ui_;
    mutable std::vector<std::vector<char>> repr_;
};

}  /* namespace UI */
}  /* namespace Quoridor */

#endif  /* QUORIDOR_RUNNER_HPP_ */
