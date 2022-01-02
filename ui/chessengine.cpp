#include "chessengine.h"

#include <QTimer>

#include <random>
#include <algorithm>

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
        emit moveSelected(shallowSearchStrategy());
    });
}

void ChessEngine::reset()
{
    game_ = Chess::Game();
}

Chess::Move ChessEngine::randomStrategy()
{
    const std::vector<Chess::Move> moves = game_.moves();
    std::uniform_int_distribution<int> dist(0, moves.size()-1);
    const int randMoveIndex = dist(rng_);
    return moves[randMoveIndex];
}

Chess::Move ChessEngine::shallowSearchStrategy()
{
    static const auto evaluatePosition = [&]()
    {
      Chess::Player active = game_.activePlayer();
      Chess::Player inactive = (active == Chess::Player::White ? Chess::Player::Black : Chess::Player::White);

      double score = 0;
      std::vector<Chess::Piece> pieces = game_.pieces(active);
      for (const auto& piece: pieces)
      {
          score -= pieceValue(piece.type());
      }

      pieces = game_.pieces(inactive);
      for (const auto& piece: pieces)
      {
          score += pieceValue(piece.type());
      }

      if (game_.isInCheck())
      {
          score += 0.5;
      }
      return score;
    };

    static const auto evaluateMove = [&](const Chess::Move& evalMove)
    {
        auto save = game_;
        game_.performMove(evalMove);
        double score = evaluatePosition();
        const std::vector<Chess::Move> moves = game_.moves();
        for (const auto& move: moves)
        {
            if (move.dst() == evalMove.dst())
            {
                score -= pieceValue(game_.piece(evalMove.dst()).type());
                break;
            }
        }
        game_ = save;

        return score;
    };

    std::vector<Chess::Move> moves = game_.moves();
    std::vector<double> evaluation(moves.size());

    std::transform(moves.begin(), moves.end(), evaluation.begin(), evaluateMove);

    std::vector<int> maxIndices;
    const double maxScore = *std::max_element(evaluation.begin(), evaluation.end());

    std::vector<Chess::Move> maxMoves;
    for(int i = 0; i < moves.size(); i++)
    {
        if (evaluation[i] == maxScore)
        {
            maxMoves.push_back(moves[i]);
        }
    }

    std::uniform_int_distribution<int> dist(0, maxMoves.size() - 1);
    const int randMoveIndex = dist(rng_);
    return maxMoves[randMoveIndex];
}

double ChessEngine::pieceValue(const Chess::PieceType type)
{
    switch(type) {
        case Chess::PieceType::King:
            return 10.0;
        case Chess::PieceType::Queen:
            return 9.0;
        case Chess::PieceType::Rook:
            return 5.0;
        case Chess::PieceType::Bishop:
            return 3.0;
        case Chess::PieceType::Knight:
            return 3.0;
        case Chess::PieceType::Pawn:
            return 1.0;
        default:
            throw 1;
    }
}
