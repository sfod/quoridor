#ifndef AI_VIEW_HPP
#define AI_VIEW_HPP

#include "iview.hpp"
#include "events/event_manager.hpp"

class AIView : public IView {
public:
    AIView();
    virtual ~AIView();

    virtual bool init() override;
    virtual void on_msg() override;
    virtual void on_update() override;
    virtual void attach(ActorId actor_id) override;

    void set_active_delegate(const std::shared_ptr<EventData> &event);

private:
    std::list<bs2::connection> conn_list_;
    ActorId actor_id_;
};

#endif // AI_VIEW_HPP
