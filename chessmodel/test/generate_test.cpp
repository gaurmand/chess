#include "test_helper.h"

#include <chessmodel/generate.h>
#include <gtest/gtest.h>

using namespace chess;

TEST(GenerateTest, pawnAdvances)
{
   MoveArray moves;
   Board b;
   State s;
   MoveGenerator gen{b, s};
   gen.setSortMovesOnOutput(true);
   gen.setCheckMoveLegality(false);

   {
      b.clear();
      b.set(Position::c3, Piece::Pawn, Colour::White);
      b.set(Position::e4, Piece::Pawn, Colour::White);
      b.set(Position::e5, Piece::Pawn, Colour::Black);
      b.set(Position::c6, Piece::Pawn, Colour::Black);

      s.active = Colour::White;

      auto numMoves = gen.exec(Piece::Pawn, moves);
      EXPECT_EQ(numMoves, 1);
      EXPECT_MOVE(
         moves[0], Colour::White, Position::c3, Position::c4, Piece::Pawn);
   }

   {
      b.clear();
      b.set(Position::c3, Piece::Pawn, Colour::White);
      b.set(Position::e4, Piece::Pawn, Colour::White);
      b.set(Position::e5, Piece::Pawn, Colour::Black);
      b.set(Position::c6, Piece::Pawn, Colour::Black);

      s.active = Colour::Black;

      auto numMoves = gen.exec(Piece::Pawn, moves);
      EXPECT_EQ(numMoves, 1);
      EXPECT_MOVE(
         moves[0], Colour::Black, Position::c6, Position::c5, Piece::Pawn);
   }
}

TEST(GenerateTest, pawnDoubleAdvances)
{
   MoveArray moves;
   Board b;
   State s;
   MoveGenerator gen{b, s};
   gen.setSortMovesOnOutput(true);
   gen.setCheckMoveLegality(false);

   {
      b.clear();
      b.set(Position::b2, Piece::Pawn, Colour::White);
      b.set(Position::d2, Piece::Pawn, Colour::White);
      b.set(Position::f2, Piece::Pawn, Colour::White);
      b.set(Position::d3, Piece::Pawn, Colour::Black);
      b.set(Position::f4, Piece::Pawn, Colour::Black);

      s.active = Colour::White;

      auto numMoves = gen.exec(Piece::Pawn, moves);
      EXPECT_EQ(numMoves, 3);
      EXPECT_MOVE(
         moves[0], Colour::White, Position::b2, Position::b3, Piece::Pawn);
      EXPECT_MOVE(
         moves[1], Colour::White, Position::b2, Position::b4, Piece::Pawn);
      EXPECT_MOVE(
         moves[2], Colour::White, Position::f2, Position::f3, Piece::Pawn);
   }

   {
      b.clear();
      b.set(Position::b7, Piece::Pawn, Colour::Black);
      b.set(Position::d7, Piece::Pawn, Colour::Black);
      b.set(Position::f7, Piece::Pawn, Colour::Black);
      b.set(Position::d6, Piece::Pawn, Colour::White);
      b.set(Position::f5, Piece::Pawn, Colour::White);

      s.active = Colour::Black;

      auto numMoves = gen.exec(Piece::Pawn, moves);
      EXPECT_EQ(numMoves, 3);
      EXPECT_MOVE(
         moves[0], Colour::Black, Position::b7, Position::b5, Piece::Pawn);
      EXPECT_MOVE(
         moves[1], Colour::Black, Position::b7, Position::b6, Piece::Pawn);
      EXPECT_MOVE(
         moves[2], Colour::Black, Position::f7, Position::f6, Piece::Pawn);
   }
}

