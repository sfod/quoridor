#include "event_data.hpp"
#include <functional>


typedef std::hash<std::string> strhash;

template<> EventType EventData_MainMenu::Base::event_type_ = strhash()("MainMenu");
template<> EventType EventData_Options::Base::event_type_ = strhash()("Options");
template<> EventType EventData_Game::Base::event_type_ = strhash()("Game");
template<> EventType EventData_GameTerminated::Base::event_type_ = strhash()("GameTerminated");
template<> EventType EventData_GameFinished::Base::event_type_ = strhash()("GameFinished");
template<> EventType EventData_Quit::Base::event_type_ = strhash()("Quit");
template<> EventType EventData_RequestNewActor::Base::event_type_ = strhash()("RequestNewActor");
template<> EventType EventData_NewActor::Base::event_type_ = strhash()("NewActor");
template<> EventType EventData_RequestActorMove::Base::event_type_ = strhash()("RequestActorMove");
template<> EventType EventData_RequestSetWall::Base::event_type_ = strhash()("RequestSetWall");
template<> EventType EventData_MoveActor::Base::event_type_ = strhash()("MoveActor");
template<> EventType EventData_SetWall::Base::event_type_ = strhash()("SetWall");
template<> EventType EventData_SetActorPossibleMoves::Base::event_type_ = strhash()("SetActorPossibleMoves");
template<> EventType EventData_SetActorActive::Base::event_type_ = strhash()("SetActorActive");


EventData_GameFinished::EventData_GameFinished(ActorId actor_id) : actor_id_(actor_id)
{
}

EventData_RequestNewActor::EventData_RequestNewActor(PlayerType ptype) : ptype_(ptype)
{
}

EventData_NewActor::EventData_NewActor(ActorId actor_id, const Node &node,
        const std::list<Node> &possible_moves, int wall_num)
    : actor_id_(actor_id), node_(node), possible_moves_(possible_moves), wall_num_(wall_num)
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
