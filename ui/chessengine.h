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
    void selectMove(const Chess::Player player, const Chess::PlayerType playerType);
    void reset();

signals:
    void moveSelected(const Chess::BP& src, const Chess::BP& dst);

private:
    Chess::Move randomStrategy();

    Chess::Game game_;
    std::array<Chess::PlayerType, NUM_PLAYERS> playerTypes_;
    std::default_random_engine rng_;
};

#endif // CHESSENGINE_H
