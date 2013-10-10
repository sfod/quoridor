#include "board.hpp"
#include "exception.hpp"
#include "gtest/gtest.h"

class BoardTest : public ::testing::Test {
protected:
    BoardTest() : board_(1, 2) {
    }

    Quoridor::Board board_;
};

TEST_F(BoardTest, ctor)
{
    EXPECT_EQ(1, board_.row_num());
    EXPECT_EQ(2, board_.col_num());
    // test invalid Board construction
    EXPECT_THROW(Quoridor::Board(-1, 1), Quoridor::Exception);
    EXPECT_THROW(Quoridor::Board(8, 0), Quoridor::Exception);
}

TEST_F(BoardTest, set_size)
{
    // test invalid Board sizes
    EXPECT_THROW(board_.set_size(0, 0), Quoridor::Exception);
    EXPECT_THROW(board_.set_size(0, 1), Quoridor::Exception);
    EXPECT_THROW(board_.set_size(1, 0), Quoridor::Exception);
    EXPECT_THROW(board_.set_size(-1, -1), Quoridor::Exception);
    EXPECT_THROW(board_.set_size(-1, 1), Quoridor::Exception);
    EXPECT_THROW(board_.set_size(1, -1), Quoridor::Exception);

    board_.set_size(10, 11);
    EXPECT_EQ(10, board_.row_num());
    EXPECT_EQ(11, board_.col_num());
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
