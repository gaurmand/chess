#ifndef POSITION_H
#define POSITION_H

#include "board.h"
#include "state.h"
#include "common.h"

namespace Chess
{

class Position: public Board, public State
{
public:
    Position(): Board(INIT_BOARD), State() {}
    Position(const FENState& state);
    void setFENState(const FENState& state);
    FENState toFENState() const { return Board::toFENState() + " " + State::toFENState(); }
    static bool isValidFENState(const FENState& state);

    Position save() const;
    void restore(const Position& pos);
};

}; // namespace Chess

#endif // POSITION_H
