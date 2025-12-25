#include <chessmodel/board.h>
#include <chessmodel/move.h>

namespace chess
{

//===========================================================================
bool Board::empty() const
{
   return occupied_.none();
}

//===========================================================================
bool Board::occupied(Position pos) const
{
   return occupied_.test(pos);
}

//===========================================================================
std::optional<Colour> Board::colour(Position pos) const
{
   for (const auto c : kColours)
   {
      if (colours_[c].test(pos))
      {
         return c;
      }
   }

   return std::nullopt;
}

//===========================================================================
std::optional<Piece> Board::piece(Position pos) const
{
   for (const auto p : kPieces)
   {
      if (pieces_[p].test(pos))
      {
         return p;
      }
   }

   return std::nullopt;
}

//===========================================================================
void Board::move(Position src, Position dst, Piece p, Colour c)
{
   const Bitboard srcDstBB = Bitboard{src} | Bitboard{dst};
   occupied_ ^= srcDstBB;
   pieces_[p] ^= srcDstBB;
   colours_[c] ^= srcDstBB;
}

//===========================================================================
void Board::set(Position pos, Piece piece, Colour colour)
{
   const Bitboard posBB{pos};
   occupied_ |= posBB;
   pieces_[piece] |= posBB;
   colours_[colour] |= posBB;
}

//===========================================================================
void Board::clear(Position pos, Piece piece, Colour colour)
{
   const Bitboard posBB = ~Bitboard{pos};
   occupied_ &= posBB;
   pieces_[piece] &= posBB;
   colours_[colour] &= posBB;
}

//===========================================================================
void Board::promote(Position pos, Piece from, Piece to)
{
   const Bitboard posBB = Bitboard{pos};
   pieces_[from] &= ~posBB;
   pieces_[to] |= posBB;
}

//===========================================================================
void Board::clear()
{
   occupied_.clear();
   for (const auto c : kColours)
   {
      colours_[c].clear();
   }
   for (const auto p : kPieces)
   {
      pieces_[p].clear();
   }
}

} // namespace chess
