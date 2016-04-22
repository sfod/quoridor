#include "ai_view.hpp"
#include "actors/ai_component.hpp"
#include "events/event_data_set_actor_active.hpp"

AIView::AIView(std::shared_ptr<ActorKeeper> actor_keeper) : actor_id_(-1), actor_keeper_(actor_keeper)
{
    EventManager::get()->add_listener(this,
            std::bind(&AIView::set_active_delegate, this, std::placeholders::_1),
            EventData_SetActorActive::static_event_type());
}

void AIView::on_msg()
{
}

void AIView::on_update()
{
}

void AIView::attach(ActorId actor_id)
{
    actor_id_ = actor_id;
}

void AIView::set_active_delegate(const std::shared_ptr<EventData> &event)
{
    auto active_event = std::dynamic_pointer_cast<EventData_SetActorActive>(event);
    if (active_event->actor_id() == actor_id_) {
        // FIXME remove actor_keeper from chain
        const std::shared_ptr<Actor> &actor = actor_keeper_->actor(active_event->actor_id());
        auto ai_comp = std::dynamic_pointer_cast<AIComponent>(actor->component(AIComponent::id()));
        ai_comp->make_move();
    }
}
