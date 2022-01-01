#include "abstractchessgame.h"
#include "chesspieceitem.h"

void AbstractChessGame::performMove(const Chess::Move& move)
{
    if (!game_.isLegalMove(move.src(), move.dst()))
    {
        throw 1;
    }

    game_.performMove(move);
    prevMove_ = move;
    emit movePerformed(move);
    updateVisual();
}

void AbstractChessGame::nextMove()
{
    if (game_.isComplete())
    {
        finishGame();
        emit gameCompleted(game_.result(), game_.activePlayer());
    }
    else if (activePlayerType() == Chess::PlayerType::Human)
    {
        readyForHumanMove();
    }
    else
    {
        emit readyForEngineMove(previousMove());
    }
}

void AbstractChessGame::newGame()
{
    prevMove_ = Chess::Move();
    game_ = Chess::Game();
    emit gameStarted();
    updateVisual();
    nextMove();
}
