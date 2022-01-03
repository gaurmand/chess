#ifndef MINIMAX_H
#define MINIMAX_H

#include "../game.h"

class Minimax
{
public:
    Minimax(int depthLimit = 2): depthLimit_(depthLimit) {}
    Chess::Move evaluate(const Chess::FENState& state);
    typedef double ValueType;

private:
    ValueType maxValue(const Chess::FENState& state, const int depth);
    ValueType minValue(const Chess::FENState& state, const int depth);

    bool isCutoffDepth(const int depth) { return depth >= depthLimit_; }
    bool isTerminalNode(const Chess::Game& game) { return game.isComplete(); }
    ValueType evaluate(const Chess::Game& game);
    ValueType sumPieceValues(const std::vector<Chess::Piece>& pieces);
    Chess::Player maxPlayer() { return maxPlayer_; }
    Chess::Player minPlayer() { return maxPlayer_ == Chess::Player::White ? Chess::Player::Black : Chess::Player::White; }

    int depthLimit_;
    Chess::Player maxPlayer_;
};

#endif // MINIMAX_H
