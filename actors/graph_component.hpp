#pragma once

#include "actor_component.hpp"
#include "graph/graph.hpp"
#include "graph/wall.hpp"

class GraphComponent : public ActorComponent {
public:
    GraphComponent();
    virtual ~GraphComponent() = default;

    virtual bool init(const QJsonObject &component_data) override;
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
