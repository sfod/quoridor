#pragma once

#include <map>
#include <memory>
#include <boost/property_tree/ptree.hpp>
#include "actor_component.hpp"

namespace boost_pt = boost::property_tree;
typedef unsigned long ActorId;

class Actor {
public:
    explicit Actor(ActorId id);
    ~Actor();

    bool init(const boost_pt::ptree &actor_data);
    void post_init();
    void add_component(std::shared_ptr<ActorComponent> &component);

    std::shared_ptr<ActorComponent> component(ComponentId id) const;

    ActorId id() const { return id_; }

private:
    std::map<ComponentId, std::shared_ptr<ActorComponent>> component_list_;
    ActorId id_;
    std::string type_;
};
