#include "node.hpp"
#include <gtest/gtest.h>

using namespace Quoridor;

TEST(node_test, test_neighbours)
{
    {
        Node n(0, 0);
        std::vector<Node> neighbours;
        n.neighbours(3, 3, &neighbours);
        EXPECT_EQ(3, neighbours.size());
        EXPECT_EQ(Node(0, 1), neighbours[0]);
        EXPECT_EQ(Node(1, 0), neighbours[1]);
        EXPECT_EQ(Node(1, 1), neighbours[2]);
    }
    {
        Node n(1, 1);
        std::vector<Node> neighbours;
        n.neighbours(3, 3, &neighbours);
        EXPECT_EQ(8, neighbours.size());
    }
    {
        Node n(2, 1);
        std::vector<Node> neighbours;
        n.neighbours(3, 3, &neighbours);
        EXPECT_EQ(5, neighbours.size());
    }
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
