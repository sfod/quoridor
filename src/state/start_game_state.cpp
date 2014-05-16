#include "start_game_state.hpp"

#include "game_state.hpp"
#include "main_menu_state.hpp"
#include "player_factory.hpp"
#include "logger.hpp"
#include "exception.hpp"


static boost::log::sources::severity_logger<boost::log::trivial::severity_level> lg;

namespace Quoridor {

std::string StartGameState::name_("Start Game");

StartGameState::StartGameState(std::shared_ptr<StateManager> stm)
    : stm_(stm), win_(), plist2_win_(), plist4_win_(), cur_plist_win_(),
    plist_handlers_()
{
    init_win_();

    plist_handlers_.push_back({"two players", std::bind(&StartGameState::set_player_list2_, this)});
    plist_handlers_.push_back({"four players", std::bind(&StartGameState::set_player_list4_, this)});

    subscribe_for_events_();
    set_player_num_();
}

StartGameState::~StartGameState()
{
    BOOST_LOG_SEV(lg, boost::log::trivial::debug) << "destroying state " << name_;
}

void StartGameState::update()
{
}

std::shared_ptr<CEGUI::Window> StartGameState::window() const
{
    return win_;
}

const std::string &StartGameState::name() const
{
    return name_;
}

void StartGameState::init_win_()
{
    win_ = std::shared_ptr<CEGUI::Window>(
            CEGUI::WindowManager::getSingleton().
                    loadLayoutFromFile("start_game.layout"),
            [=](CEGUI::Window *w) {
                BOOST_LOG_SEV(lg, boost::log::trivial::debug) << "removing window " << w;
                CEGUI::WindowManager::getSingleton().destroyWindow(w);
            }
    );
    plist2_win_ = std::shared_ptr<CEGUI::DefaultWindow>(
            static_cast<CEGUI::DefaultWindow*>(
                CEGUI::WindowManager::getSingleton().
                        loadLayoutFromFile("start_game_player_list_2.layout")),
            [=](CEGUI::DefaultWindow *w) {
                BOOST_LOG_SEV(lg, boost::log::trivial::debug) << "removing window " << w;
                CEGUI::WindowManager::getSingleton().destroyWindow(w);
            }
    );
    init_player_list_(plist2_win_, 2);

    plist4_win_ = std::shared_ptr<CEGUI::DefaultWindow>(
            static_cast<CEGUI::DefaultWindow*>(
                CEGUI::WindowManager::getSingleton().
                        loadLayoutFromFile("start_game_player_list_4.layout")),
            [=](CEGUI::DefaultWindow *w) {
                BOOST_LOG_SEV(lg, boost::log::trivial::debug) << "removing window " << w;
                CEGUI::WindowManager::getSingleton().destroyWindow(w);
            }
    );
    init_player_list_(plist4_win_, 4);

    cur_plist_win_ = plist2_win_;
    plist4_win_->setVisible(false);

    win_->addChild(plist2_win_.get());
    win_->addChild(plist4_win_.get());
}

void StartGameState::init_player_list_(std::shared_ptr<CEGUI::DefaultWindow> win, size_t n)
{
    for (size_t i = 0; i < n; ++i) {
        std::string cb_name = "playerType" + std::to_string(i);
        auto ptype_win = static_cast<CEGUI::Combobox*>(win->getChild(cb_name));
        for (std::string ptype : PlayerFactory::types()) {
            auto item = new CEGUI::ListboxTextItem(ptype);
            ptype_win->addItem(item);
        }
        ptype_win->setItemSelectState(ptype_win->getListboxItemFromIndex(0), true);
    }
}

void StartGameState::set_player_num_()
{
    CEGUI::Combobox *cbpn = static_cast<CEGUI::Combobox*>(win_->getChild("playerNum"));

    for (auto plist_handler : plist_handlers_) {
        auto item = new CEGUI::ListboxTextItem(plist_handler.name);
        cbpn->addItem(item);
    }

    if (auto item = cbpn->getListboxItemFromIndex(0)) {
        cbpn->setItemSelectState(item, true);
        handle_player_num_(CEGUI::WindowEventArgs(cbpn));
    }
    else {
        throw Exception("failed to set player number");
    }
}

void StartGameState::subscribe_for_events_()
{
    win_->getChild("startGame")->subscribeEvent(
            CEGUI::Window::EventMouseClick,
            CEGUI::Event::Subscriber(
                    &StartGameState::handle_start_game_, this
            )
    );
    win_->getChild("returnToMainMenu")->subscribeEvent(
            CEGUI::Window::EventMouseClick,
            CEGUI::Event::Subscriber(
                    &StartGameState::handle_return_, this
            )
    );
    win_->getChild("playerNum")->subscribeEvent(
            CEGUI::Combobox::EventListSelectionAccepted,
            CEGUI::Event::Subscriber(
                    &StartGameState::handle_player_num_, this
            )
    );
}

bool StartGameState::handle_start_game_(const CEGUI::EventArgs &/* e */)
{
    BOOST_LOG_SEV(lg, boost::log::trivial::info) << "starting game";
    std::vector<std::string> ptypes;
    for (size_t i = 0; i < cur_plist_win_->getChildCount(); ++i) {
        auto ptype_win = static_cast<CEGUI::Combobox*>(cur_plist_win_->getChildAtIdx(i));
        ptypes.push_back(ptype_win->getText().c_str());
    }
    stm_->change_state(std::shared_ptr<IState>(new GameState(stm_, ptypes)));
    return true;
}

bool StartGameState::handle_return_(const CEGUI::EventArgs &/* e */)
{
    BOOST_LOG_SEV(lg, boost::log::trivial::info) << "returning to main menu";
    stm_->change_state(std::shared_ptr<IState>(new MainMenuState(stm_)));
    return true;
}

bool StartGameState::handle_player_num_(const CEGUI::EventArgs &e)
{
    BOOST_LOG_SEV(lg, boost::log::trivial::info) << "changing player number";

    const CEGUI::WindowEventArgs &we = static_cast<const CEGUI::WindowEventArgs&>(e);
    CEGUI::Combobox *cb = static_cast<CEGUI::Combobox*>(we.window);

    auto item = cb->getSelectedItem();
    const CEGUI::String &item_name = item->getText();
    auto it = plist_handlers_.get<1>().find(item_name);
    (it->handler)();

    return true;
}

void StartGameState::set_player_list2_()
{
    BOOST_LOG_SEV(lg, boost::log::trivial::debug) << "list2";
    plist2_win_->setVisible(true);
    plist4_win_->setVisible(false);
    cur_plist_win_ = plist2_win_;
}

void StartGameState::set_player_list4_()
{
    BOOST_LOG_SEV(lg, boost::log::trivial::debug) << "list4";
    plist2_win_->setVisible(false);
    plist4_win_->setVisible(true);
    cur_plist_win_ = plist4_win_;
}

}  /* namespace Quoridor */
