#pragma once

#include "actor_component.hpp"

class WallComponent : public ActorComponentCRTP<WallComponent> {
public:
    explicit WallComponent(const QJsonObject &component_data);
    int wall_number() const { return wall_num_; }

private:
    int wall_num_;
};
