#pragma once

#include <memory>
#include <vector>

#include <QObject>
#include <QGuiApplication>
#include <QQmlEngine>
#include <QQmlComponent>

#include "events/event_manager.hpp"
#include "game_logic.hpp"

class GameApp : public QObject {
    Q_OBJECT

public:
    GameApp(int &argc, char **argv);

    int run();
    std::shared_ptr<GameLogic> game_logic() const { return logic_; }

    static GameApp *get();

public slots:
    void quit_delegate(const std::shared_ptr<EventData> &event);

private:
    void register_delegates();

private:
    QGuiApplication qapp_;
    QQmlEngine qengine_;
    QQmlComponent qcomponent_;
    QObject *qroot_;

    std::shared_ptr<EventManager> event_manager_;
    std::shared_ptr<GameLogic> logic_;
};
