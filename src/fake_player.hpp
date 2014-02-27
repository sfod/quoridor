#include <memory>

#include <boost/random/mersenne_twister.hpp>

#include "iplayer.hpp"
#include "board.hpp"
#include "pawn.hpp"


namespace Quoridor {

class FakePlayer : public IPlayer {
public:
    FakePlayer(std::shared_ptr<Board> board, std::shared_ptr<Pawn> pawn);
    virtual ~FakePlayer();

    virtual IMove *get_move();
    virtual bool is_interactive() const { return false; };

private:
    std::shared_ptr<Board> board_;
    std::shared_ptr<Pawn> pawn_;
    std::vector<int> fin_nodes_;
    boost::random::mt19937 gen_;
};

}  /* namespace Quoridor */
