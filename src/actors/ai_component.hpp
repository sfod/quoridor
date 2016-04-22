#pragma once

#include <memory>
#include "actor_component.hpp"
#include "graph/graph.hpp"
#include "AI/brain.hpp"

class AIComponent : public ActorComponentCRTP<AIComponent> {
public:
    AIComponent(std::shared_ptr<Graph> graph, const QJsonObject &component_data);

    virtual void post_init() override;

    void make_move();

private:
    std::shared_ptr<Brain> brain_;
};
