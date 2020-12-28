#include <QString>
#include "chesspiece.h"
#include "chessboard.h"

bool ChessPiece::isCaptured()
{
    return captured;
}

IBP ChessPiece::getIBPos()
{
    return pos;
}

PieceType ChessPiece::getType()
{
    return type;
}

Player ChessPiece::getOwner()
{
    return owner;
}

void ChessPiece::setPiece(Player o, PieceType t, IBP p, bool c)
{
    owner = o;
    type = t;
    pos = p;
    captured = c;
}

void ChessPiece::setType(PieceType newType)
{
    type = newType;
}

void ChessPiece::setIBPos(IBP newPos)
{
    pos = newPos;
}

void ChessPiece::setCaptured(bool cap)
{
    captured = cap;
}

std::string ChessPiece::toString() {
    std::string res = ChessBoard::tranlateIBPoABP(pos);
    switch(type) {
        case KING:
            return ((owner == WHITE) ? "K" : "k") + res;
        case QUEEN:
            return ((owner == WHITE) ? "Q" : "q") + res;
        case ROOK:
            return ((owner == WHITE) ? "R" : "r") + res;
        case BISHOP:
            return ((owner == WHITE) ? "B" : "b") + res;
        case KNIGHT:
            return ((owner == WHITE) ? "N" : "n") + res;
        case PAWN:
            return ((owner == WHITE) ? "P" : "p") + res;
    }
}



