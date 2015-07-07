#include "wall_component.hpp"

const char *WallComponent::name_ = "WallComponent";

WallComponent::WallComponent() : wall_num_(0)
{
}

bool WallComponent::init(const QJsonObject &component_data)
{
    // TODO add checks
    wall_num_ = component_data["number"].toInt();
    return true;
}

void WallComponent::post_init()
{
}
