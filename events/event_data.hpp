#pragma once

#include "actors/actor.hpp"
#include "game/game_data.hpp"
#include "graph/node.hpp"
#include "graph/wall.hpp"

typedef unsigned long EventType;

class EventData {
public:
    explicit EventData(float ts = 0) : ts_(ts) {}
    virtual ~EventData() {}
    virtual const EventType &event_type() const = 0;
    virtual float ts() const { return ts_; }

private:
    float ts_;
};


class EventData_MainMenu : public EventData {
public:
    EventData_MainMenu();
    virtual ~EventData_MainMenu();
    virtual const EventType &event_type() const;
    static const EventType event_type_;
};


class EventData_Options : public EventData {
public:
    EventData_Options();
    virtual ~EventData_Options();
    virtual const EventType &event_type() const;
    static const EventType event_type_;
};


class EventData_Game : public EventData {
public:
    EventData_Game();
    virtual ~EventData_Game();
    virtual const EventType &event_type() const;
    static const EventType event_type_;
};


class EventData_GameTerminated : public EventData {
public:
    EventData_GameTerminated();
    virtual ~EventData_GameTerminated();
    virtual const EventType &event_type() const;
    static const EventType event_type_;
};


class EventData_GameFinished : public EventData {
public:
    EventData_GameFinished();
    virtual ~EventData_GameFinished();
    virtual const EventType &event_type() const;
    static const EventType event_type_;
};


class EventData_Quit : public EventData {
public:
    EventData_Quit();
    virtual ~EventData_Quit();
    virtual const EventType &event_type() const;
    static const EventType event_type_;
};


class EventData_RequestNewActor : public EventData {
public:
    explicit EventData_RequestNewActor(PlayerType ptype);
    virtual ~EventData_RequestNewActor();
    virtual const EventType &event_type() const;
    static const EventType event_type_;

    PlayerType player_type() const { return ptype_; }

private:
    PlayerType ptype_;
};


class EventData_NewActor : public EventData {
public:
    explicit EventData_NewActor(ActorId actor_id);
    virtual ~EventData_NewActor();
    virtual const EventType &event_type() const;
    static const EventType event_type_;

    ActorId actor_id() const { return actor_id_; }

private:
    ActorId actor_id_;
};


class EventData_RequestActorMove : public EventData {
public:
    EventData_RequestActorMove(ActorId actor_id, const Node &node);
    virtual ~EventData_RequestActorMove();
    virtual const EventType &event_type() const;
    static const EventType event_type_;

    ActorId actor_id() const { return actor_id_; }
    Node node() const { return node_; }

private:
    ActorId actor_id_;
    Node node_;
};


class EventData_RequestSetWall : public EventData {
public:
    EventData_RequestSetWall(ActorId actor_id, const Wall &wall);
    virtual ~EventData_RequestSetWall();
    virtual const EventType &event_type() const;
    static const EventType event_type_;

    ActorId actor_id() const { return actor_id_; }
    const Wall &wall() const { return wall_; }

private:
    ActorId actor_id_;
    Wall wall_;
};


class EventData_MoveActor : public EventData {
public:
    EventData_MoveActor(ActorId actor_id, const Node &node,
            const std::list<Node> &possible_moves);
    virtual ~EventData_MoveActor();
    virtual const EventType &event_type() const;
    static const EventType event_type_;

    ActorId actor_id() const { return actor_id_; }
    Node node() const { return node_; }
    const std::list<Node> &possible_moves() const { return possible_moves_; }

private:
    ActorId actor_id_;
    Node node_;
    std::list<Node> possible_moves_;
};


class EventData_SetWall : public EventData {
public:
    EventData_SetWall(ActorId actor_id, const Wall &wall);
    virtual ~EventData_SetWall();
    virtual const EventType &event_type() const;
    static const EventType event_type_;

    ActorId actor_id() const { return actor_id_; }
    const Wall &wall() const { return wall_; }

private:
    ActorId actor_id_;
    Wall wall_;
};


class EventData_SetActorPossibleMoves : public EventData {
public:
    EventData_SetActorPossibleMoves(ActorId actor_id,
            const std::list<Node> &possible_moves);
    virtual ~EventData_SetActorPossibleMoves();
    virtual const EventType &event_type() const;
    static const EventType event_type_;

    ActorId actor_id() const { return actor_id_; }
    const std::list<Node> &possible_moves() const { return possible_moves_; }

private:
    ActorId actor_id_;
    std::list<Node> possible_moves_;
};

class EventData_SetActorActive : public EventData {
public:
    explicit EventData_SetActorActive(ActorId actor_id);
    virtual ~EventData_SetActorActive();
    virtual const EventType &event_type() const;
    static const EventType event_type_;

    ActorId actor_id() const { return actor_id_; }

private:
    ActorId actor_id_;
};
