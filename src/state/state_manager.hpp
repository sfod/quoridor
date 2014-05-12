#ifndef QUORIDOR_STATE_MANAGER_HPP_
#define QUORIDOR_STATE_MANAGER_HPP_

#include <memory>
#include <vector>

#include <SDL2/SDL.h>

#include "istate.hpp"

namespace Quoridor {

class IState;

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
    void init_sdl_();
    void init_resource_provider_();
    void inject_time_pulse_();

private:
    SDL_Window *win_;
    std::shared_ptr<IState> cur_state_;
    std::shared_ptr<IState> new_state_;
    double last_time_pulse_;
    bool is_running_;
};

}  /* namespace Quoridor */

#endif  /* QUORIDOR_STATE_MANAGER_HPP_ */
