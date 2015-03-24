#ifndef AI_COMPONENT_HPP
#define AI_COMPONENT_HPP

#include <memory>
#include "actor_component.hpp"
#include "AI/brain.hpp"

class AIComponent : public ActorComponent {
public:
    AIComponent();
    virtual ~AIComponent();

    virtual bool init(const boost::property_tree::ptree &component_data) override;
    virtual void post_init() override;
    virtual const char *name() const override { return name_; }

    void make_move();

public:
    static const char *name_;

private:
    std::shared_ptr<Brain> brain_;
};

#endif // AI_COMPONENT_HPP