TEST(GenerateTest, pawnCapture)
{
   MoveArray moves;
   Board b;
   State s;
   MoveGenerator gen{b, s};
   gen.setSortMovesOnOutput(true);
   gen.setCheckMoveLegality(false);

   {
      b.clear();
      b.set(Position::b2, Piece::Pawn, Colour::White);
      b.set(Position::f3, Piece::Pawn, Colour::White);
      b.set(Position::c3, Piece::Pawn, Colour::Black);
      b.set(Position::f4, Piece::Pawn, Colour::Black);
      b.set(Position::g4, Piece::Pawn, Colour::Black);
      s.active = Colour::White;

      auto numMoves = gen.exec(Piece::Pawn, moves);
      EXPECT_EQ(numMoves, 4);
      EXPECT_MOVE(
         moves[0], Colour::White, Position::b2, Position::b3, Piece::Pawn);
      EXPECT_MOVE(
         moves[1], Colour::White, Position::b2, Position::b4, Piece::Pawn);
      EXPECT_CAPTURE(moves[2],
                     Colour::White,
                     Position::b2,
                     Position::c3,
                     Piece::Pawn,
                     Piece::Pawn);
      EXPECT_CAPTURE(moves[3],
                     Colour::White,
                     Position::f3,
                     Position::g4,
                     Piece::Pawn,
                     Piece::Pawn);
   }

   {
      b.clear();
      b.set(Position::b7, Piece::Pawn, Colour::Black);
      b.set(Position::f6, Piece::Pawn, Colour::Black);
      b.set(Position::c6, Piece::Pawn, Colour::White);
      b.set(Position::f5, Piece::Pawn, Colour::White);
      b.set(Position::g5, Piece::Pawn, Colour::White);
      s.active = Colour::Black;

      auto numMoves = gen.exec(Piece::Pawn, moves);
      EXPECT_EQ(numMoves, 4);
      EXPECT_MOVE(
         moves[0], Colour::Black, Position::b7, Position::b5, Piece::Pawn);
      EXPECT_MOVE(
         moves[1], Colour::Black, Position::b7, Position::b6, Piece::Pawn);
      EXPECT_CAPTURE(moves[2],
                     Colour::Black,
                     Position::b7,
                     Position::c6,
                     Piece::Pawn,
                     Piece::Pawn);
      EXPECT_CAPTURE(moves[3],
                     Colour::Black,
                     Position::f6,
                     Position::g5,
                     Piece::Pawn,
                     Piece::Pawn);
   }
}

TEST(GenerateTest, kingMoves)
{
   MoveArray moves;
   Board b;
   State s;
   MoveGenerator gen{b, s};
   gen.setSortMovesOnOutput(true);
   gen.setCheckMoveLegality(false);

   {
      b.clear();
      b.set(Position::e1, Piece::King, Colour::White);
      b.set(Position::e2, Piece::Bishop, Colour::White);
      b.set(Position::f2, Piece::Pawn, Colour::Black);
      s.active = Colour::White;

      auto numMoves = gen.exec(Piece::King, moves);
      EXPECT_EQ(numMoves, 4);
      EXPECT_MOVE(
         moves[0], Colour::White, Position::e1, Position::d1, Piece::King);
      EXPECT_MOVE(
         moves[1], Colour::White, Position::e1, Position::d2, Piece::King);
      EXPECT_MOVE(
         moves[2], Colour::White, Position::e1, Position::f1, Piece::King);
      EXPECT_CAPTURE(moves[3],
                     Colour::White,
                     Position::e1,
                     Position::f2,
                     Piece::King,
                     Piece::Pawn);
   }

   {
      b.clear();
      b.set(Position::b7, Piece::King, Colour::Black);
      b.set(Position::a8, Piece::Queen, Colour::White);
      b.set(Position::b6, Piece::Knight, Colour::Black);
      s.active = Colour::Black;

      auto numMoves = gen.exec(Piece::King, moves);
      EXPECT_EQ(numMoves, 7);
      EXPECT_MOVE(
         moves[0], Colour::Black, Position::b7, Position::a6, Piece::King);
      EXPECT_MOVE(
         moves[1], Colour::Black, Position::b7, Position::a7, Piece::King);
      EXPECT_CAPTURE(moves[2],
                     Colour::Black,
                     Position::b7,
                     Position::a8,
                     Piece::King,
                     Piece::Queen);
      EXPECT_MOVE(
         moves[3], Colour::Black, Position::b7, Position::b8, Piece::King);
      EXPECT_MOVE(
         moves[4], Colour::Black, Position::b7, Position::c6, Piece::King);
      EXPECT_MOVE(
         moves[5], Colour::Black, Position::b7, Position::c7, Piece::King);
      EXPECT_MOVE(
         moves[6], Colour::Black, Position::b7, Position::c8, Piece::King);
   }
}

