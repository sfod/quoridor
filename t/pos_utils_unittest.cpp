#include "pos_utils.hpp"
#include <gtest/gtest.h>

TEST(node_to_pos_test, handle_valid_nodes)
{
    Quoridor::PosUtils pu(9, 52, 50);
    Quoridor::Node n;
    CEGUI::UVector2 p;

    n.set_row(1);
    n.set_col(1);
    p = pu.node_to_pos(n);
    EXPECT_FLOAT_EQ(0.17957746478873, p.d_x.d_scale);
    EXPECT_FLOAT_EQ(0.7288732394, p.d_y.d_scale);

    n.set_row(4);
    n.set_col(8);
    p = pu.node_to_pos(n);
    EXPECT_FLOAT_EQ(0.82042253521, p.d_x.d_scale);
    EXPECT_FLOAT_EQ(0.454225352, p.d_y.d_scale);

    n.set_row(8);
    n.set_col(0);
    p = pu.node_to_pos(n);
    EXPECT_FLOAT_EQ(0.08802817, p.d_x.d_scale);
    EXPECT_FLOAT_EQ(0.08802817, p.d_y.d_scale);

    n.set_row(0);
    n.set_col(6);
    p = pu.node_to_pos(n);
    EXPECT_FLOAT_EQ(0.63732394, p.d_x.d_scale);
    EXPECT_FLOAT_EQ(0.820422535, p.d_y.d_scale);
}

TEST(pos_to_node_test, handle_positions_within_board)
{
    Quoridor::PosUtils pu(9, 52, 50);
    Quoridor::Node n;
    CEGUI::UVector2 p({0.0, 0.0}, {0.0, 0.0});

    p.d_x.d_scale = 0.5;
    p.d_y.d_scale = 0.5;
    n = pu.pos_to_node(p);
    EXPECT_EQ(4, n.row());
    EXPECT_EQ(4, n.col());
}

TEST(pos_to_node_test, handle_positions_outside_board)
{
    Quoridor::PosUtils pu(9, 52, 50);
    Quoridor::Node n;
    CEGUI::UVector2 p({0.0, 0.0}, {0.0, 0.0});

    p.d_x.d_scale = 0;
    p.d_y.d_scale = 0;
    n = pu.pos_to_node(p);
    EXPECT_EQ(8, n.row());
    EXPECT_EQ(0, n.col());

    p.d_x.d_scale = 0.99999;
    p.d_y.d_scale = 0.99999;
    n = pu.pos_to_node(p);
    EXPECT_EQ(0, n.row());
    EXPECT_EQ(8, n.col());

    p.d_x.d_scale = 1.0;
    p.d_y.d_scale = 1.0;
    n = pu.pos_to_node(p);
    EXPECT_EQ(0, n.row());
    EXPECT_EQ(8, n.col());
}

TEST(pos_to_node_test, handle_positions_outside_window)
{
    Quoridor::PosUtils pu(9, 52, 50);
    Quoridor::Node n;
    CEGUI::UVector2 p({0.0, 0.0}, {0.0, 0.0});

    p.d_x.d_scale = -0.1;
    p.d_y.d_scale = -0.1;
    n = pu.pos_to_node(p);
    EXPECT_EQ(8, n.row());
    EXPECT_EQ(0, n.col());

    p.d_x.d_scale = 1.1;
    p.d_y.d_scale = 1.1;
    n = pu.pos_to_node(p);
    EXPECT_EQ(0, n.row());
    EXPECT_EQ(8, n.col());
}

TEST(pos_to_wall_test, hendle_positions_within_board)
{
    Quoridor::PosUtils pu(9, 52, 50);
    Quoridor::Wall w;
    CEGUI::UVector2 p({0.0, 0.0}, {0.0, 0.0});

    p.d_x.d_scale = 0.5;
    p.d_y.d_scale = 0.5;
    w = pu.pos_to_wall(p, 2);
    EXPECT_EQ(4, w.line());
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
