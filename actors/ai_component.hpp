#pragma once

#include <memory>
#include "actor_component.hpp"
#include "AI/brain.hpp"

class AIComponent : public ActorComponentCRTP<AIComponent> {
public:
    explicit AIComponent(const QJsonObject &component_data);

    virtual void post_init() override;

    void make_move();

private:
    std::shared_ptr<Brain> brain_;
};
