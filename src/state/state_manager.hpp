#ifndef QUORIDOR_STATE_MANAGER_HPP_
#define QUORIDOR_STATE_MANAGER_HPP_

#include <memory>
#include <vector>
#include "istate.hpp"
#include "UI/ui_factory.hpp"

namespace Quoridor {

class IState;

class StateManager {
public:
    StateManager();
    ~StateManager();

    void create_ui(UI::UIFactory &uif, const std::string &ui_type);
    std::shared_ptr<UI::UI> ui() const;
    void change_state(std::shared_ptr<IState> state);

    void handle_events();
    void update();
    void draw();

    bool is_running() const { return is_running_; }
    void stop() { is_running_ = false; }

private:
    std::vector<std::shared_ptr<IState>> states_;
    std::shared_ptr<UI::UI> ui_;
    bool is_running_;
};

}  /* namespace Quoridor */

#endif  /* QUORIDOR_STATE_MANAGER_HPP_ */
