#include "straight_brain.hpp"
#include "events/event_manager.hpp"
#include "events/event_data_request_actor_move.hpp"

StraightBrain::StraightBrain() : actor_id_(-1), graph_(), goal_nodes_()
{
}

void StraightBrain::make_move()
{
    const std::list<Node> &path = graph_->shortest_path(actor_id_, goal_nodes_);

    auto req_move_event = std::make_shared<EventData_RequestActorMove>(actor_id_, *path.begin());
    EventManager::get()->queue_event(req_move_event);
}
