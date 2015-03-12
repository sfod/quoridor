#ifndef STRAIGHT_BRAIN_HPP
#define STRAIGHT_BRAIN_HPP

#include "brain.hpp"
#include "graph/graph.hpp"

class StraightBrain : public Brain {
public:
    StraightBrain();
    ~StraightBrain();

    virtual void make_move() override;
    virtual void set_actor_id(ActorId actor_id) { actor_id_ = actor_id; }
    virtual void set_graph(const std::shared_ptr<Graph> &graph) { graph_ = graph; }
    virtual void set_goal_nodes(const std::set<Node> &goal_nodes) { goal_nodes_ = goal_nodes; }

private:
    ActorId actor_id_;
    std::shared_ptr<Graph> graph_;
    std::set<Node> goal_nodes_;
};

#endif // STRAIGH_TBRAIN_HPP
