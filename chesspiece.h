#ifndef CHESSPIECE_H
#define CHESSPIECE_H

#include <string>
#include "constants.h"
#include "boardposition.h"

class ChessPiece
{
public:
    ChessPiece(PieceID i, Player o, PieceType t, IBP p) : id(i), owner(o), type(t), pos(p) {}
    ChessPiece(PieceID i) : id(i) {}

    PieceID getId();
    IBP getIBPos();
    PieceType getType();
    Player getOwner();
    bool isCaptured();

    void setPiece(Player o, PieceType t, IBP p, bool c = false);
    void setIBPos(IBP newPos);
    void setType(PieceType newType);
    void setCaptured(bool cap);

    std::string toString();

private:
    PieceID id;
    Player owner;
    PieceType type;
    IBP pos;
    bool captured = false;
};

#endif // CHESSPIECE_H
