#include "runner.hpp"

#include <boost/lexical_cast.hpp>

#include "exception.hpp"
#include "walk_move.hpp"
#include "wall_move.hpp"

namespace Quoridor {
namespace UI {

Runner::Runner()
    : game_(std::shared_ptr<Board>(new Board(9, 9))), players_(), ui_()
{
}

Runner::~Runner()
{
}

void Runner::create_ui(UIFactory &uif, const std::string &ui_type)
{
    ui_ = uif.make_ui_impl(ui_type);
    ui_->draw_window();
}

void Runner::run()
{
    bool is_run = true;
    IMove *move;
    pos_t start_pos;
    pos_t end_pos;
    int rc;

    if (players_.size() == 0) {
        throw Exception("players are not set");
    }

    init_board_repr();

    // main loop
    while (is_run) {
        for (size_t i = 0; i < players_.size(); ++i) {
            auto pawn = game_.pawn_list().at(i);
            auto player = players_[i];

            start_pos = game_.pawn_pos(pawn);

            while (true) {
                move = player->get_move();

                rc = game_.make_move(move, pawn);

                if (rc == 0) {
                    break;
                }
                // player should continue turn
                else if (rc == -2) {
                    continue;
                }
                // invalid move, player should make another move
                else if (rc == -1) {
                    continue;
                }
                // not possible
                else {
                    throw Exception("unknown return code from make_move :"
                            + boost::lexical_cast<std::string>(rc));
                }
            }

            if (dynamic_cast<WalkMove *>(move)) {
                end_pos = game_.pawn_pos(pawn);
                redraw_pawn(pawn->color()[0], start_pos, end_pos);
            }
            else if (WallMove *wall_move = dynamic_cast<WallMove *>(move)) {
                const Wall &wall = wall_move->wall();
                draw_wall(wall);
            }

            ui_->update(repr_);

            if (game_.is_win(pawn)) {
                is_run = false;
                break;
            }

            usleep(500000);
        }
    }
}

void Runner::set_player(int i, std::shared_ptr<IPlayer> player)
{
    static std::string colors[] = {"red", "green", "blue", "yellow"};
    players_.push_back(player);
    game_.add_pawn(std::shared_ptr<Pawn>(new Pawn(colors[i])));
}

void Runner::init_board_repr() const
{
    repr_.resize(19);
    for (int i = 0; i < 19; ++i) {
        repr_[i].resize(19);
        for (int j = 0; j < 19; ++j) {
            if (i % 2 == 1) {
                repr_[i][j] = (j % 2 == 0 ? '|' : ' ');
            }
            else {
                repr_[i][j] = (j % 2 == 0 ? ' ' : '_');
            }
        }
    }

    for (size_t i = 0; i < players_.size(); ++i) {
        auto pawn = game_.pawn_list().at(i);
        pos_t pos = game_.pawn_pos(pawn);
        repr_[pos.row * 2 + 1][pos.col * 2 + 1] = pawn->color()[0];
    }
}

void Runner::redraw_pawn(char p, const pos_t &old_pos, const pos_t &new_pos) const
{
    repr_[old_pos.row * 2 + 1][old_pos.col * 2 + 1] = ' ';
    repr_[new_pos.row * 2 + 1][new_pos.col * 2 + 1] = p;
}

void Runner::draw_wall(const Wall &wall) const
{
    if (wall.orientation() == 0) {
        for (int i = 0; i < wall.cnt(); ++i) {
            repr_[wall.line() * 2 + 2][(wall.start_pos() + i) * 2 + 1] = '=';
        }
    }
    else {
        for (int i = 0; i < wall.cnt(); ++i) {
            repr_[(wall.start_pos() + i) * 2 + 1][wall.line() * 2 + 2] = '$';
        }
    }
}

}  /* namespace UI */
}  /* namespace Quoridor */
