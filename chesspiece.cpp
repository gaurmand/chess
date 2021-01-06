#include <QString>
#include "chesspiece.h"
#include "chessboard.h"

bool ChessPiece::isCaptured()
{
    return captured;
}

PieceID ChessPiece::getId()
{
    return id;
}

IBP ChessPiece::getIBPos()
{
    return pos;
}

ABP ChessPiece::getABPos()
{
    return BoardPosition::tranlateIBPoABP(pos);
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
    std::string res = BoardPosition::tranlateIBPoABP(pos);
    return toChar() + res;
}

std::string ChessPiece::toChar()
{
    switch(type) {
        case KING:
            return ((owner == WHITE) ? "K" : "k");
        case QUEEN:
            return ((owner == WHITE) ? "Q" : "q");
        case ROOK:
            return ((owner == WHITE) ? "R" : "r");
        case BISHOP:
            return ((owner == WHITE) ? "B" : "b");
        case KNIGHT:
            return ((owner == WHITE) ? "N" : "n");
        case PAWN:
            return ((owner == WHITE) ? "P" : "p");
    }
}
