#include "game_app.hpp"

#include <QGuiApplication>
#include <QQmlEngine>
#include <QQmlComponent>
#include <QTimer>
#include <QDebug>

#include "events/event_manager.hpp"
#include "events/event_caller.hpp"
#include "graph/wall.hpp"

static GameApp *g_app;

GameApp::GameApp()
    : event_manager_(new EventManager), conn_list_(), logic_(),
      qengine_(), qcomponent_(), qroot_()
{
    g_app = this;
}

GameApp::~GameApp()
{
    for (auto conn : conn_list_) {
        conn.disconnect();
    }
}

int GameApp::run(int argc, char **argv)
{
    std::shared_ptr<QGuiApplication> qapp = std::make_shared<QGuiApplication>(argc, argv);

    WallEnumClass::declareQML();

    qengine_ = new QQmlEngine;
    qcomponent_ = new QQmlComponent(qengine_, QUrl(QStringLiteral("qrc:///main.qml")));
    qroot_ = qcomponent_->create();

    logic_ = new GameLogic(qroot_);
    if (!logic_->init()) {
        qDebug() << "failed to init game logic";
        return 1;
    }

    logic_->change_state(LogicState::LS_MainMenu);
    register_delegates();

    QTimer qtimer;
    EventCaller event_caller;
    QObject::connect(&qtimer, SIGNAL(timeout()), &event_caller, SLOT(update()));
    qtimer.start(10);  // call event_caller every 10 ms

    return qapp->exec();
}

void GameApp::quit_delegate(const std::shared_ptr<EventDataBase> &event)
{
    (void) event;
    QGuiApplication::quit();
}

GameApp *GameApp::get()
{
    return g_app;
}

void GameApp::register_delegates()
{
    bs2::connection conn;

    conn = EventManager::get()->add_listener(
            boost::bind(&GameApp::quit_delegate, this, _1),
            EventData_Quit::event_type_);
    conn_list_.push_back(conn);
}
