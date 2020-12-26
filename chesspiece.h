#ifndef CHESSPIECE_H
#define CHESSPIECE_H

#include "constants.h"

class ChessPiece
{
public:
    ChessPiece();

private:
    PieceType type;
    IBP pos;
    bool isCaptured;
    ChessMoves* moves;
};

#endif // CHESSPIECE_H