TEST(GenerateTest, knightMoves)
{
   MoveArray moves;
   Board b;
   State s;
   MoveGenerator gen{b, s};
   gen.setSortMovesOnOutput(true);
   gen.setCheckMoveLegality(false);

   {
      b.clear();
      b.set(Position::a3, Piece::Knight, Colour::White);
      b.set(Position::a2, Piece::Pawn, Colour::White);
      b.set(Position::b2, Piece::Pawn, Colour::White);
      b.set(Position::b1, Piece::Bishop, Colour::Black);
      b.set(Position::c4, Piece::Queen, Colour::White);
      s.active = Colour::White;

      auto numMoves = gen.exec(Piece::Knight, moves);
      EXPECT_EQ(numMoves, 3);
      EXPECT_CAPTURE(moves[0],
                     Colour::White,
                     Position::a3,
                     Position::b1,
                     Piece::Knight,
                     Piece::Bishop);
      EXPECT_MOVE(
         moves[1], Colour::White, Position::a3, Position::b5, Piece::Knight);
      EXPECT_MOVE(
         moves[2], Colour::White, Position::a3, Position::c2, Piece::Knight);
   }

   {
      b.clear();
      b.set(Position::e4, Piece::Knight, Colour::Black);
      b.set(Position::d2, Piece::Pawn, Colour::White);
      b.set(Position::e5, Piece::Pawn, Colour::White);
      b.set(Position::f5, Piece::Pawn, Colour::White);
      b.set(Position::c5, Piece::Rook, Colour::Black);
      s.active = Colour::Black;

      auto numMoves = gen.exec(Piece::Knight, moves);
      EXPECT_EQ(numMoves, 7);
      EXPECT_MOVE(
         moves[0], Colour::Black, Position::e4, Position::c3, Piece::Knight);
      EXPECT_CAPTURE(moves[1],
                     Colour::Black,
                     Position::e4,
                     Position::d2,
                     Piece::Knight,
                     Piece::Pawn);
      EXPECT_MOVE(
         moves[2], Colour::Black, Position::e4, Position::d6, Piece::Knight);
      EXPECT_MOVE(
         moves[3], Colour::Black, Position::e4, Position::f2, Piece::Knight);
      EXPECT_MOVE(
         moves[4], Colour::Black, Position::e4, Position::f6, Piece::Knight);
      EXPECT_MOVE(
         moves[5], Colour::Black, Position::e4, Position::g3, Piece::Knight);
      EXPECT_MOVE(
         moves[6], Colour::Black, Position::e4, Position::g5, Piece::Knight);
   }
}

TEST(GenerateTest, bishopMoves)
{
   MoveArray moves;
   Board b;
   State s;
   MoveGenerator gen{b, s};
   gen.setSortMovesOnOutput(true);
   gen.setCheckMoveLegality(false);

   {
      b.clear();
      b.set(Position::c1, Piece::Bishop, Colour::White);
      b.set(Position::a3, Piece::Pawn, Colour::White);
      b.set(Position::e3, Piece::Queen, Colour::Black);
      s.active = Colour::White;

      auto numMoves = gen.exec(Piece::Bishop, moves);
      EXPECT_EQ(numMoves, 3);
      EXPECT_MOVE(
         moves[0], Colour::White, Position::c1, Position::b2, Piece::Bishop);
      EXPECT_MOVE(
         moves[1], Colour::White, Position::c1, Position::d2, Piece::Bishop);
      EXPECT_CAPTURE(moves[2],
                     Colour::White,
                     Position::c1,
                     Position::e3,
                     Piece::Bishop,
                     Piece::Queen);
   }

   {
      b.clear();
      b.set(Position::d5, Piece::Bishop, Colour::Black);
      b.set(Position::c4, Piece::Pawn, Colour::White);
      b.set(Position::a8, Piece::Queen, Colour::Black);
      b.set(Position::f3, Piece::Pawn, Colour::Black);
      s.active = Colour::Black;

      auto numMoves = gen.exec(Piece::Bishop, moves);
      EXPECT_EQ(numMoves, 7);
      EXPECT_MOVE(
         moves[0], Colour::Black, Position::d5, Position::b7, Piece::Bishop);
      EXPECT_CAPTURE(moves[1],
                     Colour::Black,
                     Position::d5,
                     Position::c4,
                     Piece::Bishop,
                     Piece::Pawn);
      EXPECT_MOVE(
         moves[2], Colour::Black, Position::d5, Position::c6, Piece::Bishop);
      EXPECT_MOVE(
         moves[3], Colour::Black, Position::d5, Position::e4, Piece::Bishop);
      EXPECT_MOVE(
         moves[4], Colour::Black, Position::d5, Position::e6, Piece::Bishop);
      EXPECT_MOVE(
         moves[5], Colour::Black, Position::d5, Position::f7, Piece::Bishop);
      EXPECT_MOVE(
         moves[6], Colour::Black, Position::d5, Position::g8, Piece::Bishop);
   }
}

