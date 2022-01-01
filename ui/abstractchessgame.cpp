#include "abstractchessgame.h"
#include "chesspieceitem.h"

void AbstractChessGame::performMove(const Chess::Move& move)
{
    status_ = Status::Running;
    if (resetFlag_)
    {
        newGame();
        return;
    }

    if (!game_.isLegalMove(move.src(), move.dst()))
    {
        throw 1;
    }

    game_.performMove(move);
    prevMove_ = move;
    emit movePerformed(move);

    updateVisual();
    status_ = Status::WaitingForAnimation;
}

void AbstractChessGame::nextMove()
{
    status_ = Status::Running;

    if (resetFlag_)
    {
        newGame();
    }
    else if (game_.isComplete())
    {
        status_ = Status::Stopped;
        finishGame();
        emit gameCompleted(game_.result(), game_.activePlayer());
    }
    else if (activePlayerType() == Chess::PlayerType::Human)
    {
        status_ = Status::WaitingForHumanMove;
        readyForHumanMove();
    }
    else
    {
        status_ = Status::WaitingForEngineMove;
        emit readyForEngineMove(previousMove());
    }
}

void AbstractChessGame::newGame()
{
    resetFlag_ = false;
    status_ = Status::Running;

    prevMove_ = Chess::Move();
    game_ = Chess::Game();
    emit gameStarted();

    updateVisual();
    status_ = Status::WaitingForAnimation;
}

void AbstractChessGame::reset()
{
    if (status_ == Status::WaitingForAnimation || status_ == Status::WaitingForEngineMove)
    {
        resetFlag_ = true;
    }
    else
    {
        newGame();
    }
}
