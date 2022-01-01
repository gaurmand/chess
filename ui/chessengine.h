#ifndef CHESSENGINE_H
#define CHESSENGINE_H

#include "chess/game.h"

#include <QObject>

#include <random>

class ChessEngine: public QObject
{
    Q_OBJECT
public:
    ChessEngine(const Chess::PlayerType wtype, const Chess::PlayerType btype);

public slots:
    void performMove(const Chess::Move& move);
    void selectMove();
    void reset();

signals:
    void moveSelected(const Chess::Move& move);

private:
    Chess::Move randomStrategy();
    Chess::Move shallowSearchStrategy();

    Chess::PlayerType activePlayerType() const { return playerTypes_[game_.activePlayer()]; }

    Chess::Game game_;
    std::array<Chess::PlayerType, NUM_PLAYERS> playerTypes_;
    std::default_random_engine rng_;
};

#endif // CHESSENGINE_H
