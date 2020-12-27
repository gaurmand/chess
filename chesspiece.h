#ifndef CHESSPIECE_H
#define CHESSPIECE_H

#include <string>
#include "constants.h"

class ChessPiece
{
public:
    ChessPiece(Player o, PieceType t, IBP p) : owner(o), type(t), pos(p) {}
    ChessPiece() : ChessPiece(WHITE, PAWN, {0, 0}) {}

    IBP getIBPos();
    void setIBPos(IBP newPos);
    PieceType getType();
    Player getOwner();
    bool isCaptured();
    std::string toString();

private:
    Player owner;
    PieceType type;
    IBP pos;
    bool captured = false;
};

#endif // CHESSPIECE_H
