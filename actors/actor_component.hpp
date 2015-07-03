#pragma once

#include <functional>
#include <memory>
#include <QJsonObject>

typedef unsigned long ComponentId;


class Actor;

class ActorComponent {
public:
    virtual ~ActorComponent() {}

    virtual bool init(const QJsonObject &component_data) = 0;
    virtual void post_init() {}
    virtual const char *name() const = 0;
    virtual void set_owner(std::shared_ptr<Actor> &owner) { owner_ = owner; }
    virtual std::shared_ptr<Actor> owner() const { return owner_; }

    static ComponentId id(const char *name) {
        return std::hash<std::string>()(name);
    }

private:
    std::shared_ptr<Actor> owner_;
};
