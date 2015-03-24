#ifndef GAME_APP_H
#define GAME_APP_H

#include <memory>
#include <vector>

#include <QQmlEngine>
#include <QQmlComponent>

#include "events/event_manager.hpp"
#include "game_logic.hpp"

class GameApp {
public:
    GameApp();
    virtual ~GameApp();

    int run(int argc, char **argv);
    GameLogic *game_logic() const { return logic_; }

    void quit_delegate(const std::shared_ptr<EventData> &event);

    static GameApp *get();

private:
    void register_delegates();

private:
    EventManager *event_manager_;
    std::list<bs2::connection> conn_list_;
    GameLogic *logic_;
    QQmlEngine *qengine_;
    QQmlComponent *qcomponent_;
    QObject *qroot_;
};

#endif // GAME_APP_H
