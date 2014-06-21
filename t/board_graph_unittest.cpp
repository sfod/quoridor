#include "board_graph.hpp"
#include "gtest/gtest.h"

using namespace Quoridor;

class BoardGraphTest : public ::testing::Test {
protected:
    BoardGraphTest() : bg_(5, 5) {
    }

    BoardGraph bg_;
};

TEST_F(BoardGraphTest, block_node_wo_surroundings)
{
    bg_.block_node(Node(2, 2));
    EXPECT_TRUE(bg_.is_adjacent(Node(1, 2), Node(3, 2)));
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
