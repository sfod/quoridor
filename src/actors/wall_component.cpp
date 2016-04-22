#include "wall_component.hpp"

template<> const char *WallComponent::Base::name_ = "WallComponent";

WallComponent::WallComponent(std::shared_ptr<Graph> graph, const QJsonObject &component_data) : wall_num_(0)
{
    wall_num_ = component_data["number"].toInt();
}
