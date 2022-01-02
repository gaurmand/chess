#include "state.h"

#include <string>
#include <sstream>

namespace Chess
{

State::State(const FENState& state): State()
{
    setFENState(state);
}

void State::setFENState(const FENState& state)
{
    std::istringstream fen(state);

    // set active player
    char active;
    if (!(fen >> active))
    {
        throw 1;
    }

    active_ = active == 'w' ?  Player::White :  Player::Black;

    // set castling rights
    std::string castling;
    if (!(fen >> castling))
    {
        throw 1;
    }

    if (castling == "-")
    {
        canShortCastle_[Player::White] = false;
        canLongCastle_[Player::White]  = false;
        canShortCastle_[Player::Black] = false;
        canLongCastle_[Player::Black]  = false;
    }
    else
    {
        canShortCastle_[Player::White] = castling.find('K') != std::string::npos;
        canLongCastle_[Player::White]  = castling.find('Q') != std::string::npos;
        canShortCastle_[Player::Black] = castling.find('k') != std::string::npos;
        canLongCastle_[Player::Black]  = castling.find('q') != std::string::npos;
    }

    // set en passant
    std::string enPassant;
    if (!(fen >> enPassant))
    {
        throw 1;
    }
    BP enPassantPos = BP(enPassant);
    if (enPassantPos.isValid())
    {
        canEnPassant_ = true;
        enPassantPosition_ = enPassantPos;
    }
    else
    {
        canEnPassant_ = false;
        enPassantPosition_ = {-1, -1};
    }

    // set halfmove clock
    int halfmoveClock;
    if (!(fen >> halfmoveClock))
    {
        throw 1;
    }
    numHalfMoves_ = halfmoveClock;

    // set fullmove clock
    int fullmoveClock;
    if (!(fen >> fullmoveClock))
    {
        throw 1;
    }
    numFullMoves_ = fullmoveClock;
}

FENState State::toFENState() const
{
    std::ostringstream fen;

    // active player
    fen << (active_ == Player::White ? "w " : "b ");

    // castling rights
    int numCastles = 0;
    if (canShortCastle_[Player::White])
    {
        fen << "K";
        numCastles++;
    }
    if (canLongCastle_[Player::White])
    {
        fen << "Q";
        numCastles++;
    }
    if (canShortCastle_[Player::Black])
    {
        fen << "k";
        numCastles++;
    }
    if (canLongCastle_[Player::Black])
    {
        fen << "q";
        numCastles++;
    }
    fen << (numCastles > 0 ? " " : "- ");

    // en passant
    fen << (canEnPassant_ ? enPassantPosition_.toANBP() + " " : "- ");

    // half move clock
    fen << numHalfMoves_ << " ";

    // full move clock
    fen << numFullMoves_;

    return fen.str();
}

std::ostream& operator<<(std::ostream& os, const State& rhs)
{
    return os << rhs.toFENState();
}

}; // namespace Chess
