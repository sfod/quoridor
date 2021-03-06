#pragma once

#include <memory>
#include <QString>
#include "actor.hpp"

class ActorFactory {
public:
    std::shared_ptr<Actor> create_actor(QString &resource_file,
            const std::vector<QString> &component_resource_files);

private:
    void add_components(std::shared_ptr<Actor> &actor, const QJsonObject &actor_components);
    std::shared_ptr<ActorComponent> create_actor_component(
            const QString &type, const QJsonObject &component_data);
};
