#pragma once

#include <QObject>
#include <QVariant>
#include <string>
#include <vector>
#include "qt_view.hpp"
#include "game/game_data.hpp"
#include "events/event_manager.hpp"

class OptionsView : public QtView {
    Q_OBJECT

public:
    explicit OptionsView(QObject *qroot, const std::shared_ptr<EventManager> &event_manager);

    virtual void on_msg() override;
    virtual void on_update() override;
    virtual void attach(ActorId actor_id) override;

public slots:
    void button_start_game_clicked();
    void button_back_clicked();
    void on_players_changed(QVariant player_list);

private:
    virtual QObject *find_object_by_name(const char *name) const override;
    void load_players_data();
    void connect_options();
    void send_new_actors_data() const;

private:
    QObject *qroot_;
    QObject *qoptions_;
    ActorId actor_id_;
    std::shared_ptr<EventManager> event_manager_;
    std::vector<QString> player_types_;
    std::vector<int> player_nums_;
    std::vector<PlayerType> selected_players_;
};
