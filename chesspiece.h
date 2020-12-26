#ifndef CHESSPIECE_H
#define CHESSPIECE_H

#include <string>
#include "constants.h"

class ChessPiece
{
public:
    ChessPiece();
    ChessPiece(Player owner, PieceType type, IBP pos);
    IBP getIBPos();
    void setIBPos(IBP newPos);
    PieceType getType();
    Player getOwner();
    ChessMoves* getMoves();
    std::string toString();

private:
    Player owner;
    PieceType type;
    IBP pos;
    bool isCaptured;
    ChessMoves* moves;
};

#endif // CHESSPIECE_H
