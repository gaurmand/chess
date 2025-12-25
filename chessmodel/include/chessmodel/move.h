#ifndef CHESSMODEL_MOVE_H
#define CHESSMODEL_MOVE_H

#include <chessmodel/piece.h>
#include <chessmodel/position.h>
#include <cstdint>
#include <optional>
#include <ostream>
#include <string>

namespace chess
{

//===========================================================================
struct Move
{
private:
   //===========================================================================
   enum Mask
   {
      Active        = 0b00000000'00000000'00000000'00000001,
      Source        = 0b00000000'00000000'00000000'01111110,
      Destination   = 0b00000000'00000000'00011111'10000000,
      MovedPiece    = 0b00000000'00000000'11100000'00000000,
      Capture       = 0b00000000'00000001'00000000'00000000,
      CapturedPiece = 0b00000000'00001110'00000000'00000000,
      Promotion     = 0b00000000'00010000'00000000'00000000,
      PromotedPiece = 0b00000000'11100000'00000000'00000000,
      LongCastle    = 0b00000001'00000000'00000000'00000000,
      ShortCastle   = 0b00000010'00000000'00000000'00000000,
      EnPassant     = 0b00000100'00000000'00000000'00000000
   };
   //===========================================================================
   std::uint32_t repr_{0};

public:
   //===========================================================================
   Colour active() const { return static_cast<Colour>(repr_ & Active); }
   void setActive(Colour c) { repr_ = (repr_ & ~Active) | c; }

   //===========================================================================
   Position src() const { return static_cast<Position>((repr_ & Source) >> 1); }
   void setSrc(Position src) { repr_ = (repr_ & ~Source) | (src << 1); }

   //===========================================================================
   Position dst() const
   {
      return static_cast<Position>((repr_ & Destination) >> 7);
   }
   void setDst(Position dst) { repr_ = (repr_ & ~Destination) | (dst << 7); }

   //===========================================================================
   Piece piece() const
   {
      return static_cast<Piece>((repr_ & MovedPiece) >> 13);
   }
   void setPiece(Piece p) { repr_ = (repr_ & ~MovedPiece) | (p << 13); }

   //===========================================================================
   bool capture() const { return repr_ & Capture; }
   Piece capturedPiece() const
   {
      return static_cast<Piece>((repr_ & CapturedPiece) >> 17);
   }
   void setCapturedPiece(Piece p)
   {
      repr_ |= Capture;
      repr_ = (repr_ & ~CapturedPiece) | (p << 17);
   }
   void clearCapture() { repr_ &= ~(Capture | CapturedPiece); }

   //===========================================================================
   bool promotion() const { return repr_ & Promotion; }
   Piece promotedPiece() const
   {
      return static_cast<Piece>((repr_ & PromotedPiece) >> 21);
   }
   void setPromotedPiece(Piece p)
   {
      repr_ |= Promotion;
      repr_ = (repr_ & ~PromotedPiece) | (p << 21);
   }
   void clearPromotion() { repr_ &= ~(Promotion | PromotedPiece); }

   //===========================================================================
   bool longCastle() const { return repr_ & LongCastle; }
   void setLongCastle(bool c) { repr_ = (repr_ & ~LongCastle) | (c << 24); }

   //===========================================================================
   bool shortCastle() const { return repr_ & ShortCastle; }
   void setShortCastle(bool c) { repr_ = (repr_ & ~ShortCastle) | (c << 25); }

   //===========================================================================
   bool enPassant() const { return repr_ & EnPassant; }
   void setEnPassant(bool ep) { repr_ = (repr_ & ~EnPassant) | (ep << 26); }

   //===========================================================================
   bool operator==(const Move&) const = default;

   //===========================================================================
   std::string toAlgebraic() const;
   // TODO
   static std::optional<Move> fromAlgebraic(const std::string& str);
};

//===========================================================================
// Max theoretical limit is 218 moves
using MoveArray = std::array<Move, 256>;

//===========================================================================
std::ostream& operator<<(std::ostream&, const Move&);

} // namespace chess

#endif
