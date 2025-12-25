#ifndef CHESSMODEL_STATE_H
#define CHESSMODEL_STATE_H

#include <chessmodel/piece.h>
#include <chessmodel/position.h>

//===========================================================================
namespace chess
{

//===========================================================================
struct State
{
   Colour active{Colour::White};
   std::array<bool, 2> longCastle{true, true};
   std::array<bool, 2> shortCastle{true, true};
   Position enPassant{Position::Invalid};
   std::uint16_t halfMoveClock{0};
   std::uint16_t fullMoveClock{1};
};

} // namespace chess

#endif
