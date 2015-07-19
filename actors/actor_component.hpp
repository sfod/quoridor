#pragma once

#include <functional>
#include <memory>
#include <QJsonObject>

typedef unsigned long ComponentId;


class Actor;

class ActorComponent {
public:
    virtual ~ActorComponent() = default;

    virtual void post_init() {}

    virtual const char *name() const = 0;
    virtual void set_owner(std::shared_ptr<Actor> &owner) = 0;
    virtual std::shared_ptr<Actor> owner() const = 0;

    static ComponentId id(const char *name) { return std::hash<std::string>()(name); }
};


template<typename Derived>
class ActorComponentCRTP : public ActorComponent {
public:
    typedef ActorComponentCRTP<Derived> Base;

    virtual const char *name() const override { return name_; }
    virtual void set_owner(std::shared_ptr<Actor> &owner) override { owner_ = owner; }
    virtual std::shared_ptr<Actor> owner() const override { return owner_; }

    static const char *static_name() { return name_; }
    static ComponentId id() { return ActorComponent::id(name_); }

protected:
    static const char *name_;

private:
    std::shared_ptr<Actor> owner_;
};
