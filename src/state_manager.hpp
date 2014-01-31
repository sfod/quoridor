#ifndef QUORIDOR_STATE_MANAGER_HPP_
#define QUORIDOR_STATE_MANAGER_HPP_

#include <memory>
#include <vector>
#include "istate.hpp"

namespace Quoridor {

class StateManager {
public:
    StateManager();
    ~StateManager();

    void change_state(std::shared_ptr<IState> state);

    void handle_events();
    void update();
    void draw();

    bool is_running() const { return is_running_; }
    void stop() { is_running_ = false; }

private:
    std::vector<std::shared_ptr<IState>> states_;
    bool is_running_;
};

}  /* namespace Quoridor */

#endif  /* QUORIDOR_STATE_MANAGER_HPP_ */
