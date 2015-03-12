#include "event_data.hpp"

const EventType EventData_MainMenu::event_type_(1011);
const EventType EventData_Options::event_type_(1018);
const EventType EventData_Game::event_type_(1012);
const EventType EventData_GameTerminated::event_type_(1020);
const EventType EventData_GameFinished::event_type_(1022);
const EventType EventData_Quit::event_type_(1013);
const EventType EventData_RequestNewActor::event_type_(1019);
const EventType EventData_NewActor::event_type_(1014);
const EventType EventData_RequestActorMove::event_type_(1015);
const EventType EventData_RequestSetWall::event_type_(1023);
const EventType EventData_MoveActor::event_type_(1016);
const EventType EventData_SetWall::event_type_(1024);
const EventType EventData_SetActorPossibleMoves::event_type_(1021);
const EventType EventData_SetActorActive::event_type_(1017);


EventData_MainMenu::EventData_MainMenu()
{
}

EventData_MainMenu::~EventData_MainMenu()
{
}

const EventType &EventData_MainMenu::event_type() const
{
    return event_type_;
}


EventData_Options::EventData_Options()
{
}

EventData_Options::~EventData_Options()
{
}

const EventType &EventData_Options::event_type() const
{
    return event_type_;
}


EventData_Game::EventData_Game()
{
}

EventData_Game::~EventData_Game()
{
}

const EventType &EventData_Game::event_type() const
{
    return event_type_;
}


EventData_GameTerminated::EventData_GameTerminated()
{
}

EventData_GameTerminated::~EventData_GameTerminated()
{
}

const EventType &EventData_GameTerminated::event_type() const
{
    return event_type_;
}


EventData_GameFinished::EventData_GameFinished()
{
}

EventData_GameFinished::~EventData_GameFinished()
{
}

const EventType &EventData_GameFinished::event_type() const
{
    return event_type_;
}


EventData_Quit::EventData_Quit()
{
}

EventData_Quit::~EventData_Quit()
{
}

const EventType &EventData_Quit::event_type() const
{
    return event_type_;
}


EventData_RequestNewActor::EventData_RequestNewActor(PlayerType ptype) : ptype_(ptype)
{
}

EventData_RequestNewActor::~EventData_RequestNewActor()
{
}

const EventType &EventData_RequestNewActor::event_type() const
{
    return event_type_;
}


EventData_NewActor::EventData_NewActor(ActorId actor_id) : actor_id_(actor_id)
{
}

EventData_NewActor::~EventData_NewActor()
{
}

const EventType &EventData_NewActor::event_type() const
{
    return event_type_;
}


EventData_RequestActorMove::EventData_RequestActorMove(ActorId actor_id,
        const Node &node)
    : actor_id_(actor_id), node_(node)
{
}

EventData_RequestActorMove::~EventData_RequestActorMove()
{
}

const EventType &EventData_RequestActorMove::event_type() const
{
    return event_type_;
}


EventData_RequestSetWall::EventData_RequestSetWall(ActorId actor_id, const Wall &wall)
    : actor_id_(actor_id), wall_(wall)
{
}

EventData_RequestSetWall::~EventData_RequestSetWall()
{
}

const EventType &EventData_RequestSetWall::event_type() const
{
    return event_type_;
}


EventData_MoveActor::EventData_MoveActor(ActorId actor_id, const Node &node,
        const std::list<Node> &possible_moves)
    : actor_id_(actor_id), node_(node), possible_moves_(possible_moves)
{
}

EventData_MoveActor::~EventData_MoveActor()
{
}

const EventType &EventData_MoveActor::event_type() const
{
    return event_type_;
}


EventData_SetWall::EventData_SetWall(ActorId actor_id, const Wall &wall)
    : actor_id_(actor_id), wall_(wall)
{
}

EventData_SetWall::~EventData_SetWall()
{
}

const EventType &EventData_SetWall::event_type() const
{
    return event_type_;
}


EventData_SetActorPossibleMoves::EventData_SetActorPossibleMoves(
        ActorId actor_id,
        const std::list<Node> &possible_moves)
    : actor_id_(actor_id), possible_moves_(possible_moves)
{
}

EventData_SetActorPossibleMoves::~EventData_SetActorPossibleMoves()
{
}

const EventType &EventData_SetActorPossibleMoves::event_type() const
{
    return event_type_;
}


EventData_SetActorActive::EventData_SetActorActive(ActorId actor_id)
    : actor_id_(actor_id)
{
}

EventData_SetActorActive::~EventData_SetActorActive()
{
}

const EventType &EventData_SetActorActive::event_type() const
{
    return event_type_;
}
