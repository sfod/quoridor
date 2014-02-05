#include "ncurses_ui.hpp"
#include "ncurses_window.hpp"
#include "ncurses_menu.hpp"

namespace Quoridor {
namespace UI {

NcursesUI::NcursesUI()
{
}

NcursesUI::~NcursesUI()
{
}

std::shared_ptr<Window> create_window()
{
    return std::shared_ptr<Window>(new NcursesWindow());
}

std::shared_ptr<Menu> create_menu(const std::vector<std::string> &items)
{
    return std::shared_ptr<Menu>(new NcursesMenu(items));
}

}  /* namespace UI */
}  /* namespace Quoridor */
