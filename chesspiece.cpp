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
            return res + ((owner == WHITE) ? "K" : "k");
        case QUEEN:
            return res + ((owner == WHITE) ? "Q" : "q");
        case ROOK:
            return res + ((owner == WHITE) ? "R" : "r");
        case BISHOP:
            return res + ((owner == WHITE) ? "B" : "b");
        case KNIGHT:
            return res + ((owner == WHITE) ? "N" : "n");
        case PAWN:
            return res + ((owner == WHITE) ? "P" : "p");
    }
}



