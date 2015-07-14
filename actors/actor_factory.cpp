#include <QDebug>  // include it first to avoid error with boost::Q_FOREACH
#include "actor_factory.hpp"

#include <QJsonDocument>
#include <QJsonObject>

#include "game/game_app.hpp"
#include "graph_component.hpp"
#include "wall_component.hpp"
#include "ai_component.hpp"


static ActorId g_actor_id = 0;

std::shared_ptr<Actor> ActorFactory::create_actor(QString &resource_file,
        const std::vector<QString> &component_resource_files)
{
    QFile resource(resource_file);
    if (!resource.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "failed to open file: " << resource.errorString();
        return std::shared_ptr<Actor>();
    }

    QJsonDocument jd = QJsonDocument::fromJson(resource.readAll());
    QJsonObject player = jd.object();
    QJsonObject actor_data = player["actor"].toObject();

    std::shared_ptr<Actor> actor(new Actor(++g_actor_id));
    if (!actor->init(actor_data)) {
        return std::shared_ptr<Actor>();
    }

    QJsonObject actor_components = actor_data["components"].toObject();
    if (!actor_components.isEmpty()) {
        for (const auto &component_type : actor_components.keys()) {
            std::shared_ptr<ActorComponent> component = create_actor_component(
                        component_type,
                        actor_components[component_type].toObject());
            if (component) {
                actor->add_component(component);
                component->set_owner(actor);
            }
        }
    }

    for (const auto component_resource_file : component_resource_files) {
        QFile component_resource(component_resource_file);
        if (!component_resource.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qDebug() << "failed to open file" << component_resource_file << ":"
                     << component_resource.errorString();
            return std::shared_ptr<Actor>();
        }

        QJsonDocument jd = QJsonDocument::fromJson(component_resource.readAll());
        QJsonObject component = jd.object();

        QJsonObject actor_components = component["components"].toObject();
        if (!actor_components.isEmpty()) {
            for (const auto &component_type : actor_components.keys()) {
                std::shared_ptr<ActorComponent> component = create_actor_component(
                            component_type,
                            actor_components[component_type].toObject());
                if (component) {
                    actor->add_component(component);
                    component->set_owner(actor);
                }
            }
        }

    }

    actor->post_init();

    GameApp::get()->game_logic()->actor_keeper()->add_actor(actor);
    return actor;
}

std::shared_ptr<ActorComponent> ActorFactory::create_actor_component(
        const QString &type, const QJsonObject &component_data)
{
    if (type == "GraphComponent") {
        std::shared_ptr<ActorComponent> component(new GraphComponent);
        if (!component->init(component_data)) {
            return std::shared_ptr<ActorComponent>();
        }
        return component;
    }
    else if (type == "AIComponent") {
        auto component = std::make_shared<AIComponent>();
        if (!component->init(component_data)) {
            return std::shared_ptr<ActorComponent>();
        }
        return component;
    }
    else if (type == "WallComponent") {
        auto component = std::make_shared<WallComponent>();
        if (!component->init(component_data)) {
            return std::shared_ptr<ActorComponent>();
        }
        return component;
    }
    else {
        return std::shared_ptr<ActorComponent>();
    }
}
