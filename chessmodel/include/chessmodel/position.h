#ifndef CHESSMODEL_POSITION_H
#define CHESSMODEL_POSITION_H

#include <array>
#include <cstdint>
#include <limits>
#include <ostream>

namespace chess
{

//===========================================================================
constexpr int kNumSquares = 64;
constexpr int kNumRows    = 8;
constexpr int kNumCols    = 8;

//===========================================================================
enum Position : std::uint8_t
{
   a1,
   b1,
   c1,
   d1,
   e1,
   f1,
   g1,
   h1,
   a2,
   b2,
   c2,
   d2,
   e2,
   f2,
   g2,
   h2,
   a3,
   b3,
   c3,
   d3,
   e3,
   f3,
   g3,
   h3,
   a4,
   b4,
   c4,
   d4,
   e4,
   f4,
   g4,
   h4,
   a5,
   b5,
   c5,
   d5,
   e5,
   f5,
   g5,
   h5,
   a6,
   b6,
   c6,
   d6,
   e6,
   f6,
   g6,
   h6,
   a7,
   b7,
   c7,
   d7,
   e7,
   f7,
   g7,
   h7,
   a8,
   b8,
   c8,
   d8,
   e8,
   f8,
   g8,
   h8,
   Invalid = std::numeric_limits<std::uint8_t>::max()
};

inline constexpr std::array kPositions{
   Position::a1, Position::a2, Position::a3, Position::a4, Position::a5,
   Position::a6, Position::a7, Position::a8, Position::b1, Position::b2,
   Position::b3, Position::b4, Position::b5, Position::b6, Position::b7,
   Position::b8, Position::c1, Position::c2, Position::c3, Position::c4,
   Position::c5, Position::c6, Position::c7, Position::c8, Position::d1,
   Position::d2, Position::d3, Position::d4, Position::d5, Position::d6,
   Position::d7, Position::d8, Position::e1, Position::e2, Position::e3,
   Position::e4, Position::e5, Position::e6, Position::e7, Position::e8,
   Position::f1, Position::f2, Position::f3, Position::f4, Position::f5,
   Position::f6, Position::f7, Position::f8, Position::g1, Position::g2,
   Position::g3, Position::g4, Position::g5, Position::g6, Position::g7,
   Position::g8, Position::h1, Position::h2, Position::h3, Position::h4,
   Position::h5, Position::h6, Position::h7, Position::h8};

constexpr inline bool isValid(Position pos)
{
   return pos <= Position::h8;
}

//===========================================================================
// row is the zero-based index from top to bottom.
// col is the zero-based index from left to right.
Position positionFromRowColIndex(int row, int col);

//===========================================================================
enum Direction : int8_t
{
   // Orthogonal
   N = -10,
   E = 1,
   S = 10,
   W = -1,
   // Diagonal
   NE = -9,
   SE = 11,
   SW = 9,
   NW = -11,
   // Hooks
   NNE = -19,
   ENE = -8,
   ESE = 12,
   SSE = 21,
   SSW = 19,
   WSW = 8,
   WNW = -12,
   NNW = -21
};

constexpr inline std::array kOrthogonalRays = {
   Direction::N, Direction::E, Direction::S, Direction::W};
constexpr inline std::array kDiagonalRays = {
   Direction::NE, Direction::SE, Direction::SW, Direction::NW};

constexpr inline std::array kRays = {Direction::N,
                                     Direction::NE,
                                     Direction::E,
                                     Direction::SE,
                                     Direction::S,
                                     Direction::SW,
                                     Direction::W,
                                     Direction::NW};

constexpr inline std::array kHooks = {Direction::NNE,
                                      Direction::ENE,
                                      Direction::ESE,
                                      Direction::SSE,
                                      Direction::SSW,
                                      Direction::WSW,
                                      Direction::WNW,
                                      Direction::NNW};

//===========================================================================
namespace mailbox
{

constexpr int kMailboxSize = 120;
constexpr int I            = Position::Invalid;

// clang-format off

// Maps mailbox index to a position (can be valid or invalid)
constexpr std::array<int, kMailboxSize> kMailbox =
{
   I,  I,  I,  I,  I,  I,  I,  I,  I,  I,
   I,  I,  I,  I,  I,  I,  I,  I,  I,  I,
   I, 56, 57, 58, 59, 60, 61, 62, 63,  I,
   I, 48, 49, 50, 51, 52, 53, 54, 55,  I,
   I, 40, 41, 42, 43, 44, 45, 46, 47,  I,
   I, 32, 33, 34, 35, 36, 37, 38, 39,  I,
   I, 24, 25, 26, 27, 28, 29, 30, 31,  I,
   I, 16, 17, 18, 19, 20, 21, 22, 23,  I,
   I,  8,  9, 10, 11, 12, 13, 14, 15,  I,
   I,  0,  1,  2,  3,  4,  5,  6,  7,  I,
   I,  I,  I,  I,  I,  I,  I,  I,  I,  I,
   I,  I,  I,  I,  I,  I,  I,  I,  I,  I
};

// Maps position to mailbox index
constexpr std::array<int, kNumSquares> kIndex =
{
   91, 92, 93, 94, 95, 96, 97, 98,
   81, 82, 83, 84, 85, 86, 87, 88,
   71, 72, 73, 74, 75, 76, 77, 78,
   61, 62, 63, 64, 65, 66, 67, 68,
   51, 52, 53, 54, 55, 56, 57, 58,
   41, 42, 43, 44, 45, 46, 47, 48,
   31, 32, 33, 34, 35, 36, 37, 38,
   21, 22, 23, 24, 25, 26, 27, 28,
};
// clang-format on

} // namespace mailbox

//===========================================================================
constexpr inline Position positionAt(Position pos,
                                     Direction dir,
                                     int distance = 1)
{
   const int offset = dir * distance;
   const int posInt = mailbox::kMailbox[mailbox::kIndex[pos] + offset];
   return static_cast<Position>(posInt);
}

//===========================================================================
// Converts 28 -> e4
std::string toAlgebraic(const Position& pos);
// Converts e4 -> 28
Position fromAlgebraic(std::string_view str);

//===========================================================================
std::ostream& operator<<(std::ostream&, const Position&);

//===========================================================================
Position enPassantCapturePosition(Position enPassantPos);
Position enPassantPosition(Position enPassantCapturePos);

//===========================================================================
bool isDoubleAdvance(Position src, Position dst);

} // namespace chess

#endif
