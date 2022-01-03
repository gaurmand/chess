#include "minimax.h"

#include <algorithm>
#include <cassert>
#include <limits>

static const int kInitialDepth = 1;

static const Minimax::ValueType kStalemateValue = 0;
static const Minimax::ValueType kCheckmateWinValue = 1000;
static const Minimax::ValueType kCheckmateLossValue = -1000;

static const Minimax::ValueType kMaxValue = std::numeric_limits<double>::infinity();
static const Minimax::ValueType kMinValue = -std::numeric_limits<double>::infinity();

Chess::Move Minimax::evaluate(const Chess::FENState& state)
{
    Chess::Game game(state);
    maxPlayer_ = game.activePlayer();
    const auto moves = game.moves();

    if (moves.size() <= 0)
    {
        return Chess::Move();
    }

    Chess::Move max;
    ValueType currMax = kMinValue;
    for (const auto& move: moves)
    {
        game.performMove(move);
        const ValueType value = minValue(game.toFENState(), kInitialDepth);

        if (value > currMax)
        {
            currMax = value;
            max = move;
        }
        game.setFENState(state);
    }

    return max;
}

Minimax::ValueType Minimax::maxValue(const Chess::FENState& state, const int depth)
{
    Chess::Game game(state);

    if (isCutoffDepth(depth) || isTerminalNode(game))
    {
        return evaluate(game);
    }

    const auto moves = game.moves();
    assert(moves.size() > 0);

    ValueType max = kMinValue;
    for (const auto& move: moves)
    {
        game.performMove(move);
        max = std::max(max, minValue(game.toFENState(), depth + 1));
        game.setFENState(state);
    }
    return max;
}

Minimax::ValueType Minimax::minValue(const Chess::FENState& state, const int depth)
{
    Chess::Game game(state);

    if (isCutoffDepth(depth) || isTerminalNode(game))
    {
        return evaluate(game);
    }

    const auto moves = game.moves();
    assert(moves.size() > 0);

    ValueType min = kMaxValue;
    for (const auto& move: moves)
    {
        game.performMove(move);
        min = std::min(min, maxValue(game.toFENState(), depth + 1));
        game.setFENState(state);
    }
    return min;
}

Minimax::ValueType Minimax::evaluate(const Chess::Game& game)
{
    if (game.isComplete())
    {
        if (game.result() == Chess::ResultType::Stalemate)
        {
            return kStalemateValue;
        }
        else if (game.activePlayer() == maxPlayer_)
        {
            return kCheckmateLossValue;
        }
        else
        {
            return kCheckmateWinValue;
        }
    }

    ValueType sum = 0;
    sum += sumPieceValues(game.pieces(maxPlayer()));
    sum -= sumPieceValues(game.pieces(minPlayer()));
    return sum;
}

Minimax::ValueType Minimax::sumPieceValues(const std::vector<Chess::Piece>& pieces)
{
    ValueType sum = 0;
    for (const auto& piece: pieces)
    {
        switch(piece.type()) {
            case Chess::PieceType::King:
                sum += 0.0;
                break;
            case Chess::PieceType::Queen:
                sum += 9.0;
                break;
            case Chess::PieceType::Rook:
                sum += 5.0;
                break;
            case Chess::PieceType::Bishop:
                sum += 3.0;
                break;
            case Chess::PieceType::Knight:
                sum += 3.0;
                break;
            case Chess::PieceType::Pawn:
                sum += 1.0;
                break;
            default:
                throw 1;
        }
    }
    return sum;
}

