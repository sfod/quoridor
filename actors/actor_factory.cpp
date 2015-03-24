#include <QDebug>  // include it first to avoid error with boost::Q_FOREACH
#include "actor_factory.hpp"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "game/game_app.hpp"
#include "graph_component.hpp"
#include "ai_component.hpp"

static ActorId g_actor_id = 0;

ActorFactory::ActorFactory()
{
}

std::shared_ptr<Actor> ActorFactory::create_actor(const std::string &resource,
        const std::vector<std::string> &component_resources)
{
    boost_pt::ptree pt;
    try {
        boost_pt::read_json(resource, pt);
    }
    catch (boost_pt::ptree_error &e) {
        qDebug() << "failed to open resource file:" << e.what();
        return std::shared_ptr<Actor>();
    }

    boost::optional<boost_pt::ptree &> actor_data = pt.get_child_optional("actor");
    if (!actor_data) {
        return std::shared_ptr<Actor>();
    }

    std::shared_ptr<Actor> actor(new Actor(++g_actor_id));
    if (!actor->init(*actor_data)) {
        return std::shared_ptr<Actor>();
    }

    boost::optional<boost_pt::ptree &> actor_components =
            pt.get_child_optional("actor.components");
    if (actor_components) {
        for (auto component_data : *actor_components) {
            std::shared_ptr<ActorComponent> component =
                    create_actor_component(component_data.first, component_data.second);
            if (component) {
                actor->add_component(component);
                component->set_owner(actor);
            }
        }
    }

    for (const std::string &component_resource : component_resources) {
        boost_pt::ptree pt;
        try {
            boost_pt::read_json(component_resource, pt);
        }
        catch (boost_pt::ptree_error &e) {
            qDebug() << "failed to open resource file:" << e.what();
            return std::shared_ptr<Actor>();
        }

        boost::optional<boost_pt::ptree &> actor_components =
                pt.get_child_optional("components");
        if (actor_components) {
            for (auto component_data : *actor_components) {
                std::shared_ptr<ActorComponent> component =
                        create_actor_component(component_data.first, component_data.second);
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
        const std::string &type, const boost_pt::ptree &component_data)
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
    else {
        return std::shared_ptr<ActorComponent>();
    }
}
