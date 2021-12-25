#ifndef STATE_H
#define STATE_H

#include "boardpos.h"
#include "common.h"

#include <array>

namespace Chess
{

class State
{
public:
    State() {};
    State(const FENState& state);
    void setFENState(const FENState& state);
    FENState toFENState() const { return FENState();} // TODO

    Player activePlayer() const {return active_;}
    int numHalfMoves() const {return numHalfMoves_;}
    int numFullMoves() const {return numFullMoves_;}
    bool canShortCastle(Player player) const {return canShortCastle_[player];}
    bool canLongCastle(Player player) const {return canLongCastle_[player];}
    bool canEnPassant() const {return canEnPassant_;}
    BP enPassantPosition() const {return enPassantPosition_;}

    void setActivePlayer(Player player) {active_ = player;}
    void switchActivePlayer() {active_ = active_ == Player::White ? Player::Black : Player::White;}

protected:
    int numHalfMoves_ = 0;
    int numFullMoves_ = 1;
    Player active_ = Player::White;
    std::array<bool, NUM_PLAYERS> canShortCastle_ = {true, true};
    std::array<bool, NUM_PLAYERS> canLongCastle_ = {true, true};
    bool canEnPassant_= false;
    BP enPassantPosition_ = {-1, -1};
};

std::ostream& operator<<(std::ostream&, const State&);


}; // namespace Chess

#endif // STATE_H
