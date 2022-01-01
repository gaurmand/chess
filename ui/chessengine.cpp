#include "chessengine.h"

#include <QTimer>

#include <random>

ChessEngine::ChessEngine(const Chess::PlayerType wtype, const Chess::PlayerType btype)
    : playerTypes_{wtype, btype}, rng_(std::random_device()())
{
}

void ChessEngine::performMove(const Chess::Move& move)
{
    game_.performMove(move);
}

void ChessEngine::selectMove()
{
    if (activePlayerType() != Chess::PlayerType::AI)
    {
        return;
    }

    QTimer::singleShot(500, [=](){
        emit moveSelected(randomStrategy());
    });
}


void ChessEngine::reset()
{
    game_ = Chess::Game();
}

Chess::Move ChessEngine::randomStrategy()
{
    const std::vector<Chess::Piece> pieces = game_.pieces(game_.activePlayer());
    std::uniform_int_distribution<int> dist1(0, pieces.size()-1);

    const int randPieceIndex = dist1(rng_);
    const std::vector<Chess::Move> moves = game_.moves(pieces[randPieceIndex].pos());

    if (moves.size() == 0)
    {
        return randomStrategy();
    }

    std::uniform_int_distribution<int> dist2(0, moves.size()-1);
    const int randMoveIndex = dist2(rng_);

    return moves[randMoveIndex];
}
