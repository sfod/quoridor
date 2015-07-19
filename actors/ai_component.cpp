#include <QObject>  // include it to avoid error with boost::Q_FOREACH
#include "ai_component.hpp"
#include "game/game_app.hpp"
#include "graph_component.hpp"
#include "AI/randomer_brain.hpp"
#include "AI/straight_brain.hpp"
#include "exceptions/exception.hpp"

template<> const char *AIComponent::Base::name_ = "AIComponent";

AIComponent::AIComponent(const QJsonObject &component_data)
{
    if (component_data["brain"].isUndefined()) {
        throw invalid_json_error();
    }

    QString brain_type = component_data["brain"].toString();
    if (brain_type == "randomer") {
        brain_ = std::make_shared<RandomerBrain>();
    }
    else if (brain_type == "straight") {
        brain_ = std::make_shared<StraightBrain>();
    }
    else {
        throw invalid_json_error();
    }
}

void AIComponent::post_init()
{
    brain_->set_graph(GameApp::get()->game_logic()->graph());
    brain_->set_actor_id(owner()->id());

    auto graph_comp = std::dynamic_pointer_cast<GraphComponent>(
                owner()->component(GraphComponent::id())
    );
    brain_->set_goal_nodes(graph_comp->goal_nodes());
}

void AIComponent::make_move()
{
    brain_->make_move();
}
