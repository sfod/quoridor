#include "pos.hpp"
#include <tuple>

namespace Quoridor {

Pos::Pos() : row_(0), col_(0)
{
}

Pos::Pos(int row, int col) : row_(row), col_(col)
{
}

Pos::Pos(const Pos &pos)
{
    row_ = pos.row_;
    col_ = pos.col_;
}

bool Pos::operator<(const Pos &pos) const
{
    return std::tie(row_, col_) < std::tie(pos.row_, pos.col_);
}

bool Pos::operator==(const Pos &pos) const
{
    return (row_ == pos.row_) && (col_ == pos.col_);
}

bool Pos::operator!=(const Pos &pos) const
{
    return !(*this == pos);
}

const Pos &Pos::operator+=(const Pos &pos)
{
    row_ += pos.row_;
    col_ += pos.col_;
    return *this;
}
const Pos Pos::operator+(const Pos &pos) {
    Pos p = *this;
    p += pos;
    return p;
}

}  // namespace Quoridor
