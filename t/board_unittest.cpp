#include "board.hpp"
#include "gtest/gtest.h"

class BoardTest : public ::testing::Test {
protected:
    BoardTest() : board_(1, 2) {
    }
    virtual ~BoardTest() {
    }

    virtual void SetUp() {
    }

    Quoridor::Board board_;
};

TEST_F(BoardTest, set_size)
{
    board_.set_size(10, 11);
    EXPECT_EQ(10, board_.row_num());
    EXPECT_EQ(11, board_.col_num());
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
