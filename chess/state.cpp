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

std::ostream& operator<<(std::ostream& os, const State& rhs)
{
    return os << rhs.toFENState();
}

}; // namespace Chess
