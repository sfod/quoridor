#pragma once

#include <memory>
#include "actor_component.hpp"
#include "AI/brain.hpp"

class AIComponent : public ActorComponent {
public:
    explicit AIComponent(const QJsonObject &component_data);
    virtual ~AIComponent() = default;

    virtual void post_init() override;
    virtual const char *name() const override { return name_; }

    void make_move();

public:
    static const char *name_;

private:
    std::shared_ptr<Brain> brain_;
};