TEST(GenerateTest, rookMoves)
{
   MoveArray moves;
   Board b;
   State s;
   MoveGenerator gen{b, s};
   gen.setSortMovesOnOutput(true);
   gen.setCheckMoveLegality(false);

   {
      b.clear();
      b.set(Position::a1, Piece::Rook, Colour::White);
      b.set(Position::b2, Piece::Pawn, Colour::White);
      b.set(Position::c1, Piece::Queen, Colour::White);
      b.set(Position::a3, Piece::Rook, Colour::Black);
      s.active = Colour::White;

      auto numMoves = gen.exec(Piece::Rook, moves);
      EXPECT_EQ(numMoves, 3);
      EXPECT_MOVE(
         moves[0], Colour::White, Position::a1, Position::a2, Piece::Rook);
      EXPECT_CAPTURE(moves[1],
                     Colour::White,
                     Position::a1,
                     Position::a3,
                     Piece::Rook,
                     Piece::Rook);
      EXPECT_MOVE(
         moves[2], Colour::White, Position::a1, Position::b1, Piece::Rook);
   }

   {
      b.clear();
      b.set(Position::e5, Piece::Rook, Colour::Black);
      b.set(Position::e3, Piece::Pawn, Colour::White);
      b.set(Position::a5, Piece::Queen, Colour::Black);
      b.set(Position::e7, Piece::Pawn, Colour::Black);
      s.active = Colour::Black;

      auto numMoves = gen.exec(Piece::Rook, moves);
      EXPECT_EQ(numMoves, 9);
      EXPECT_MOVE(
         moves[0], Colour::Black, Position::e5, Position::b5, Piece::Rook);
      EXPECT_MOVE(
         moves[1], Colour::Black, Position::e5, Position::c5, Piece::Rook);
      EXPECT_MOVE(
         moves[2], Colour::Black, Position::e5, Position::d5, Piece::Rook);
      EXPECT_CAPTURE(moves[3],
                     Colour::Black,
                     Position::e5,
                     Position::e3,
                     Piece::Rook,
                     Piece::Pawn);
      EXPECT_MOVE(
         moves[4], Colour::Black, Position::e5, Position::e4, Piece::Rook);
      EXPECT_MOVE(
         moves[5], Colour::Black, Position::e5, Position::e6, Piece::Rook);
      EXPECT_MOVE(
         moves[6], Colour::Black, Position::e5, Position::f5, Piece::Rook);
      EXPECT_MOVE(
         moves[7], Colour::Black, Position::e5, Position::g5, Piece::Rook);
      EXPECT_MOVE(
         moves[8], Colour::Black, Position::e5, Position::h5, Piece::Rook);
   }
}

