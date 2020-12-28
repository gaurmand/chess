#ifndef CHESSPIECE_H
#define CHESSPIECE_H

#include <string>
#include "constants.h"

class ChessPiece
{
public:
    ChessPiece(Player o, PieceType t, IBP p) : owner(o), type(t), pos(p) {}
    ChessPiece() {}

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
    Player owner;
    PieceType type;
    IBP pos;
    bool captured = false;
};

#endif // CHESSPIECE_H
