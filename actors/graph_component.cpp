#include "graph_component.hpp"
#include <QObject>
#include "game/game_app.hpp"

const char *GraphComponent::name_ = "GraphComponent";

GraphComponent::GraphComponent() : node_(), goal_nodes_()
{
    graph_ = GameApp::get()->game_logic()->graph();
}

bool GraphComponent::init(const QJsonObject &component_data)
{
    qDebug() << component_data;

    // set player initial position
    QJsonArray pos = component_data["position"].toArray();
    if (pos.size() != 2) {
        return false;
    }
    node_.set_row(pos.at(0).toInt());
    node_.set_col(pos.at(1).toInt());

    // set player goal nodes
    bool rc = true;
    QJsonArray goals = component_data["goals"].toArray();
    for (const auto &goal : goals) {
        QJsonArray g = goal.toArray();
        if (g.size() != 2) {
            rc = false;
            break;
        }
        goal_nodes_.emplace(g.at(0).toInt(), g.at(1).toInt());
    }

    return rc;
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
    return graph_->set_wall(owner()->id(), wall);
}

std::list<Node> GraphComponent::possible_moves() const
{
    return graph_->possible_moves(owner()->id());
}
