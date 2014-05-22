#ifndef QUORIDOR_POS_HPP_
#define QUORIDOR_POS_HPP_

namespace Quoridor {

class Pos {
public:
    Pos();
    Pos(int row, int col);
    Pos(const Pos &pos);

    int row() const { return row_; }
    int col() const { return col_; }

    bool operator<(const Pos &pos) const;
    bool operator==(const Pos &pos) const;
    bool operator!=(const Pos &pos) const;
    const Pos &operator+=(const Pos &pos);
    const Pos operator+(const Pos &pos);

private:
    int row_;
    int col_;
};

}  // namespace Quoridor

#endif  // QUORIDOR_POS_HPP_
