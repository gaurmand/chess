#include <cassert>
#include <chessmodel/position.h>
#include <unordered_map>

namespace chess
{

//===========================================================================
Position enPassantCapturePosition(Position enPassantPos)
{
   static const std::unordered_map<Position, Position> kPositionMap{
      {Position::a3, Position::a4},
      {Position::b3, Position::b4},
      {Position::c3, Position::c4},
      {Position::d3, Position::d4},
      {Position::e3, Position::e4},
      {Position::f3, Position::f4},
      {Position::g3, Position::g4},
      {Position::h3, Position::h4},
      {Position::a6, Position::a5},
      {Position::b6, Position::b5},
      {Position::c6, Position::c5},
      {Position::d6, Position::d5},
      {Position::e6, Position::e5},
      {Position::f6, Position::f5},
      {Position::g6, Position::g5},
      {Position::h6, Position::h5}};

   assert(kPositionMap.find(enPassantPos) != kPositionMap.end());
   return kPositionMap.at(enPassantPos);
}

//===========================================================================
Position enPassantPosition(Position enPassantCapturePos)
{
   static const std::unordered_map<Position, Position> kPositionMap{
      {Position::a4, Position::a3},
      {Position::b4, Position::b3},
      {Position::c4, Position::c3},
      {Position::d4, Position::d3},
      {Position::e4, Position::e3},
      {Position::f4, Position::f3},
      {Position::g4, Position::g3},
      {Position::h4, Position::h3},
      {Position::a5, Position::a6},
      {Position::b5, Position::b6},
      {Position::c5, Position::c6},
      {Position::d5, Position::d6},
      {Position::e5, Position::e6},
      {Position::f5, Position::f6},
      {Position::g5, Position::g6},
      {Position::h5, Position::h6}};

   assert(kPositionMap.find(enPassantCapturePos) != kPositionMap.end());
   return kPositionMap.at(enPassantCapturePos);
}

//===========================================================================
bool isDoubleAdvance(Position src, Position dst)
{
   static const std::unordered_map<Position, Position> kPositionMap{
      {Position::a2, Position::a4},
      {Position::b2, Position::b4},
      {Position::c2, Position::c4},
      {Position::d2, Position::d4},
      {Position::e2, Position::e4},
      {Position::f2, Position::f4},
      {Position::g2, Position::g4},
      {Position::h2, Position::h4},
      {Position::a7, Position::a5},
      {Position::b7, Position::b5},
      {Position::c7, Position::c5},
      {Position::d7, Position::d5},
      {Position::e7, Position::e5},
      {Position::f7, Position::f5},
      {Position::g7, Position::g5},
      {Position::h7, Position::h5}};

   auto it = kPositionMap.find(src);
   return it != kPositionMap.end() && it->second == dst;
}

//===========================================================================
Position positionFromRowColIndex(int row, int col)
{
   const int numPrecedingRows = 7 - row;
   return static_cast<Position>(numPrecedingRows * 8 + col);
}

} // namespace chess
