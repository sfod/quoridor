#include "graph.hpp"
#include <QDebug>

// @fixme get board size from config file
Graph::Graph() : board_graph_(new BoardGraph(9, 9)), actor_node_list_()
{
}

Graph::~Graph()
{
}

bool Graph::add_actor(ActorId id, const std::set<Node> &goal_nodes)
{
    if (actor_node_list_.count(id) == 0) {
        actor_node_t actor_node = { Node(), {}, goal_nodes };
        actor_node_list_[id] = actor_node;
        return true;
    }
    return false;
}

bool Graph::move_actor(ActorId id, const Node &node)
{
    // @fixme check blocking and unblocking nodes
    if (actor_node_list_.count(id) > 0) {
        board_graph_->unblock_node(actor_node_list_[id].node);
        board_graph_->block_node(node);
        actor_node_list_[id].node = node;

        for (auto actor_node : actor_node_list_) {
            set_possible_moves(actor_node.first);
        }
        return true;
    }
    return false;
}

bool Graph::set_wall(ActorId id, const Wall &wall)
{
    std::vector<goal_nodes_t> goal_nodes_list;
    goal_nodes_t goal_node;
    for (const auto &ac : actor_node_list_) {
        goal_node.node = ac.second.node;
        goal_node.goal_nodes = &ac.second.goal_nodes_;
        goal_nodes_list.push_back(goal_node);
    }

    auto res = board_graph_->remove_edges(wall.affected_nodes(), goal_nodes_list, false);
    if (res) {
        for (auto actor_node : actor_node_list_) {
            set_possible_moves(actor_node.first);
        }
    }

    return res;
}

Node Graph::node(ActorId id) const
{
    if (actor_node_list_.count(id) > 0) {
        return actor_node_list_.at(id).node;
    }
    return Node();
}

std::list<Node> Graph::possible_moves(ActorId id) const
{
    if (actor_node_list_.count(id) > 0) {
        return actor_node_list_.at(id).possible_moves_;
    }
    return std::list<Node>();
}

std::list<Node> Graph::shortest_path(ActorId id, const std::set<Node> &goal_nodes) const
{
    std::list<Node> path;
    size_t n = board_graph_->shortest_path(actor_node_list_.at(id).node, goal_nodes, &path);
    if (n == 0) {
        // @todo
    }
    return path;
}

// @fixme
void Graph::set_possible_moves(ActorId id)
{
    if (actor_node_list_.count(id) > 0) {
        actor_node_t &actor_node = actor_node_list_[id];
        Node &node = actor_node.node;

        actor_node.possible_moves_.clear();
        actor_node.possible_moves_ = board_graph_->adjacent_nodes(node);
    }
}
