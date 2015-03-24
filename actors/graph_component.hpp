#ifndef GRAPH_COMPONENT_HPP
#define GRAPH_COMPONENT_HPP

#include "actor_component.hpp"
#include "graph/graph.hpp"
#include "graph/wall.hpp"

class GraphComponent : public ActorComponent {
public:
    GraphComponent();
    virtual ~GraphComponent();

    virtual bool init(const boost_pt::ptree &component_data) override;
    virtual void post_init() override;
    virtual const char *name() const override { return name_; }

    bool move_actor(const Node &node);
    bool is_at_goal_node() const;

    bool set_wall(const Wall &wall);

    Node node() const { return node_; }
    std::list<Node> possible_moves() const;
    std::set<Node> goal_nodes() const { return goal_nodes_; }

public:
    static const char *name_;

private:
    std::shared_ptr<Graph> graph_;
    Node node_;
    std::set<Node> goal_nodes_;
};

#endif // GRAPH_COMPONENT_HPP
