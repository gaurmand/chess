#ifndef MOVE_H
#define MOVE_H

#include "boardpos.h"

namespace Chess
{

enum class MoveType{Unknown, Normal, Capture, EnPassant, ShortCastle, LongCastle, Promotion};

typedef std::string LongAlgebraicNotationMove;
typedef LongAlgebraicNotationMove LANMove;

class Move
{
public:
    Move() {}
    Move(BP src, BP dst, MoveType type = MoveType::Unknown): src_(src), dst_(dst), type_(type) {}
    Move(const LANMove& move);

    LANMove toLANMove() const;
    bool isValid() const {return src_.isValid() && dst_.isValid();}

    BP src() const {return src_;}
    BP dst() const {return dst_;}
    MoveType type() const {return type_;}
    void setType(MoveType type) {type_ = type;}

private:
    BP src_= BP();
    BP dst_ = BP();
    MoveType type_ = MoveType::Unknown;
};

std::ostream& operator<<(std::ostream&, const Move&);

bool operator==(const Move&, const Move&);
bool operator!=(const Move&, const Move&);

}; // namespace Chess

#endif // MOVE_H
