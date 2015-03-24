#ifndef BRAIN_HPP
#define BRAIN_HPP

#include <memory>
#include "graph/graph.hpp"

class Brain {
public:
    virtual void make_move() = 0;
    virtual void set_actor_id(ActorId actor_id) = 0;
    virtual void set_graph(const std::shared_ptr<Graph> &graph) = 0;
    virtual void set_goal_nodes(const std::set<Node> &goal_nodes) = 0;
};

#endif // BRAIN_HPP
