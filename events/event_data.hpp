#pragma once

#include "actors/actor.hpp"
#include "game/game_data.hpp"
#include "graph/node.hpp"
#include "graph/wall.hpp"

typedef std::size_t EventType;

/*
 * Base class for events
 */
class EventDataBase {
public:
    virtual ~EventDataBase() = default;
    virtual EventType event_type() const = 0;
};


template<typename Derived>
class EventData : public EventDataBase {
public:
    virtual ~EventData() = default;
    virtual EventType event_type() const override { return event_type_; }

public:
    static EventType event_type_;
};


class EventData_MainMenu : public EventData<EventData_MainMenu> {
public:
    virtual ~EventData_MainMenu() = default;
};


class EventData_Options : public EventData<EventData_Options> {
public:
    virtual ~EventData_Options() = default;
};


class EventData_Game : public EventData<EventData_Game> {
public:
    virtual ~EventData_Game() = default;
};


class EventData_GameTerminated : public EventData<EventData_GameTerminated> {
public:
    virtual ~EventData_GameTerminated() = default;
};


class EventData_GameFinished : public EventData<EventData_GameFinished> {
public:
    explicit EventData_GameFinished(ActorId actor_id);
    virtual ~EventData_GameFinished() = default;

    ActorId actor_id() const { return actor_id_; }

private:
    ActorId actor_id_;
};


class EventData_Quit : public EventData<EventData_Quit> {
public:
    virtual ~EventData_Quit() = default;
};


class EventData_RequestNewActor : public EventData<EventData_RequestNewActor> {
public:
    explicit EventData_RequestNewActor(PlayerType ptype);
    virtual ~EventData_RequestNewActor() = default;

    PlayerType player_type() const { return ptype_; }

private:
    PlayerType ptype_;
};


class EventData_NewActor : public EventData<EventData_NewActor> {
public:
    explicit EventData_NewActor(ActorId actor_id, const Node &node, const std::list<Node> &possible_moves);
    virtual ~EventData_NewActor() = default;

    ActorId actor_id() const { return actor_id_; }
    Node node() const { return node_; }
    const std::list<Node> &possible_moves() const { return possible_moves_; }

private:
    ActorId actor_id_;
    Node node_;
    std::list<Node> possible_moves_;
};


class EventData_RequestActorMove : public EventData<EventData_RequestActorMove> {
public:
    EventData_RequestActorMove(ActorId actor_id, const Node &node);
    virtual ~EventData_RequestActorMove() = default;

    ActorId actor_id() const { return actor_id_; }
    Node node() const { return node_; }

private:
    ActorId actor_id_;
    Node node_;
};


class EventData_RequestSetWall : public EventData<EventData_RequestSetWall> {
public:
    EventData_RequestSetWall(ActorId actor_id, const Wall &wall);
    virtual ~EventData_RequestSetWall() = default;

    ActorId actor_id() const { return actor_id_; }
    const Wall &wall() const { return wall_; }

private:
    ActorId actor_id_;
    Wall wall_;
};


class EventData_MoveActor : public EventData<EventData_MoveActor> {
public:
    EventData_MoveActor(ActorId actor_id, const Node &node,
            const std::list<Node> &possible_moves);
    virtual ~EventData_MoveActor() = default;

    ActorId actor_id() const { return actor_id_; }
    Node node() const { return node_; }
    const std::list<Node> &possible_moves() const { return possible_moves_; }

private:
    ActorId actor_id_;
    Node node_;
    std::list<Node> possible_moves_;
};


class EventData_SetWall : public EventData<EventData_SetWall> {
public:
    EventData_SetWall(ActorId actor_id, const Wall &wall);
    virtual ~EventData_SetWall() = default;

    ActorId actor_id() const { return actor_id_; }
    const Wall &wall() const { return wall_; }

private:
    ActorId actor_id_;
    Wall wall_;
};


class EventData_SetActorPossibleMoves : public EventData<EventData_SetActorPossibleMoves> {
public:
    EventData_SetActorPossibleMoves(ActorId actor_id,
            const std::list<Node> &possible_moves);
    virtual ~EventData_SetActorPossibleMoves() = default;

    ActorId actor_id() const { return actor_id_; }
    const std::list<Node> &possible_moves() const { return possible_moves_; }

private:
    ActorId actor_id_;
    std::list<Node> possible_moves_;
};


class EventData_SetActorActive : public EventData<EventData_SetActorActive> {
public:
    explicit EventData_SetActorActive(ActorId actor_id);
    virtual ~EventData_SetActorActive() = default;

    ActorId actor_id() const { return actor_id_; }

private:
    ActorId actor_id_;
};
