#include "ui_factory.hpp"
#include "exception.hpp"
#include "ncurses_ui_impl.hpp"

namespace Quoridor {
namespace UI {

UIFactory::UIFactory()
{
}

UIFactory::~UIFactory()
{
}

std::shared_ptr<UIImpl> UIFactory::make_ui_impl(const std::string &ui_type) const
{
    if (ui_type == "ncurses") {
        return std::shared_ptr<UIImpl>(new NcursesUIImpl);
    }

    throw Exception("Invalid UI type");
}

}  /* namespace UI */
}  /* namespace Quoridor */
