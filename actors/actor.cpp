#include "actor.hpp"
#include "exceptions/exception.hpp"

Actor::Actor(ActorId id, QJsonObject actor_data) : component_list_(), id_(id)
{
    if (actor_data["type"].isUndefined()) {
        throw invalid_json_error();
    }
    type_ = actor_data["type"].toString();
}

void Actor::post_init()
{
    for (auto component : component_list_) {
        component.second->post_init();
    }
}

void Actor::add_component(std::shared_ptr<ActorComponent> &component)
{
    component_list_[ActorComponent::id(component->name())] = component;
}

std::shared_ptr<ActorComponent> Actor::component(ComponentId id) const
{
    return component_list_.at(id);
}
