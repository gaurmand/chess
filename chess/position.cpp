#include "position.h"

#include <sstream>
#include <regex>

namespace Chess
{

Position::Position(const FENState& state)
{
    setFENState(state);
};

void Position::setFENState(const FENState& state)
{
    std::istringstream fen(state);
    std::string boardState;
    if(!(fen >> boardState))
    {
        throw 1;
    }
    Board::setFENState(boardState);

    std::string flags;
    if(!std::getline(fen, flags))
    {
        throw 1;
    }
    State::setFENState(flags);
}

Position Position::save() const
{
    return *this;
}

void Position::restore(const Position& pos)
{
    *this = pos;
}

bool Position::isValidFENState(const FENState& state)
{
    // TODO check board size
    static const std::regex re("^([KkQqRrBbNnPp\\d]{1,8}/){7}[KkQqRrBbNnPp\\d]{1,8} [wb] (-|K*Q*k*q*) ([a-h][1-8]|-) (\\d)+ (\\d)+$");
    return std::regex_match(state, re);
}

}; // namespace Chess
