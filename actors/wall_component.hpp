#pragma once

#include "actor_component.hpp"

class WallComponent : public ActorComponent {
public:
    WallComponent(const QJsonObject &component_data);
    virtual ~WallComponent() = default;

    virtual void post_init() override;
    virtual const char *name() const override { return name_; }

    int wall_number() const { return wall_num_; }

public:
    static const char *name_;

private:
    int wall_num_;
};
