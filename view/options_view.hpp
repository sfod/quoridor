#pragma once

#include <QObject>
#include <QVariant>
#include <string>
#include <vector>
#include "qt_view.hpp"
#include "game/game_data.hpp"

class OptionsView : public QtView {
    Q_OBJECT

public:
    explicit OptionsView(QObject *qroot, QObject *qparent = 0);
    virtual ~OptionsView() = default;

    bool init();
    virtual void on_msg();
    virtual void on_update();
    virtual void attach(ActorId actor_id);

public slots:
    void button_start_game_clicked();
    void button_back_clicked();
    void on_players_changed(QVariant player_list);

private:
    virtual QObject *find_object_by_name(const char *name) const override;
    bool load_players_data();
    bool connect_options();
    void send_new_actors_data() const;

private:
    QObject *qroot_;
    QObject *qoptions_;
    ActorId actor_id_;
    std::vector<QString> player_types_;
    std::vector<int> player_nums_;
    std::vector<PlayerType> selected_players_;
};
