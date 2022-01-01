#ifndef ABSTRACTCHESSGAME_H
#define ABSTRACTCHESSGAME_H

#include "chess/game.h"
#include "ui.h"

#include <QObject>

#include <array>

class AbstractChessGame: public QObject
{
    Q_OBJECT
public:
    AbstractChessGame(const Chess::PlayerType wtype, const Chess::PlayerType btype)
        : playerTypes_{wtype, btype} {}

public slots:
    void newGame();
    void performMove(const Chess::Move& move);

protected:
    Chess::Player activePlayer() { return game_.activePlayer(); }
    Chess::Player inactivePlayer() { return game_.activePlayer() == Chess::Player::White ?  Chess::Player::Black :  Chess::Player::White; }
    Chess::PlayerType activePlayerType() const { return playerTypes_[game_.activePlayer()]; }
    const Chess::Game& game() const { return game_; }
    Chess::Move previousMove() const { return prevMove_; }
    void nextMove();

    virtual void readyForHumanMove() = 0;
    virtual void finishGame() = 0;
    virtual void updateVisual() = 0;

signals:
    void movePerformed(const Chess::Move& move);
    void readyForEngineMove(const Chess::Move& prevMove);
    void gameStarted();
    void gameCompleted(const Chess::ResultType result, const Chess::Player lastPlayer);

private:

    Chess::Game game_;
    Chess::Move prevMove_;
    std::array<Chess::PlayerType, NUM_PLAYERS> playerTypes_;
};

#endif // ABSTRACTCHESSGAME_H