TEST(GenerateTest, queenMoves)
{
   MoveArray moves;
   Board b;
   State s;
   MoveGenerator gen{b, s};
   gen.setSortMovesOnOutput(true);
   gen.setCheckMoveLegality(false);

   {
      b.clear();
      b.set(Position::a3, Piece::Queen, Colour::White);
      b.set(Position::c3, Piece::Pawn, Colour::White);
      b.set(Position::c1, Piece::King, Colour::White);
      b.set(Position::c5, Piece::Queen, Colour::Black);
      b.set(Position::a6, Piece::Pawn, Colour::Black);
      s.active = Colour::White;

      auto numMoves = gen.exec(Piece::Queen, moves);
      EXPECT_EQ(numMoves, 9);
      EXPECT_MOVE(
         moves[0], Colour::White, Position::a3, Position::a1, Piece::Queen);
      EXPECT_MOVE(
         moves[1], Colour::White, Position::a3, Position::a2, Piece::Queen);
      EXPECT_MOVE(
         moves[2], Colour::White, Position::a3, Position::a4, Piece::Queen);
      EXPECT_MOVE(
         moves[3], Colour::White, Position::a3, Position::a5, Piece::Queen);
      EXPECT_CAPTURE(moves[4],
                     Colour::White,
                     Position::a3,
                     Position::a6,
                     Piece::Queen,
                     Piece::Pawn);
      EXPECT_MOVE(
         moves[5], Colour::White, Position::a3, Position::b2, Piece::Queen);
      EXPECT_MOVE(
         moves[6], Colour::White, Position::a3, Position::b3, Piece::Queen);
      EXPECT_MOVE(
         moves[7], Colour::White, Position::a3, Position::b4, Piece::Queen);
      EXPECT_CAPTURE(moves[8],
                     Colour::White,
                     Position::a3,
                     Position::c5,
                     Piece::Queen,
                     Piece::Queen);
   }

   {
      b.clear();
      b.set(Position::e5, Piece::Queen, Colour::Black);
      b.set(Position::f4, Piece::Knight, Colour::White);
      b.set(Position::h8, Piece::Rook, Colour::Black);
      b.set(Position::c7, Piece::Pawn, Colour::Black);
      b.set(Position::e6, Piece::Pawn, Colour::Black);
      b.set(Position::c5, Piece::Bishop, Colour::White);
      b.set(Position::e1, Piece::Queen, Colour::White);
      s.active = Colour::Black;

      auto numMoves = gen.exec(Piece::Queen, moves);
      EXPECT_EQ(numMoves, 17);
      EXPECT_MOVE(
         moves[0], Colour::Black, Position::e5, Position::a1, Piece::Queen);
      EXPECT_MOVE(
         moves[1], Colour::Black, Position::e5, Position::b2, Piece::Queen);
      EXPECT_MOVE(
         moves[2], Colour::Black, Position::e5, Position::c3, Piece::Queen);
      EXPECT_CAPTURE(moves[3],
                     Colour::Black,
                     Position::e5,
                     Position::c5,
                     Piece::Queen,
                     Piece::Bishop);
      EXPECT_MOVE(
         moves[4], Colour::Black, Position::e5, Position::d4, Piece::Queen);
      EXPECT_MOVE(
         moves[5], Colour::Black, Position::e5, Position::d5, Piece::Queen);
      EXPECT_MOVE(
         moves[6], Colour::Black, Position::e5, Position::d6, Piece::Queen);
      EXPECT_CAPTURE(moves[7],
                     Colour::Black,
                     Position::e5,
                     Position::e1,
                     Piece::Queen,
                     Piece::Queen);
      EXPECT_MOVE(
         moves[8], Colour::Black, Position::e5, Position::e2, Piece::Queen);
      EXPECT_MOVE(
         moves[9], Colour::Black, Position::e5, Position::e3, Piece::Queen);
      EXPECT_MOVE(
         moves[10], Colour::Black, Position::e5, Position::e4, Piece::Queen);
      EXPECT_CAPTURE(moves[11],
                     Colour::Black,
                     Position::e5,
                     Position::f4,
                     Piece::Queen,
                     Piece::Knight);
      EXPECT_MOVE(
         moves[12], Colour::Black, Position::e5, Position::f5, Piece::Queen);
      EXPECT_MOVE(
         moves[13], Colour::Black, Position::e5, Position::f6, Piece::Queen);
      EXPECT_MOVE(
         moves[14], Colour::Black, Position::e5, Position::g5, Piece::Queen);
      EXPECT_MOVE(
         moves[15], Colour::Black, Position::e5, Position::g7, Piece::Queen);
      EXPECT_MOVE(
         moves[16], Colour::Black, Position::e5, Position::h5, Piece::Queen);
   }
}

