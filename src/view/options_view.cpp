#include "options_view.hpp"
#include "game/game_data.hpp"
#include "events/event_data_main_menu.hpp"
#include "events/event_data_game.hpp"
#include "events/event_data_request_new_actor.hpp"
#include "exceptions/exception.hpp"


OptionsView::OptionsView(QObject *qroot, std::shared_ptr<EventManager> event_manager)
    : QtView(NULL), qroot_(qroot), qoptions_(), actor_id_(-1),
      event_manager_(event_manager), player_types_(), player_nums_(), selected_players_()
{
    load_players_data();
    connect_options();

    QVariantList types;
    for (const auto &type : player_types_) {
        types << type;
    }
    QMetaObject::invokeMethod(qoptions_, "setPlayerTypes",
            Q_ARG(QVariant, QVariant::fromValue(types)));

    QVariantList nums;
    for (int num : player_nums_) {
        nums << num;
    }
    QMetaObject::invokeMethod(qoptions_, "setPlayerNums",
            Q_ARG(QVariant, QVariant::fromValue(nums)));

    connect_button("buttonStartGame", SLOT(button_start_game_clicked()));
    connect_button("buttonBackToMainMenu", SLOT(button_back_clicked()));
}

void OptionsView::on_msg()
{
}

void OptionsView::on_update()
{
}

void OptionsView::attach(ActorId actor_id)
{
    actor_id_ = actor_id;
}

void OptionsView::button_start_game_clicked()
{
    send_new_actors_data();

    auto event = std::make_shared<EventData_Game>();
    if (!event_manager_->queue_event(event)) {
        qDebug() << "failed to queue Game event";
    }
}

void OptionsView::button_back_clicked()
{
    auto event = std::make_shared<EventData_MainMenu>();
    if (!event_manager_->queue_event(event)) {
        qDebug() << "failed to queue MainMenu event";
    }
}

void OptionsView::on_players_changed(QVariant player_list)
{
    QVariantList lst = player_list.toList();
    selected_players_.clear();
    for (QVariant ptype_str : lst) {
        PlayerType ptype = str_to_player_type.at(ptype_str.toString());
        selected_players_.push_back(ptype);
    }
}

QObject *OptionsView::find_object_by_name(const char *name) const
{
    return qroot_->findChild<QObject*>(name);
}

void OptionsView::load_players_data()
{
    QFile players_file;
    players_file.setFileName(":/configs/players.json");
    if (!players_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        throw open_file_error();
    }

    QJsonDocument jd = QJsonDocument::fromJson(players_file.readAll());
    QJsonObject players = jd.object();  // FIXME check if jd is an object

    QJsonArray player_types = players["types"].toArray();
    if (player_types.isEmpty()) {
        throw invalid_json_error();
    }

    for (auto type : player_types) {
        player_types_.push_back(type.toString());
    }

    QJsonArray player_nums = players["nums"].toArray();
    if (player_nums.isEmpty()) {
        throw invalid_json_error();
    }

    for (auto num : player_nums) {
        player_nums_.push_back(num.toInt());
    }
}

void OptionsView::connect_options()
{
    qoptions_ = find_object_by_name("options");
    if (!qoptions_) {
        throw qml_missing_element_error();
    }
    if (!QObject::connect(
            qoptions_, SIGNAL(playersChanged(QVariant)),
            this, SLOT(on_players_changed(QVariant)))) {
        throw qml_connect_error();
    }
}

void OptionsView::send_new_actors_data() const
{
    for (PlayerType ptype : selected_players_) {
        auto ev = std::make_shared<EventData_RequestNewActor>(ptype);
        event_manager_->queue_event(ev);
    }
}
