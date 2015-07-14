#pragma once

#include <map>
#include <memory>
#include <QString>
#include <QJsonObject>
#include "actor_component.hpp"

typedef unsigned long ActorId;

class Actor {
public:
    Actor(ActorId id, QJsonObject actor_data);
    ~Actor() = default;

    void post_init();
    void add_component(std::shared_ptr<ActorComponent> &component);

    std::shared_ptr<ActorComponent> component(ComponentId id) const;

    ActorId id() const { return id_; }

private:
    std::map<ComponentId, std::shared_ptr<ActorComponent>> component_list_;
    ActorId id_;
    QString type_;
};
