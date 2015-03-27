#include <QObject>  // include it to avoid error with boost::Q_FOREACH
#include "ai_component.hpp"
#include "game/game_app.hpp"
#include "graph_component.hpp"
#include "AI/randomer_brain.hpp"
#include "AI/straight_brain.hpp"

const char *AIComponent::name_ = "AIComponent";

AIComponent::AIComponent() : brain_()
{
}

AIComponent::~AIComponent()
{
}

bool AIComponent::init(const QJsonObject &component_data)
{
    QString brain_type = component_data["brain"].toString();
    if (brain_type.isEmpty()) {
        return false;
    }

    if (brain_type == "randomer") {
        brain_ = std::make_shared<RandomerBrain>();
    }
    else if (brain_type == "straight") {
        brain_ = std::make_shared<StraightBrain>();
    }
    else {
        return false;
    }

    return true;
}

void AIComponent::post_init()
{
    brain_->set_graph(GameApp::get()->game_logic()->graph());
    brain_->set_actor_id(owner()->id());

    auto cid = GraphComponent::id(GraphComponent::name_);
    auto graph_comp = std::dynamic_pointer_cast<GraphComponent>(owner()->component(cid));
    brain_->set_goal_nodes(graph_comp->goal_nodes());
}

void AIComponent::make_move()
{
    brain_->make_move();
}
