#include "move.h"

#include <iostream>

namespace Chess
{

Move::Move(const LANMove& move): Move()
{
    if (move.length() >= 5)
    {
        src_ = BP(move.substr(0, 2));
        dst_ = BP(move.substr(3, 2));
    }
}

LANMove Move::toLANMove() const
{
    if (!isValid())
    {
        return ANBP();
    }
    return src_.toANBP() + " " + dst_.toANBP();
}

std::ostream& operator<<(std::ostream& os, const Move& rhs)
{
    return os << rhs.toLANMove();
}

bool operator==(const Move& lhs, const Move& rhs)
{
    return lhs.src() == rhs.src() && lhs.dst() == rhs.dst();
}

bool operator!=(const Move& lhs, const Move& rhs)
{
    return !(lhs == rhs);
}

}; // namespace Chess