TEST(GenerateTest, pawnPromotion)
{
   MoveArray moves;
   Board b;
   State s;
   MoveGenerator gen{b, s};
   gen.setSortMovesOnOutput(true);
   gen.setCheckMoveLegality(false);

   {
      b.clear();
      b.set(Position::b7, Piece::Pawn, Colour::White);
      b.set(Position::a8, Piece::Rook, Colour::Black);
      s.active = Colour::White;

      auto numMoves = gen.exec(Piece::Pawn, moves);
      EXPECT_EQ(numMoves, 8);
      EXPECT_CAPTURE_PROMOTION(moves[0],
                               Colour::White,
                               Position::b7,
                               Position::a8,
                               Piece::Rook,
                               Piece::Bishop);
      EXPECT_CAPTURE_PROMOTION(moves[1],
                               Colour::White,
                               Position::b7,
                               Position::a8,
                               Piece::Rook,
                               Piece::Knight);
      EXPECT_CAPTURE_PROMOTION(moves[2],
                               Colour::White,
                               Position::b7,
                               Position::a8,
                               Piece::Rook,
                               Piece::Queen);
      EXPECT_CAPTURE_PROMOTION(moves[3],
                               Colour::White,
                               Position::b7,
                               Position::a8,
                               Piece::Rook,
                               Piece::Rook);
      EXPECT_PROMOTION(
         moves[4], Colour::White, Position::b7, Position::b8, Piece::Bishop);
      EXPECT_PROMOTION(
         moves[5], Colour::White, Position::b7, Position::b8, Piece::Knight);
      EXPECT_PROMOTION(
         moves[6], Colour::White, Position::b7, Position::b8, Piece::Queen);
      EXPECT_PROMOTION(
         moves[7], Colour::White, Position::b7, Position::b8, Piece::Rook);
   }

   {
      b.clear();
      b.set(Position::e2, Piece::Pawn, Colour::Black);
      b.set(Position::d1, Piece::Knight, Colour::White);
      b.set(Position::e1, Piece::Queen, Colour::White);
      b.set(Position::f1, Piece::Bishop, Colour::White);
      s.active = Colour::Black;

      auto numMoves = gen.exec(Piece::Pawn, moves);
      EXPECT_EQ(numMoves, 8);
      EXPECT_CAPTURE_PROMOTION(moves[0],
                               Colour::Black,
                               Position::e2,
                               Position::d1,
                               Piece::Knight,
                               Piece::Bishop);
      EXPECT_CAPTURE_PROMOTION(moves[1],
                               Colour::Black,
                               Position::e2,
                               Position::d1,
                               Piece::Knight,
                               Piece::Knight);
      EXPECT_CAPTURE_PROMOTION(moves[2],
                               Colour::Black,
                               Position::e2,
                               Position::d1,
                               Piece::Knight,
                               Piece::Queen);
      EXPECT_CAPTURE_PROMOTION(moves[3],
                               Colour::Black,
                               Position::e2,
                               Position::d1,
                               Piece::Knight,
                               Piece::Rook);
      EXPECT_CAPTURE_PROMOTION(moves[4],
                               Colour::Black,
                               Position::e2,
                               Position::f1,
                               Piece::Bishop,
                               Piece::Bishop);
      EXPECT_CAPTURE_PROMOTION(moves[5],
                               Colour::Black,
                               Position::e2,
                               Position::f1,
                               Piece::Bishop,
                               Piece::Knight);
      EXPECT_CAPTURE_PROMOTION(moves[6],
                               Colour::Black,
                               Position::e2,
                               Position::f1,
                               Piece::Bishop,
                               Piece::Queen);
      EXPECT_CAPTURE_PROMOTION(moves[7],
                               Colour::Black,
                               Position::e2,
                               Position::f1,
                               Piece::Bishop,
                               Piece::Rook);
   }
}

