#include "graph_component.hpp"
#include "exceptions/exception.hpp"

template<> const char *GraphComponent::Base::name_ = "GraphComponent";

GraphComponent::GraphComponent(std::shared_ptr<Graph> graph, const QJsonObject &component_data) : graph_(graph), node_(), goal_nodes_()
{
    QJsonValue json_position = component_data["position"];
    QJsonValue json_goals = component_data["goals"];

    if ((!json_position.isArray()) || (!json_goals.isArray())) {
        throw invalid_json_error();
    }

    // set player initial position
    QJsonArray position = json_position.toArray();
    if (position.size() != 2) {
        throw invalid_json_error();
    }
    node_.set_row(position.at(0).toInt());
    node_.set_col(position.at(1).toInt());

    // set player goal nodes
    QJsonArray goals = json_goals.toArray();
    for (const auto &json_goal : goals) {
        if (!json_goal.isArray()) {
            throw invalid_json_error();
        }

        QJsonArray goal = json_goal.toArray();
        if (goal.size() != 2) {
            throw invalid_json_error();
        }
        goal_nodes_.emplace(goal.at(0).toInt(), goal.at(1).toInt());
    }
}

void GraphComponent::post_init()
{
    const std::shared_ptr<Actor> &actor = owner();
    if (actor) {
        graph_->add_actor(actor->id(), goal_nodes_);
        move_actor(node_);
    }
}

bool GraphComponent::move_actor(const Node &pos)
{
    bool res = graph_->move_actor(owner()->id(), pos);
    if (res) {
        node_ = pos;
    }
    return res;
}

bool GraphComponent::is_at_goal_node() const
{
    return (goal_nodes_.count(node_) > 0);
}

bool GraphComponent::set_wall(const Wall &wall)
{
    return graph_->set_wall(wall);
}

std::list<Node> GraphComponent::possible_moves() const
{
    return graph_->possible_moves(owner()->id());
}
