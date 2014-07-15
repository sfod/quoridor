#include "move_handler.hpp"

namespace Quoridor {

MoveHandler::MoveHandler() : move_(), is_ready_(false)
{
}

MoveHandler::~MoveHandler()
{
}

void MoveHandler::callback(const move_t &move)
{
    move_ = move;
    is_ready_ = true;
}

}  // namespace Quoridor
