#ifndef CHESSPIECE_H
#define CHESSPIECE_H

#include "constants.h"

class ChessPiece
{
public:
    ChessPiece();
    IBP getIBP();
    PieceType getType();
    Player getOwner();

private:
    PieceType type;
    IBP pos;
    bool isCaptured;
    ChessMoves* moves;
};

#endif // CHESSPIECE_H
