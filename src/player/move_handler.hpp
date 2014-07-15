#ifndef QUORIDOR_MOVE_HANDLER_HPP_
#define QUORIDOR_MOVE_HANDLER_HPP_

#include <atomic>
#include "iplayer.hpp"

namespace Quoridor {

class MoveHandler {
public:
    MoveHandler();
    virtual ~MoveHandler();

    const move_t &move() const { return move_; }
    bool is_ready() const { return is_ready_; }
    void reset() { is_ready_ = false; }

    void callback(const move_t &move);

private:
    move_t move_;
    std::atomic_bool is_ready_;
};

}  // namespace Quoridor

#endif  // QUORIDOR_MOVE_HANDLER_HPP_