TEST(GenerateTest, enPassant)
{
   MoveArray moves;
   Board b;
   State s;
   MoveGenerator gen{b, s};
   gen.setSortMovesOnOutput(true);
   gen.setCheckMoveLegality(false);

   {
      b.clear();
      b.set(Position::b5, Piece::Pawn, Colour::White);
      b.set(Position::d5, Piece::Pawn, Colour::White);
      b.set(Position::c5, Piece::Pawn, Colour::Black);
      s.active    = Colour::White;
      s.enPassant = Position::c6;

      auto numMoves = gen.exec(Piece::Pawn, moves);
      EXPECT_EQ(numMoves, 4);
      EXPECT_MOVE(
         moves[0], Colour::White, Position::b5, Position::b6, Piece::Pawn);
      EXPECT_ENPASSANT(moves[1], Colour::White, Position::b5, Position::c6);
      EXPECT_ENPASSANT(moves[2], Colour::White, Position::d5, Position::c6);
      EXPECT_MOVE(
         moves[3], Colour::White, Position::d5, Position::d6, Piece::Pawn);
   }

   {
      b.clear();
      b.set(Position::b4, Piece::Pawn, Colour::Black);
      b.set(Position::b3, Piece::Pawn, Colour::White);
      b.set(Position::c4, Piece::Pawn, Colour::White);
      s.active    = Colour::Black;
      s.enPassant = Position::c3;

      auto numMoves = gen.exec(Piece::Pawn, moves);
      EXPECT_EQ(numMoves, 1);
      EXPECT_ENPASSANT(moves[0], Colour::Black, Position::b4, Position::c3);
   }
}

