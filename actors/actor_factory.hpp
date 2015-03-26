#pragma once

#include <memory>
#include <QString>
#include "actor.hpp"

class ActorFactory
{
public:
    ActorFactory();
    ~ActorFactory();

    std::shared_ptr<Actor> create_actor(QString &resource_file,
            const std::vector<QString> &component_resource_files);

    std::shared_ptr<Actor> create_actor(const std::string &resource,
            const std::vector<std::string> &component_resources);
    std::shared_ptr<ActorComponent> create_actor_component(
            const std::string &type, const boost_pt::ptree &component_data);
};
