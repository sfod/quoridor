#include "move.hpp"

namespace Quoridor {

Move::Move() : action_(kEND), cont_(false)
{
}

Move::Move(int action) : action_(action), cont_(false)
{
}

Move::~Move()
{
}

void Move::set_action(int action)
{
    action_ = action;
}

int Move::action() const
{
    return action_;
}

void Move::set_cont(bool cont)
{
    cont_ = cont;
}

bool Move::cont() const
{
    return cont_;
}

}  /* namespace Quoridor */
