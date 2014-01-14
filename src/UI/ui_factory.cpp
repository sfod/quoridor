#include "ui_factory.hpp"
#include "ncurses_ui_impl.hpp"

namespace Quoridor {
namespace UI {

UIFactory::UIFactory()
{
}

UIFactory::~UIFactory()
{
}

std::shared_ptr<UIImpl> UIFactory::make_ui_impl() const
{
    return std::shared_ptr<UIImpl>(new NcursesUIImpl);
}

}  /* namespace UI */
}  /* namespace Quoridor */
