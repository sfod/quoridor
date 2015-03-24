#pragma once

#include <boost/property_tree/ptree.hpp>
#include <boost/functional/hash.hpp>

namespace boost_pt = boost::property_tree;
typedef unsigned long ComponentId;


class Actor;

class ActorComponent {
public:
    virtual ~ActorComponent() {}

    virtual bool init(const boost_pt::ptree &component_data) = 0;
    virtual void post_init() {}
    virtual const char *name() const = 0;
    virtual void set_owner(std::shared_ptr<Actor> &owner) { owner_ = owner; }
    virtual std::shared_ptr<Actor> owner() const { return owner_; }

    static ComponentId id(const char *name) {
        boost::hash<std::string> hash;
        return hash(name);
    }

private:
    std::shared_ptr<Actor> owner_;
};
