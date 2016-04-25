#pragma once

#include "iview.hpp"
#include "actors/actor_keeper.hpp"
#include "events/event_manager.hpp"

class AIView : public QObject, public IView {
    Q_OBJECT

public:
    AIView(const std::shared_ptr<ActorKeeper> &actor_keeper, const std::shared_ptr<EventManager> &event_manager);

    virtual void on_msg() override;
    virtual void on_update() override;
    virtual void attach(ActorId actor_id) override;

    void set_active_delegate(const std::shared_ptr<EventData> &event);

private:
    ActorId actor_id_;
    std::shared_ptr<EventManager> event_manager_;
    std::shared_ptr<ActorKeeper> actor_keeper_;
};
