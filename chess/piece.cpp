#include "piece.h"

namespace Chess
{

std::ostream& operator<<(std::ostream& os, const Piece& rhs)
{
    const bool isWhite = rhs.owner() == Player::White;
    switch(rhs.type()) {
        case PieceType::King:
            return os << (isWhite ? 'K' : 'k');
        case PieceType::Queen:
            return os << (isWhite ? 'Q' : 'q');
        case PieceType::Rook:
            return os << (isWhite ? 'R' : 'r');
        case PieceType::Bishop:
            return os << (isWhite ? 'B' : 'b');
        case PieceType::Knight:
            return os << (isWhite ? 'N' : 'n');
        case PieceType::Pawn:
            return os << (isWhite ? 'P' : 'p');
        default:
            return os;
    }
}

bool operator==(const Piece& lhs, const Piece& rhs)
{
    return lhs.type() == rhs.type() &&
            lhs.owner() == rhs.owner() &&
            lhs.pos() == rhs.pos() &&
            lhs.id() == rhs.id() &&
            lhs.isCaptured() == rhs.isCaptured();
}

bool operator!=(const Piece& lhs, const Piece& rhs)
{
    return !(lhs == rhs);
}


}; // namespace Chess
