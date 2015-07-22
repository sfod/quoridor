#pragma once

#include "event_data.hpp"

class EventData_SetActorPossibleMoves : public EventDataCRTP<EventData_SetActorPossibleMoves> {
public:
    EventData_SetActorPossibleMoves(ActorId actor_id,
            const std::list<Node> &possible_moves);
    ActorId actor_id() const;
    const std::list<Node> &possible_moves() const;

private:
    ActorId actor_id_;
    std::list<Node> possible_moves_;
};
