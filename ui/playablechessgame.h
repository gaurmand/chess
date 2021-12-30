#ifndef PLAYABLECHESSGAME_H
#define PLAYABLECHESSGAME_H

#include "chess/game.h"
#include "ui.h"

#include <QObject>

#include <array>

class PlayableChessGame: public QObject
{
    Q_OBJECT
public:
    PlayableChessGame(const Chess::PlayerType wtype, const Chess::PlayerType btype)
        : playerTypes_{wtype, btype} {}

    const Chess::Game& game() { return game_; }

public slots:
    void performMove(const Chess::BP& src, const Chess::BP& dst);
    void newGame();
    void emitSelectedBPStates(const Chess::BP& selectedPos);
    void emitDefaultBPStates();

signals:
    void movePerformed(const Chess::Move& move);
    void moveFailed(const Chess::Move& move);
    void waitingForMove(const Chess::Player player, const Chess::PlayerType playerType);
    void gameStarted();
    void gameCompleted(const Chess::ResultType result, const Chess::Player lastPlayer);
    void updatedBoardStates(const ui::BPStates& states);

private:
    Chess::PlayerType activePlayerType() const { return playerTypes_[game_.activePlayer()]; }
    void computeDefaultBPStates();

    Chess::Game game_;
    Chess::Move prevMove_;
    std::array<Chess::PlayerType, NUM_PLAYERS> playerTypes_;
    ui::BPStates defautStates_;
};

#endif // PLAYABLECHESSGAME_H
