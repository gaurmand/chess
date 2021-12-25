#ifndef PIECE_H
#define PIECE_H

#include "common.h"
#include "boardpos.h"

#include <iostream>

namespace Chess
{

class Piece
{
public:
    Piece() {}
    Piece(PieceType type, Player owner, BP pos, int id, bool isCaptured = false):
        type_(type), owner_(owner), pos_(pos), id_(id), isCaptured_(isCaptured) {}

    PieceType type() const { return type_; }
    Player owner() const { return owner_; }
    BP pos() const { return pos_; }
    int id() const { return id_; }
    bool isCaptured() const { return isCaptured_; }

    bool isValid() const { return id_ >= MIN_PIECE_ID && id_ <= MAX_PIECE_ID; }

    void capture() { isCaptured_ = true; }
    void setType(PieceType type) { type_ = type; }
    void setPos(const BP& pos) { pos_ = pos; }

private:
    PieceType type_ = PieceType::King;
    Player owner_ = Player::White;
    BP pos_ = BP();
    int id_ = -1;
    bool isCaptured_ = true;
};

std::ostream& operator<<(std::ostream&, const Piece&);
bool operator==(const Piece&, const Piece&);
bool operator!=(const Piece&, const Piece&);

}; // namespace Chess

#endif // PIECE_H
