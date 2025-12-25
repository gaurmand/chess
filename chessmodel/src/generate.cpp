#include "chessmodel/piece.h"
#include "chessmodel/position.h"

#include <algorithm>
#include <cassert>
#include <chessmodel/generate.h>

namespace chess
{

namespace
{

//===========================================================================
using BitboardArray            = std::array<Bitboard, kPositions.size()>;
using BitboardArrayPerPosition = std::array<BitboardArray, kPositions.size()>;

//===========================================================================
template <typename Directions>
constexpr void setMoves(Bitboard& bb,
                        Position pos,
                        const Directions& directions)
{
   for (Direction dir : directions)
   {
      if (const Position movePos = positionAt(pos, dir);
          movePos != Position::Invalid)
      {
         bb.set(movePos);
      }
   }
}

//===========================================================================
template <typename Directions>
constexpr void setMovesAlongRay(Bitboard& bb,
                                Position pos,
                                const Directions& rays)
{
   for (Direction ray : rays)
   {
      for (Position movePos = positionAt(pos, ray);
           movePos != Position::Invalid;
           movePos = positionAt(movePos, ray))
      {
         bb.set(movePos);
      }
   }
}

//===========================================================================
template <typename Directions>
constexpr void setMovesAlongRayExceptLast(Bitboard& bb,
                                          Position pos,
                                          const Directions& rays)
{
   for (Direction ray : rays)
   {
      for (Position movePos = positionAt(pos, ray);
           movePos != Position::Invalid;
           movePos = positionAt(movePos, ray))
      {
         if (positionAt(pos, ray) == Position::Invalid)
         {
            // Break early if next position is out of bounds
            break;
         }

         bb.set(movePos);
      }
   }
}

//===========================================================================
constexpr BitboardArrayPerPosition kBehindBlockers = []() {
   BitboardArrayPerPosition behindBBArr;
   for (Position src : kPositions)
   {
      for (Direction ray : kRays)
      {
         int dist = 1;
         for (Position blocker = positionAt(src, ray, dist);
              blocker != Position::Invalid && dist < 8;
              blocker = positionAt(src, ray, ++dist))
         {
            Bitboard& bb = behindBBArr[src][blocker];
            setMovesAlongRay(bb, blocker, std::array{ray});
         }
      }
   }

   return behindBBArr;
}();

//===========================================================================
// Bitboard getWPawnAttacks(Position src, Bitboard enemy)
// {
//    static constexpr BitboardArray kWPawnAttacks = []() {
//       BitboardArray attackBBs;
//       for (Position pos : kPositions)
//       {
//          setMoves(
//             attackBBs[pos], pos, std::array{Direction::NW, Direction::NE});
//       }

//       return attackBBs;
//    }();

//    return kWPawnAttacks[src] & enemy;
// }

//===========================================================================
// Bitboard getWPawnAdvances(Position src, Bitboard occupied)
// {
//    static constexpr BitboardArray kWPawnAdvances = []() {
//       BitboardArray attackBBs;
//       for (Position pos : kPositions)
//       {
//          setMoves(attackBBs[pos], pos, std::array{Direction::N});

//          if (pos >= Position::a2 && pos <= Position::h2)
//          {
//             setMoves(attackBBs[pos],
//                      positionAt(pos, Direction::N),
//                      std::array{Direction::N});
//          }
//       }

//       return attackBBs;
//    }();

//    static constexpr BitboardArray kWPawnBlockers = []() {
//       BitboardArray blockersBBs;
//       for (Position pos = Position::a2; pos <= Position::h2;
//            pos          = static_cast<Position>(pos + 1))
//       {
//          blockersBBs[pos].set(positionAt(pos, Direction::N));
//       }

//       return blockersBBs;
//    }();

//    Bitboard moves    = kWPawnAdvances[src] & ~occupied;
//    Bitboard blockers = kWPawnBlockers[src] & occupied;
//    for (Position blocker : blockers)
//    {
//       moves &= ~kBehindBlockers[src][blocker];
//    }

//    return moves;
// }

//===========================================================================
// Bitboard getWPawnMoves(Position src, Bitboard occupied, Bitboard enemy)
// {
//    return getWPawnAdvances(src, occupied) | getWPawnAttacks(src, enemy);
// }

//===========================================================================
// Bitboard getBPawnAttacks(Position src, Bitboard enemy)
// {
//    static constexpr BitboardArray kBPawnAttacks = []() {
//       BitboardArray attackBBs;
//       for (Position pos : kPositions)
//       {
//          setMoves(
//             attackBBs[pos], pos, std::array{Direction::SW, Direction::SE});
//       }

//       return attackBBs;
//    }();

//    return kBPawnAttacks[src] & enemy;
// }

//===========================================================================
// Bitboard getBPawnAdvances(Position src, Bitboard occupied)
// {
//    static constexpr BitboardArray kBPawnAdvances = []() {
//       BitboardArray attackBBs;
//       for (Position pos : kPositions)
//       {
//          setMoves(attackBBs[pos], pos, std::array{Direction::S});

//          if (pos >= Position::a7 && pos <= Position::h7)
//          {
//             setMoves(attackBBs[pos],
//                      positionAt(pos, Direction::S),
//                      std::array{Direction::S});
//          }
//       }

//       return attackBBs;
//    }();

//    static constexpr BitboardArray kBPawnBlockers = []() {
//       BitboardArray blockersBBs;
//       for (Position pos = Position::a7; pos <= Position::h7;
//            pos          = static_cast<Position>(pos + 1))
//       {
//          blockersBBs[pos].set(positionAt(pos, Direction::S));
//       }

//       return blockersBBs;
//    }();

//    Bitboard moves    = kBPawnAdvances[src] & ~occupied;
//    Bitboard blockers = kBPawnBlockers[src] & occupied;
//    for (Position blocker : blockers)
//    {
//       moves &= ~kBehindBlockers[src][blocker];
//    }

//    return moves;
// }

//===========================================================================
// Bitboard getBPawnMoves(Position src, Bitboard occupied, Bitboard enemy)
// {
//    return getBPawnAdvances(src, occupied) | getBPawnAttacks(src, enemy);
// }

//===========================================================================
Bitboard getPawnAdvances(Colour c, Position src, Bitboard occupied)
{
   static constexpr BitboardArray kWPawnAdvances = []() {
      BitboardArray attackBBs;
      for (Position pos : kPositions)
      {
         setMoves(attackBBs[pos], pos, std::array{Direction::N});

         if (pos >= Position::a2 && pos <= Position::h2)
         {
            setMoves(attackBBs[pos],
                     positionAt(pos, Direction::N),
                     std::array{Direction::N});
         }
      }

      return attackBBs;
   }();

   static constexpr BitboardArray kWPawnBlockers = []() {
      BitboardArray blockersBBs;
      for (Position pos = Position::a2; pos <= Position::h2;
           pos          = static_cast<Position>(pos + 1))
      {
         blockersBBs[pos].set(positionAt(pos, Direction::N));
      }

      return blockersBBs;
   }();

   static constexpr BitboardArray kBPawnAdvances = []() {
      BitboardArray attackBBs;
      for (Position pos : kPositions)
      {
         setMoves(attackBBs[pos], pos, std::array{Direction::S});

         if (pos >= Position::a7 && pos <= Position::h7)
         {
            setMoves(attackBBs[pos],
                     positionAt(pos, Direction::S),
                     std::array{Direction::S});
         }
      }

      return attackBBs;
   }();

   static constexpr BitboardArray kBPawnBlockers = []() {
      BitboardArray blockersBBs;
      for (Position pos = Position::a7; pos <= Position::h7;
           pos          = static_cast<Position>(pos + 1))
      {
         blockersBBs[pos].set(positionAt(pos, Direction::S));
      }

      return blockersBBs;
   }();

   static constexpr std::array kPawnAdvances = {kWPawnAdvances, kBPawnAdvances};
   static constexpr std::array kPawnBlockers = {kWPawnBlockers, kBPawnBlockers};

   Bitboard moves    = kPawnAdvances[c][src] & ~occupied;
   Bitboard blockers = kPawnBlockers[c][src] & occupied;
   for (Position blocker : blockers)
   {
      moves &= ~kBehindBlockers[src][blocker];
   }

   return moves;
}

//===========================================================================
Bitboard getPawnAttacks(Colour c, Position src, Bitboard enemy)
{
   static constexpr BitboardArray kWPawnAttacks = []() {
      BitboardArray attackBBs;
      for (Position pos : kPositions)
      {
         setMoves(
            attackBBs[pos], pos, std::array{Direction::NW, Direction::NE});
      }

      return attackBBs;
   }();
   static constexpr BitboardArray kBPawnAttacks = []() {
      BitboardArray attackBBs;
      for (Position pos : kPositions)
      {
         setMoves(
            attackBBs[pos], pos, std::array{Direction::SW, Direction::SE});
      }

      return attackBBs;
   }();

   static constexpr std::array kPawnAttacks = {kWPawnAttacks, kBPawnAttacks};

   return kPawnAttacks[c][src] & enemy;
}

// //===========================================================================
// Bitboard getWPawnEnPassants(Position enPassantPos, Bitboard wPawns)
// {
//    return getBPawnAttacks(enPassantPos, wPawns);
// }

// //===========================================================================
// Bitboard getBPawnEnPassants(Position enPassantPos, Bitboard bPawns)
// {
//    return getWPawnAttacks(enPassantPos, bPawns);
// }

//===========================================================================
Bitboard getPawnEnPassants(Colour c, Position enPassantPos, Bitboard pawns)
{
   return getPawnAttacks(opponent(c), enPassantPos, pawns);
}

//===========================================================================
Bitboard getKingMoves(Position src)
{
   static constexpr BitboardArray kKingAttacks = []() {
      BitboardArray attackBBs;
      for (Position pos : kPositions)
      {
         setMoves(attackBBs[pos], pos, kRays);
      }

      return attackBBs;
   }();

   return kKingAttacks[src];
}

//===========================================================================
Bitboard getKnightMoves(Position src)
{
   static constexpr BitboardArray kKnightAttacks = []() {
      BitboardArray attackBBs;
      for (Position pos : kPositions)
      {
         setMoves(attackBBs[pos], pos, kHooks);
      }

      return attackBBs;
   }();

   return kKnightAttacks[src];
}

//===========================================================================
Bitboard getRookMoves(Position src, Bitboard occupied)
{
   static constexpr BitboardArray kRookAttacks = []() {
      BitboardArray attackBBs;
      for (Position pos : kPositions)
      {
         setMovesAlongRay(attackBBs[pos], pos, kOrthogonalRays);
      }

      return attackBBs;
   }();

   static constexpr BitboardArray kRookBlockers = []() {
      BitboardArray blockersBBs;
      for (Position pos : kPositions)
      {
         setMovesAlongRayExceptLast(blockersBBs[pos], pos, kOrthogonalRays);
      }

      return blockersBBs;
   }();

   Bitboard moves    = kRookAttacks[src];
   Bitboard blockers = kRookBlockers[src] & occupied;
   for (Position blocker : blockers)
   {
      moves &= ~kBehindBlockers[src][blocker];
   }

   return moves;
}

//===========================================================================
Bitboard getBishopMoves(Position src, Bitboard occupied)
{
   static constexpr BitboardArray kBishopAttacks = []() {
      BitboardArray attackBBs;
      for (Position pos : kPositions)
      {
         setMovesAlongRay(attackBBs[pos], pos, kDiagonalRays);
      }

      return attackBBs;
   }();

   static constexpr BitboardArray kBishopBlockers = []() {
      BitboardArray blockersBBs;
      for (Position pos : kPositions)
      {
         setMovesAlongRayExceptLast(blockersBBs[pos], pos, kDiagonalRays);
      }

      return blockersBBs;
   }();

   Bitboard moves    = kBishopAttacks[src];
   Bitboard blockers = kBishopBlockers[src] & occupied;
   for (Position blocker : blockers)
   {
      moves &= ~kBehindBlockers[src][blocker];
   }
   return moves;
}

//===========================================================================
Bitboard getQueenMoves(Position src, Bitboard occupied)
{
   static constexpr BitboardArray kQueenAttacks = []() {
      BitboardArray attackBBs;
      for (Position pos : kPositions)
      {
         setMovesAlongRay(attackBBs[pos], pos, kRays);
      }

      return attackBBs;
   }();

   static constexpr BitboardArray kQueenBlockers = []() {
      BitboardArray blockersBBs;
      for (Position pos : kPositions)
      {
         setMovesAlongRayExceptLast(blockersBBs[pos], pos, kRays);
      }

      return blockersBBs;
   }();

   Bitboard moves    = kQueenAttacks[src];
   Bitboard blockers = kQueenBlockers[src] & occupied;
   for (Position blocker : blockers)
   {
      moves &= ~kBehindBlockers[src][blocker];
   }
   return moves;
}

// //===========================================================================
// bool canWShortCastle(Bitboard kings, Bitboard rooks, Bitboard occupied)
// {
//    static constexpr Bitboard kEmptySpace{0x0000000000000060};
//    return kings.test(Position::e1) && rooks.test(Position::h1) &&
//           (occupied & kEmptySpace) == Bitboard{};
// }

// //===========================================================================
// bool canWLongCastle(Bitboard kings, Bitboard rooks, Bitboard occupied)
// {
//    static constexpr Bitboard kEmptySpace{0x000000000000000E};
//    return kings.test(Position::e1) && rooks.test(Position::a1) &&
//           (occupied & kEmptySpace) == Bitboard{};
// }

// //===========================================================================
// bool canBShortCastle(Bitboard kings, Bitboard rooks, Bitboard occupied)
// {
//    static constexpr Bitboard kEmptySpace{0x6000000000000000};
//    return kings.test(Position::e8) && rooks.test(Position::h8) &&
//           (occupied & kEmptySpace) == Bitboard{};
// }

// //===========================================================================
// bool canBLongCastle(Bitboard kings, Bitboard rooks, Bitboard occupied)
// {
//    static constexpr Bitboard kEmptySpace{0x0E00000000000000};
//    return kings.test(Position::e8) && rooks.test(Position::a8) &&
//           (occupied & kEmptySpace) == Bitboard{};
// }

//===========================================================================
bool canLongCastle(Colour c, Bitboard kings, Bitboard rooks, Bitboard occupied)
{
   static constexpr Bitboard kWEmptySpace{0x000000000000000E};
   static constexpr Bitboard kBEmptySpace{0x0E00000000000000};

   if (c == Colour::White)
   {
      return kings.test(Position::e1) && rooks.test(Position::a1) &&
             (occupied & kWEmptySpace) == Bitboard{};
   }
   else
   {
      return kings.test(Position::e8) && rooks.test(Position::a8) &&
             (occupied & kBEmptySpace) == Bitboard{};
   }
}

//===========================================================================
bool canShortCastle(Colour c, Bitboard kings, Bitboard rooks, Bitboard occupied)
{
   static constexpr Bitboard kWEmptySpace{0x0000000000000060};
   static constexpr Bitboard kBEmptySpace{0x6000000000000000};

   if (c == Colour::White)
   {
      return kings.test(Position::e1) && rooks.test(Position::h1) &&
             (occupied & kWEmptySpace) == Bitboard{};
   }
   else
   {
      return kings.test(Position::e8) && rooks.test(Position::h8) &&
             (occupied & kBEmptySpace) == Bitboard{};
   }
}

//===========================================================================
void sortMoves(MoveArray& moves, int numMoves)
{
   std::ranges::sort(moves.begin(),
                     moves.begin() + numMoves,
                     [](const Move& lhs, const Move& rhs) {
      return lhs.toAlgebraic() < rhs.toAlgebraic();
   });
}

} // namespace

//===========================================================================
MoveGenerator::MoveGenerator(const Board& board, const State& state)
   : board_(board), state_(state)
{
}

//===========================================================================
int MoveGenerator::exec(MoveArray& moves) const
{
   MoveWriter writer{moves.begin()};
   exec(writer);

   if (sortMovesOnOutput_)
   {
      sortMoves(moves, writer.numOutput());
   }

   return writer.numOutput();
}

//===========================================================================
int MoveGenerator::exec(Piece pc, MoveArray& moves) const
{
   MoveWriter writer{moves.begin()};
   switch (pc)
   {
      case Piece::Pawn: generatePawnMoves(writer); break;
      case Piece::King: generateKingMoves(writer); break;
      case Piece::Knight: generateKnightMoves(writer); break;
      case Piece::Bishop: generateBishopMoves(writer); break;
      case Piece::Rook: generateRookMoves(writer); break;
      case Piece::Queen: generateQueenMoves(writer); break;
   }

   if (sortMovesOnOutput_)
   {
      sortMoves(moves, writer.numOutput());
   }

   return writer.numOutput();
}

//===========================================================================
void MoveGenerator::exec(MoveWriter<MoveArray::iterator>& writer) const
{
   generatePawnMoves(writer);
   generateKingMoves(writer);
   generateKnightMoves(writer);
   generateBishopMoves(writer);
   generateRookMoves(writer);
   generateQueenMoves(writer);
}

//===========================================================================
Bitboard MoveGenerator::pieces(Piece p, Colour c) const
{
   return board_.pieces_[p] & board_.colours_[c];
}

//===========================================================================
Position MoveGenerator::getKingPosition(Colour c) const
{
   Bitboard kingBB = board_.pieces_[Piece::King] & board_.colours_[c];
   assert(kingBB.count() == 1);
   return *kingBB.begin();
}

//===========================================================================
void MoveGenerator::generatePawnMoves(
   MoveWriter<MoveArray::iterator>& writer) const
{
   Move mv;
   mv.setPiece(Piece::Pawn);
   mv.setActive(state_.active);

   Colour inactive = opponent(state_.active);
   static constexpr std::array kPromotionMask{Bitboard{0xFF00000000000000},
                                              Bitboard{0x00000000000000FF}};

   Bitboard pawns = pieces(Piece::Pawn, state_.active);
   for (Position src : pawns)
   {
      mv.setSrc(src);

      Bitboard moves   = getPawnAdvances(state_.active, src, board_.occupied_);
      bool isPromotion = (moves & kPromotionMask[state_.active]) != Bitboard{};
      for (Position dst : moves)
      {
         mv.setDst(dst);

         if (isPromotion)
         {
            writePromotions(writer, mv);
         }
         else
         {
            writeMove(writer, mv);
         }
      }

      Bitboard captures =
         getPawnAttacks(state_.active, src, board_.colours_[inactive]);
      isPromotion = (captures & kPromotionMask[state_.active]) != Bitboard{};
      for (Position dst : captures)
      {
         mv.setDst(dst);
         mv.setCapturedPiece(board_.piece(dst).value());

         if (isPromotion)
         {
            writePromotions(writer, mv);
         }
         else
         {
            writeMove(writer, mv);
         }
      }

      mv.clearCapture();
   }

   if (state_.enPassant != Position::Invalid)
   {
      pawns = getPawnEnPassants(state_.active, state_.enPassant, pawns);
      for (Position src : pawns)
      {
         mv.setSrc(src);
         mv.setDst(state_.enPassant);
         mv.setCapturedPiece(Piece::Pawn);
         mv.setEnPassant(true);
         writeMove(writer, mv);
      }
   }
}

//===========================================================================
void MoveGenerator::generateKingMoves(
   MoveWriter<MoveArray::iterator>& writer) const
{
   Move mv;
   mv.setPiece(Piece::King);
   mv.setActive(state_.active);

   Colour inactive = opponent(state_.active);

   Bitboard kings = pieces(Piece::King, state_.active);
   for (Position src : kings)
   {
      Bitboard moves = getKingMoves(src);
      moves &= ~board_.colours_[state_.active];
      Bitboard captures = moves & board_.colours_[inactive];
      moves &= ~captures;

      mv.setSrc(src);

      for (Position dst : moves)
      {
         mv.setDst(dst);
         writeMove(writer, mv);
      }

      for (Position dst : captures)
      {
         mv.setDst(dst);
         mv.setCapturedPiece(board_.piece(dst).value());
         writeMove(writer, mv);
      }

      mv.clearCapture();
   }

   Bitboard rooks = pieces(Piece::Rook, state_.active);
   if (state_.shortCastle[state_.active] &&
       canShortCastle(state_.active, kings, rooks, board_.occupied_))
   {
      static constexpr std::array kShortCastleDest{Position::g1, Position::g8};
      mv.setDst(kShortCastleDest[state_.active]);
      mv.setShortCastle(true);
      writeMove(writer, mv);
   }
   if (state_.longCastle[state_.active] &&
       canLongCastle(state_.active, kings, rooks, board_.occupied_))
   {
      static constexpr std::array kLongCastleDest{Position::c1, Position::c8};
      mv.setDst(kLongCastleDest[state_.active]);
      mv.setShortCastle(false);
      mv.setLongCastle(true);
      writeMove(writer, mv);
   }
}

//===========================================================================
void MoveGenerator::generateKnightMoves(
   MoveWriter<MoveArray::iterator>& writer) const
{
   Move mv;
   mv.setPiece(Piece::Knight);
   mv.setActive(state_.active);

   Colour inactive = opponent(state_.active);

   for (Position src : pieces(Piece::Knight, state_.active))
   {
      Bitboard moves = getKnightMoves(src);
      moves &= ~board_.colours_[state_.active];
      Bitboard captures = moves & board_.colours_[inactive];
      moves &= ~captures;

      mv.setSrc(src);

      for (Position dst : moves)
      {
         mv.setDst(dst);
         writeMove(writer, mv);
      }

      for (Position dst : captures)
      {
         mv.setDst(dst);
         mv.setCapturedPiece(board_.piece(dst).value());
         writeMove(writer, mv);
      }

      mv.clearCapture();
   }
}

//===========================================================================
void MoveGenerator::generateBishopMoves(
   MoveWriter<MoveArray::iterator>& writer) const
{
   Move mv;
   mv.setPiece(Piece::Bishop);
   mv.setActive(state_.active);

   Colour inactive = opponent(state_.active);

   for (Position src : pieces(Piece::Bishop, state_.active))
   {
      Bitboard moves = getBishopMoves(src, board_.occupied_);
      moves &= ~board_.colours_[state_.active];
      Bitboard captures = moves & board_.colours_[inactive];
      moves &= ~captures;

      mv.setSrc(src);

      for (Position dst : moves)
      {
         mv.setDst(dst);
         writeMove(writer, mv);
      }

      for (Position dst : captures)
      {
         mv.setDst(dst);
         mv.setCapturedPiece(board_.piece(dst).value());
         writeMove(writer, mv);
      }

      mv.clearCapture();
   }
}

//===========================================================================
void MoveGenerator::generateRookMoves(
   MoveWriter<MoveArray::iterator>& writer) const
{
   Move mv;
   mv.setPiece(Piece::Rook);
   mv.setActive(state_.active);

   Colour inactive = opponent(state_.active);

   for (Position src : pieces(Piece::Rook, state_.active))
   {
      Bitboard moves = getRookMoves(src, board_.occupied_);
      moves &= ~board_.colours_[state_.active];
      Bitboard captures = moves & board_.colours_[inactive];
      moves &= ~captures;

      mv.setSrc(src);

      for (Position dst : moves)
      {
         mv.setDst(dst);
         writeMove(writer, mv);
      }

      for (Position dst : captures)
      {
         mv.setDst(dst);
         mv.setCapturedPiece(board_.piece(dst).value());
         writeMove(writer, mv);
      }

      mv.clearCapture();
   }
}

//===========================================================================
void MoveGenerator::generateQueenMoves(
   MoveWriter<MoveArray::iterator>& writer) const
{
   Move mv;
   mv.setPiece(Piece::Queen);
   mv.setActive(state_.active);

   Colour inactive = opponent(state_.active);

   for (Position src : pieces(Piece::Queen, state_.active))
   {
      Bitboard moves = getQueenMoves(src, board_.occupied_);
      moves &= ~board_.colours_[state_.active];
      Bitboard captures = moves & board_.colours_[inactive];
      moves &= ~captures;

      mv.setSrc(src);

      for (Position dst : moves)
      {
         mv.setDst(dst);
         writeMove(writer, mv);
      }

      for (Position dst : captures)
      {
         mv.setDst(dst);
         mv.setCapturedPiece(board_.piece(dst).value());
         writeMove(writer, mv);
      }

      mv.clearCapture();
   }
}

//===========================================================================
void MoveGenerator::writeMove(MoveWriter<MoveArray::iterator>& writer,
                              const Move& mv) const
{
   if (!checkMoveLegality_ || isLegalMove(mv))
   {
      writer << mv;
   }
}

//===========================================================================
void MoveGenerator::writePromotions(MoveWriter<MoveArray::iterator>& writer,
                                    Move& mv) const
{
   if (!checkMoveLegality_ || isLegalMove(mv))
   {
      mv.setPromotedPiece(Piece::Knight);
      writer << mv;
      mv.setPromotedPiece(Piece::Bishop);
      writer << mv;
      mv.setPromotedPiece(Piece::Rook);
      writer << mv;
      mv.setPromotedPiece(Piece::Queen);
      writer << mv;
      mv.clearPromotion();
   }
}

//===========================================================================
bool MoveGenerator::isLegalMove(const Move& mv) const
{
   Position kingPos = getKingPosition(state_.active);

   Position src = mv.src();
   Position dst = mv.dst();

   // For castles, ensure that the king start position, end position, and empty
   // spaces in between are not in check.
   if (mv.longCastle())
   {
      static constexpr std::array kLongCastlePos{
         std::array{Position::e1, Position::d1, Position::c1},
         std::array{Position::e8, Position::d8, Position::c8}};

      return std::ranges::none_of(
         kLongCastlePos[state_.active],
         [this](Position pos) { return inCheck(pos, board_); });
   }
   else if (mv.shortCastle())
   {
      static constexpr std::array kShortCastlePos{
         std::array{Position::e1, Position::f1, Position::g1},
         std::array{Position::e8, Position::f8, Position::g8}};

      return std::ranges::none_of(
         kShortCastlePos[state_.active],
         [this](Position pos) { return inCheck(pos, board_); });
   }

   // For all other moves, ensure that the king is not in check after the move.
   Board board{board_};

   // Ensure src is empty and dst is occupied.
   Bitboard srcBB{src};
   Bitboard dstBB{dst};
   board.occupied_ &= ~srcBB;
   board.occupied_ |= dstBB;

   // Ensure no enemy pieces at dst.
   board.pieces_[Piece::Queen] &= ~dstBB;
   board.pieces_[Piece::Rook] &= ~dstBB;
   board.pieces_[Piece::Bishop] &= ~dstBB;
   board.pieces_[Piece::Knight] &= ~dstBB;
   board.pieces_[Piece::Pawn] &= ~dstBB;
   // board.pieces_[Piece::King] &= ~Bitboard{dst}; // impossible

   // Ensure captured en passant pawn is removed.
   if (mv.enPassant())
   {
      Position enPassantCapturePos = enPassantCapturePosition(dst);
      board.occupied_ &= ~Bitboard{enPassantCapturePos};
      board.pieces_[Piece::Pawn] &= ~Bitboard{enPassantCapturePos};
   }

   // Update king pos if moved.
   if (kingPos == src)
   {
      kingPos = dst;
   }

   return !inCheck(kingPos, board);
}

//===========================================================================
bool MoveGenerator::inCheck(Position pos, const Board& board) const
{
   Bitboard threats;

   Colour inactive = opponent(state_.active);

   threats = getQueenMoves(pos, board.occupied_);
   threats &= board.pieces_[Piece::Queen] & board_.colours_[inactive];
   if (threats.any())
   {
      return true;
   }

   threats = getRookMoves(pos, board.occupied_);
   threats &= board.pieces_[Piece::Rook] & board_.colours_[inactive];
   if (threats.any())
   {
      return true;
   }

   threats = getBishopMoves(pos, board.occupied_);
   threats &= board.pieces_[Piece::Bishop] & board_.colours_[inactive];
   if (threats.any())
   {
      return true;
   }

   threats = getKnightMoves(pos);
   threats &= board.pieces_[Piece::Knight] & board_.colours_[inactive];
   if (threats.any())
   {
      return true;
   }

   threats = board.pieces_[Piece::Pawn] & board_.colours_[inactive];
   threats = getPawnAttacks(state_.active, pos, threats);
   if (threats.any())
   {
      return true;
   }

   threats = getKingMoves(pos);
   threats &= board.pieces_[Piece::King] & board_.colours_[inactive];
   if (threats.any())
   {
      return true;
   }

   return false;
}

} // namespace chess
