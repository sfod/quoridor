#pragma once

#include <memory>
#include <QString>
#include "actor.hpp"
#include "graph/graph.hpp"
#include "events/event_manager.hpp"

class ActorFactory {
public:
    std::shared_ptr<Actor> create_actor(const std::shared_ptr<EventManager> &event_manager, const std::shared_ptr<Graph> &graph, QString &resource_file,
            const std::vector<QString> &component_resource_files);

private:
    void add_components(const std::shared_ptr<EventManager> &event_manager, const std::shared_ptr<Graph> &graph, std::shared_ptr<Actor> actor,
            const QJsonObject &actor_components);
    std::shared_ptr<ActorComponent> create_actor_component(const std::shared_ptr<EventManager> &event_manager, const std::shared_ptr<Graph> &graph, const QString &type,
            const QJsonObject &component_data);
};
