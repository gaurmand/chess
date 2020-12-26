#include <QString>
#include "chesspiece.h"

ChessPiece::ChessPiece()
    : ChessPiece(WHITE, PAWN, {0, 0})
{}

ChessPiece::ChessPiece(Player o, PieceType t, IBP p)
    : owner(o), type(t), pos(p), isCaptured(false), moves(nullptr)
{}

IBP ChessPiece::getIBPos()
{
    return pos;
}

void ChessPiece::setIBPos(IBP newPos)
{
    pos = newPos;
}


PieceType ChessPiece::getType()
{
    return type;
}

Player ChessPiece::getOwner()
{
    return owner;
}

ChessMoves* ChessPiece::getMoves()
{
    return moves;
}

std::string ChessPiece::toString() {
    QString str = "%1 %2 (%3, %4)";
    if(owner == WHITE) {
        str = str.arg("white");
    } else {
        str = str.arg("black");
    }

    switch(type) {
        case KING:
            str = str.arg("king");
            break;
        case QUEEN:
            str = str.arg("queen");
            break;
        case ROOK:
            str = str.arg("rook");
            break;
        case BISHOP:
            str = str.arg("bishop");
            break;
        case KNIGHT:
            str = str.arg("knight");
            break;
        case PAWN:
            str = str.arg("pawn");
            break;
    }

    return str.arg(pos.row).arg(pos.col).toStdString();
}



