#ifndef CHESSMODEL_PIECE_H
#define CHESSMODEL_PIECE_H

#include <array>
#include <chessmodel/bitboard.h>

//===========================================================================
namespace chess
{

//===========================================================================
enum Colour : uint8_t
{
   White,
   Black
};

//===========================================================================
inline Colour opponent(Colour c)
{
   return static_cast<Colour>(!c);
}

//===========================================================================
enum Piece : uint8_t
{
   King,
   Queen,
   Rook,
   Bishop,
   Knight,
   Pawn
};

//===========================================================================
inline constexpr auto kNumColours    = 2;
inline constexpr auto kNumPieceTypes = 6;

//===========================================================================
inline constexpr std::array kColours = {Colour::White, Colour::Black};
inline constexpr std::array kPieces  = {Piece::King,
                                        Piece::Queen,
                                        Piece::Rook,
                                        Piece::Bishop,
                                        Piece::Knight,
                                        Piece::Pawn};

//===========================================================================
inline constexpr Colour kInvalidColour{
   std::numeric_limits<std::uint8_t>::max()};
inline constexpr Piece kInvalidPiece{std::numeric_limits<std::uint8_t>::max()};

} // namespace chess

#endif
