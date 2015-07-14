#include "game_app.hpp"

#include <QQmlEngine>
#include <QQmlComponent>
#include <QTimer>
#include <QDebug>

#include "events/event_manager.hpp"
#include "events/event_caller.hpp"
#include "graph/wall.hpp"
#include "exceptions/exception.hpp"

static GameApp *g_app;

GameApp::GameApp(int argc, char **argv) : qapp_(argc, argv), qengine_(),
    qcomponent_(&qengine_), conn_list_()
{
    WallEnumClass::declareQML();
    qcomponent_.loadUrl(QUrl(QStringLiteral("qrc:///main.qml")));
    if (qcomponent_.isError()) {
        for (auto qerr : qcomponent_.errors()) {
            qDebug() << qerr;
        }
        throw qml_error();
    }

    qroot_ = qcomponent_.create();

    event_manager_ = std::make_shared<EventManager>();
    logic_ = std::make_shared<GameLogic>(qroot_);

    g_app = this;
}

GameApp::~GameApp()
{
    for (auto conn : conn_list_) {
        conn.disconnect();
    }
    delete qroot_;
}

int GameApp::run()
{
    logic_->change_state(LogicState::LS_MainMenu);
    register_delegates();

    QTimer qtimer;
    EventCaller event_caller;
    QObject::connect(&qtimer, SIGNAL(timeout()), &event_caller, SLOT(update()));
    qtimer.start(10);  // call event_caller every 10 ms

    return qapp_.exec();
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