TEST(GenerateTest, castling)
{
   MoveArray moves;
   Board b;
   State s;
   MoveGenerator gen{b, s};
   gen.setSortMovesOnOutput(true);
   gen.setCheckMoveLegality(false);

   {
      b.clear();
      b.set(Position::e1, Piece::King, Colour::White);
      b.set(Position::a1, Piece::Rook, Colour::White);
      b.set(Position::h1, Piece::Rook, Colour::White);
      b.set(Position::e8, Piece::King, Colour::Black);
      b.set(Position::a8, Piece::Rook, Colour::Black);
      b.set(Position::h8, Piece::Rook, Colour::Black);

      s.longCastle[Colour::White]  = true;
      s.longCastle[Colour::Black]  = true;
      s.shortCastle[Colour::White] = true;
      s.shortCastle[Colour::Black] = true;

      s.active = Colour::White;

      auto numMoves = gen.exec(Piece::King, moves);
      EXPECT_EQ(numMoves, 7);
      EXPECT_LONG_CASTLE(moves[0], Colour::White, Position::e1, Position::c1);
      EXPECT_MOVE(
         moves[1], Colour::White, Position::e1, Position::d1, Piece::King);
      EXPECT_MOVE(
         moves[2], Colour::White, Position::e1, Position::d2, Piece::King);
      EXPECT_MOVE(
         moves[3], Colour::White, Position::e1, Position::e2, Piece::King);
      EXPECT_MOVE(
         moves[4], Colour::White, Position::e1, Position::f1, Piece::King);
      EXPECT_MOVE(
         moves[5], Colour::White, Position::e1, Position::f2, Piece::King);
      EXPECT_SHORT_CASTLE(moves[6], Colour::White, Position::e1, Position::g1);

      s.active = Colour::Black;

      numMoves = gen.exec(Piece::King, moves);
      EXPECT_EQ(numMoves, 7);
      EXPECT_LONG_CASTLE(moves[0], Colour::Black, Position::e8, Position::c8);
      EXPECT_MOVE(
         moves[1], Colour::Black, Position::e8, Position::d7, Piece::King);
      EXPECT_MOVE(
         moves[2], Colour::Black, Position::e8, Position::d8, Piece::King);
      EXPECT_MOVE(
         moves[3], Colour::Black, Position::e8, Position::e7, Piece::King);
      EXPECT_MOVE(
         moves[4], Colour::Black, Position::e8, Position::f7, Piece::King);
      EXPECT_MOVE(
         moves[5], Colour::Black, Position::e8, Position::f8, Piece::King);
      EXPECT_SHORT_CASTLE(moves[6], Colour::Black, Position::e8, Position::g8);
   }

   {
      b.clear();
      b.set(Position::e1, Piece::King, Colour::White);
      b.set(Position::a1, Piece::Rook, Colour::White);
      b.set(Position::h1, Piece::Rook, Colour::White);
      b.set(Position::e8, Piece::King, Colour::Black);
      b.set(Position::a8, Piece::Rook, Colour::Black);
      b.set(Position::h8, Piece::Rook, Colour::Black);

      s.longCastle[Colour::White]  = false;
      s.longCastle[Colour::Black]  = false;
      s.shortCastle[Colour::White] = false;
      s.shortCastle[Colour::Black] = false;

      s.active = Colour::White;

      auto numMoves = gen.exec(Piece::King, moves);
      EXPECT_EQ(numMoves, 5);
      EXPECT_MOVE(
         moves[0], Colour::White, Position::e1, Position::d1, Piece::King);
      EXPECT_MOVE(
         moves[1], Colour::White, Position::e1, Position::d2, Piece::King);
      EXPECT_MOVE(
         moves[2], Colour::White, Position::e1, Position::e2, Piece::King);
      EXPECT_MOVE(
         moves[3], Colour::White, Position::e1, Position::f1, Piece::King);
      EXPECT_MOVE(
         moves[4], Colour::White, Position::e1, Position::f2, Piece::King);

      s.active = Colour::Black;

      numMoves = gen.exec(Piece::King, moves);
      EXPECT_EQ(numMoves, 5);
      EXPECT_MOVE(
         moves[0], Colour::Black, Position::e8, Position::d7, Piece::King);
      EXPECT_MOVE(
         moves[1], Colour::Black, Position::e8, Position::d8, Piece::King);
      EXPECT_MOVE(
         moves[2], Colour::Black, Position::e8, Position::e7, Piece::King);
      EXPECT_MOVE(
         moves[3], Colour::Black, Position::e8, Position::f7, Piece::King);
      EXPECT_MOVE(
         moves[4], Colour::Black, Position::e8, Position::f8, Piece::King);
   }

   {
      b.clear();
      b.set(Position::e1, Piece::King, Colour::White);
      b.set(Position::a1, Piece::Rook, Colour::White);
      b.set(Position::h1, Piece::Rook, Colour::White);
      b.set(Position::c1, Piece::Queen, Colour::White);
      b.set(Position::f8, Piece::Queen, Colour::White);
      b.set(Position::e8, Piece::King, Colour::Black);
      b.set(Position::a8, Piece::Rook, Colour::Black);
      b.set(Position::h8, Piece::Rook, Colour::Black);
      b.set(Position::c8, Piece::Queen, Colour::Black);
      b.set(Position::f1, Piece::Queen, Colour::Black);

      s.longCastle[Colour::White]  = true;
      s.longCastle[Colour::Black]  = true;
      s.shortCastle[Colour::White] = true;
      s.shortCastle[Colour::Black] = true;

      s.active = Colour::White;

      auto numMoves = gen.exec(Piece::King, moves);
      EXPECT_EQ(numMoves, 5);
      EXPECT_MOVE(
         moves[0], Colour::White, Position::e1, Position::d1, Piece::King);
      EXPECT_MOVE(
         moves[1], Colour::White, Position::e1, Position::d2, Piece::King);
      EXPECT_MOVE(
         moves[2], Colour::White, Position::e1, Position::e2, Piece::King);
      EXPECT_CAPTURE(moves[3],
                     Colour::White,
                     Position::e1,
                     Position::f1,
                     Piece::King,
                     Piece::Queen);
      EXPECT_MOVE(
         moves[4], Colour::White, Position::e1, Position::f2, Piece::King);

      s.active = Colour::Black;

      numMoves = gen.exec(Piece::King, moves);
      EXPECT_EQ(numMoves, 5);
      EXPECT_MOVE(
         moves[0], Colour::Black, Position::e8, Position::d7, Piece::King);
      EXPECT_MOVE(
         moves[1], Colour::Black, Position::e8, Position::d8, Piece::King);
      EXPECT_MOVE(
         moves[2], Colour::Black, Position::e8, Position::e7, Piece::King);
      EXPECT_MOVE(
         moves[3], Colour::Black, Position::e8, Position::f7, Piece::King);
      EXPECT_CAPTURE(moves[4],
                     Colour::Black,
                     Position::e8,
                     Position::f8,
                     Piece::King,
                     Piece::Queen);
   }
}
