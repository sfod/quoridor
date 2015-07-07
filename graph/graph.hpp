#pragma once

#include <map>
#include "node.hpp"
#include "actors/actor.hpp"
#include "board_graph.hpp"
#include "wall.hpp"
#include "wall_grid.hpp"

struct actor_node_t {
    Node node;
    std::list<Node> possible_moves_;
    std::set<Node> goal_nodes_;
};

class Graph {
public:
    Graph();

    bool add_actor(ActorId id, const std::set<Node> &goal_nodes);
    bool move_actor(ActorId id, const Node &node);
    bool set_wall(ActorId id, const Wall &wall);

    Node node(ActorId id) const;
    std::list<Node> possible_moves(ActorId id) const;
    std::list<Node> shortest_path(ActorId id, const std::set<Node> &goal_nodes) const;

private:
    void set_possible_moves(ActorId id);

private:
    WallGrid wg_;
    std::shared_ptr<BoardGraph> board_graph_;
    std::map<ActorId, actor_node_t> actor_node_list_;
};
