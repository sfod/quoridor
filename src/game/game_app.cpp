#include "game_app.hpp"

#include <QQmlEngine>
#include <QQmlComponent>
#include <QTimer>

#include "events/event_manager.hpp"
#include "events/event_caller.hpp"
#include "events/event_data_quit.hpp"
#include "graph/wall.hpp"
#include "exceptions/exception.hpp"

GameApp::GameApp(int argc, char **argv) : qapp_(argc, argv), qengine_(),
    qcomponent_(&qengine_)
{
    WallEnumClass::declareQML();
    qcomponent_.loadUrl(QUrl(QStringLiteral("qrc:///qml/main.qml")));
    if (qcomponent_.isError()) {
        for (auto qerr : qcomponent_.errors()) {
            qDebug() << qerr;
        }
        throw qml_error();
    }

    qroot_ = qcomponent_.create();

    event_manager_ = std::make_shared<EventManager>();
    logic_ = std::make_shared<GameLogic>(qroot_);
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

void GameApp::quit_delegate(const std::shared_ptr<EventData> &event)
{
    (void) event;
    QGuiApplication::quit();
}

void GameApp::register_delegates()
{
    EventManager::get()->add_listener(this,
            std::bind(&GameApp::quit_delegate, this, std::placeholders::_1),
            EventData_Quit::static_event_type());
}
