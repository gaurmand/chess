#ifndef CHESSMODEL_BOARD_H
#define CHESSMODEL_BOARD_H

#include <array>
#include <chessmodel/bitboard.h>
#include <chessmodel/move.h>
#include <chessmodel/piece.h>
#include <chessmodel/position.h>
#include <optional>

//===========================================================================
namespace chess
{

class MoveGenerator;

class Board
{
   //===========================================================================
   friend class MoveGenerator;

public:
   //===========================================================================
   Board() = default;

   //===========================================================================
   bool empty() const;
   void clear();

   //===========================================================================
   bool occupied(Position pos) const;
   std::optional<Colour> colour(Position pos) const;
   std::optional<Piece> piece(Position pos) const;

   //===========================================================================
   void move(Position src, Position dst, Piece p, Colour c);
   void set(Position pos, Piece p, Colour c);
   void clear(Position pos, Piece p, Colour c);
   void promote(Position pos, Piece from, Piece to);

private:
   //===========================================================================
   std::array<Bitboard, kPieces.size()> pieces_;
   std::array<Bitboard, kColours.size()> colours_;
   Bitboard occupied_;
};

} // namespace chess

#endif
