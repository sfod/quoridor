#include <QObject>  // include it first to avoid error with boost::Q_FOREACH
#include "graph_component.hpp"
#include "game/game_app.hpp"

const char *GraphComponent::name_ = "GraphComponent";

GraphComponent::GraphComponent() : node_(), goal_nodes_()
{
    graph_ = GameApp::get()->game_logic()->graph();
}

GraphComponent::~GraphComponent()
{
}

bool GraphComponent::init(const boost::property_tree::ptree &component_data)
{
    // set player initial position
    boost::optional<const boost_pt::ptree &> pos =
            component_data.get_child_optional("position");
    if (!pos || ((*pos).size() != 2)) {
        return false;
    }
    auto pos_it = (*pos).begin();
    node_.set_row(pos_it->second.get_value<int>());
    node_.set_col((++pos_it)->second.get_value<int>());

    // set player goal nodes
    boost::optional<const boost_pt::ptree &> goals =
            component_data.get_child_optional("goals");
    if (!goals) {
        return false;
    }

    bool rc = true;
    for (auto &goal : *goals) {
        if (goal.second.size() != 2) {
            rc = false;
            break;
        }

        int items[2];
        int i = 0;
        for (auto &node_item : goal.second) {
            items[i++] = node_item.second.get_value<int>();
        }
        goal_nodes_.emplace(items[0], items[1]);
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
