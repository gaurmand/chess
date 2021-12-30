#include "playablechessgame.h"

void PlayableChessGame::performMove(const Chess::BP& src, const Chess::BP& dst)
{
    if (!game_.isLegalMove(src, dst))
    {
        emit moveFailed(Chess::Move(src, dst));
        return;
    }

    Chess::Move move = game_.move(src, dst);
    game_.performMove(move);
    emit movePerformed(move);

    prevMove_ = move;
    computeDefaultBPStates();
    emitDefaultBPStates();

    if (game_.isComplete())
    {
        emit gameCompleted(game_.result(), game_.activePlayer());
    }
    else
    {
        emit waitingForMove(game_.activePlayer(), activePlayerType());
    }
}

void PlayableChessGame::newGame()
{
    std::for_each(defautStates_.begin(), defautStates_.end(), [](auto& row){ row.fill(ui::BPInfo::NONE); });
    prevMove_ = Chess::Move();
    game_ = Chess::Game();
    emit gameStarted();
    emit waitingForMove(game_.activePlayer(), activePlayerType());
}

void PlayableChessGame::computeDefaultBPStates()
{
    std::for_each(defautStates_.begin(), defautStates_.end(), [](auto& row){ row.fill(ui::BPInfo::NONE); });

    // king square if in check
    if (game_.isInCheck())
    {
        const Chess::BP checkPos = game_.kingPosition(game_.activePlayer());
        defautStates_[checkPos.row()][checkPos.col()] |= ui::BPInfo::CHECK;
    }

    // previous move
    if (prevMove_.isValid())
    {
        const Chess::BP src = prevMove_.src();
        defautStates_[src.row()][src.col()] |= ui::BPInfo::PREV_SRC;

        const Chess::BP dst = prevMove_.dst();
        defautStates_[dst.row()][dst.col()] |= ui::BPInfo::PREV_DST;
    }
}

void PlayableChessGame::emitSelectedBPStates(const Chess::BP& src)
{
    ui::BPStates states = defautStates_;

    // move source square
    states[src.row()][src.col()] |= ui::BPInfo::SOURCE;

    // move squares (normal and captures)
    const auto moves = game_.moves(src);
    for (const auto& move: moves)
    {
        const Chess::BP dst = move.dst();
        if (move.type() == Chess::MoveType::Capture)
        {
            states[dst.row()][dst.col()] |= ui::BPInfo::CAPTURE;
        }
        else
        {
            states[dst.row()][dst.col()] |= ui::BPInfo::NORMAL_MOVE;
        }
    }
    emit updatedBoardStates(states);
}

void PlayableChessGame::emitDefaultBPStates()
{
    emit updatedBoardStates(defautStates_);
}
