#include "event_data.hpp"
#include <functional>

// TODO simplify
template<> EventType EventData_MainMenu::EventData<EventData_MainMenu>::event_type_ = std::hash<std::string>()("MainMenu");
template<> EventType EventData_Options::EventData<EventData_Options>::event_type_ = std::hash<std::string>()("Options");
template<> EventType EventData_Game::EventData<EventData_Game>::event_type_ = std::hash<std::string>()("Game");
template<> EventType EventData_GameTerminated::EventData<EventData_GameTerminated>::event_type_ = std::hash<std::string>()("GameTerminated");
template<> EventType EventData_GameFinished::EventData<EventData_GameFinished>::event_type_ = std::hash<std::string>()("GameFinished");
template<> EventType EventData_Quit::EventData<EventData_Quit>::event_type_ = std::hash<std::string>()("Quit");
template<> EventType EventData_RequestNewActor::EventData<EventData_RequestNewActor>::event_type_ = std::hash<std::string>()("RequestNewActor");
template<> EventType EventData_NewActor::EventData<EventData_NewActor>::event_type_ = std::hash<std::string>()("NewActor");
template<> EventType EventData_RequestActorMove::EventData<EventData_RequestActorMove>::event_type_ = std::hash<std::string>()("RequestActorMove");
template<> EventType EventData_RequestSetWall::EventData<EventData_RequestSetWall>::event_type_ = std::hash<std::string>()("RequestSetWall");
template<> EventType EventData_MoveActor::EventData<EventData_MoveActor>::event_type_ = std::hash<std::string>()("MoveActor");
template<> EventType EventData_SetWall::EventData<EventData_SetWall>::event_type_ = std::hash<std::string>()("SetWall");
template<> EventType EventData_SetActorPossibleMoves::EventData<EventData_SetActorPossibleMoves>::event_type_ = std::hash<std::string>()("SetActorPossibleMoves");
template<> EventType EventData_SetActorActive::EventData<EventData_SetActorActive>::event_type_ = std::hash<std::string>()("SetActorActive");


EventData_GameFinished::EventData_GameFinished(ActorId actor_id) : actor_id_(actor_id)
{
}

EventData_RequestNewActor::EventData_RequestNewActor(PlayerType ptype) : ptype_(ptype)
{
}

EventData_NewActor::EventData_NewActor(ActorId actor_id, const Node &node, const std::list<Node> &possible_moves)
    : actor_id_(actor_id), node_(node), possible_moves_(possible_moves)
{
}

EventData_RequestActorMove::EventData_RequestActorMove(ActorId actor_id,
        const Node &node)
    : actor_id_(actor_id), node_(node)
{
}

EventData_RequestSetWall::EventData_RequestSetWall(ActorId actor_id, const Wall &wall)
    : actor_id_(actor_id), wall_(wall)
{
}

EventData_MoveActor::EventData_MoveActor(ActorId actor_id, const Node &node,
        const std::list<Node> &possible_moves)
    : actor_id_(actor_id), node_(node), possible_moves_(possible_moves)
{
}

EventData_SetWall::EventData_SetWall(ActorId actor_id, const Wall &wall)
    : actor_id_(actor_id), wall_(wall)
{
}

EventData_SetActorPossibleMoves::EventData_SetActorPossibleMoves(
        ActorId actor_id,
        const std::list<Node> &possible_moves)
    : actor_id_(actor_id), possible_moves_(possible_moves)
{
}

EventData_SetActorActive::EventData_SetActorActive(ActorId actor_id)
    : actor_id_(actor_id)
{
}
