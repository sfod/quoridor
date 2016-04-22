#pragma once

#include "actor_component.hpp"
#include "graph/graph.hpp"

class WallComponent : public ActorComponentCRTP<WallComponent> {
public:
    WallComponent(std::shared_ptr<Graph> graph, const QJsonObject &component_data);
    int wall_number() const { return wall_num_; }

private:
    int wall_num_;
};
