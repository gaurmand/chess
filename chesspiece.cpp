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

//std::string ChessPiece::toString() {
//    QString str = "%1 %2 (%3, %4)";
//    if(owner == WHITE) {
//        str = str.arg("white");
//    } else {
//        str = str.arg("black");
//    }

//    switch(type) {
//        case KING:
//            str = str.arg("king");
//            break;
//        case QUEEN:
//            str = str.arg("queen");
//            break;
//        case ROOK:
//            str = str.arg("rook");
//            break;
//        case BISHOP:
//            str = str.arg("bishop");
//            break;
//        case KNIGHT:
//            str = str.arg("knight");
//            break;
//        case PAWN:
//            str = str.arg("pawn");
//            break;
//    }

//    return str.arg(pos.row).arg(pos.col).toStdString();
//}

std::string ChessPiece::toString() {
    std::string res = "";
    if(owner == WHITE) {
        switch(type) {
            case KING:
                res = "K";
                break;
            case QUEEN:
                res = "Q";
                break;
            case ROOK:
                res = "R";
                break;
            case BISHOP:
                res = "B";
                break;
            case KNIGHT:
                res = "N";
                break;
            case PAWN:
                res = "P";
                break;
        }
    } else {
        switch(type) {
            case KING:
                res = "k";
                break;
            case QUEEN:
                res = "q";
                break;
            case ROOK:
                res = "r";
                break;
            case BISHOP:
                res = "b";
                break;
            case KNIGHT:
                res = "n";
                break;
            case PAWN:
                res = "p";
                break;
        }
    }

    res += ChessBoard::tranlateIBPoABP(pos);
    return res;
}



