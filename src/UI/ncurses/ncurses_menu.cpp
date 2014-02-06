#include "ncurses_menu.hpp"

namespace Quoridor {
namespace UI {

NcursesMenu::NcursesMenu(const std::vector<std::string> &items)
        : menu_(), items_(), item_num_(0)
{
    item_num_ = items.size();
    items_ = new ITEM*[item_num_ + 1];

    size_t i;
    for (i = 0; i < item_num_; ++i) {
        items_[i] = new_item(items[i].c_str(), items[i].c_str());
    }
    items_[i] = NULL;

    menu_ = new_menu(items_);
    set_menu_mark(menu_, " * ");
}

NcursesMenu::~NcursesMenu()
{
    unpost_menu(menu_);
    for (size_t i = 0; i < item_num_; ++i) {
        free_item(items_[i]);
    }
    delete[] items_;
    free_menu(menu_);
}

void NcursesMenu::up()
{
    menu_driver(menu_, REQ_UP_ITEM);
}

void NcursesMenu::down()
{
    menu_driver(menu_, REQ_DOWN_ITEM);
}

std::string NcursesMenu::item()
{
    ITEM *item = current_item(menu_);
    return item_name(item);
}

}  /* namespace UI */
}  /* namespace Quoridor */
