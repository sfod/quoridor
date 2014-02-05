#include "ui_factory.hpp"
#include "exception.hpp"
#include "ncurses/ncurses_ui.hpp"

namespace Quoridor {
namespace UI {

UIFactory::UIFactory()
{
}

UIFactory::~UIFactory()
{
}

std::shared_ptr<UI> UIFactory::make_ui(const std::string &ui_type) const
{
    if (ui_type == "ncurses") {
        return std::shared_ptr<UI>(new NcursesUI);
    }
    throw Exception("Invalid UI type");
}

}  /* namespace UI */
}  /* namespace Quoridor */
