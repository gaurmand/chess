#include "test_helper.h"

#include <chessmodel/generate.h>
#include <gtest/gtest.h>

using namespace chess;

TEST(GenerateLegalTest, kingThreat)
{
   MoveArray moves;
   Board b;
   State s;
   MoveGenerator gen{b, s};
   gen.setSortMovesOnOutput(true);

   {
      b.clear();
      b.set(Position::e2, Piece::King, Colour::White);
      b.set(Position::e4, Piece::King, Colour::Black);

      s.active = Colour::White;

      auto numMoves = gen.exec(Piece::King, moves);
      EXPECT_EQ(numMoves, 5);
      EXPECT_MOVE(
         moves[0], Colour::White, Position::e2, Position::d1, Piece::King);
      EXPECT_MOVE(
         moves[1], Colour::White, Position::e2, Position::d2, Piece::King);
      EXPECT_MOVE(
         moves[2], Colour::White, Position::e2, Position::e1, Piece::King);
      EXPECT_MOVE(
         moves[3], Colour::White, Position::e2, Position::f1, Piece::King);
      EXPECT_MOVE(
         moves[4], Colour::White, Position::e2, Position::f2, Piece::King);
   }

   {
      b.clear();
      b.set(Position::e2, Piece::King, Colour::White);
      b.set(Position::e4, Piece::King, Colour::Black);

      s.active = Colour::Black;

      auto numMoves = gen.exec(Piece::King, moves);
      EXPECT_EQ(numMoves, 5);
      EXPECT_MOVE(
         moves[0], Colour::Black, Position::e4, Position::d4, Piece::King);
      EXPECT_MOVE(
         moves[1], Colour::Black, Position::e4, Position::d5, Piece::King);
      EXPECT_MOVE(
         moves[2], Colour::Black, Position::e4, Position::e5, Piece::King);
      EXPECT_MOVE(
         moves[3], Colour::Black, Position::e4, Position::f4, Piece::King);
      EXPECT_MOVE(
         moves[4], Colour::Black, Position::e4, Position::f5, Piece::King);
   }
}

TEST(GenerateLegalTest, pawnThreat)
{
   MoveArray moves;
   Board b;
   State s;
   MoveGenerator gen{b, s};
   gen.setSortMovesOnOutput(true);

   {
      b.clear();
      b.set(Position::d4, Piece::King, Colour::White);
      b.set(Position::b4, Piece::Pawn, Colour::White);
      b.set(Position::e6, Piece::Pawn, Colour::White);
      b.set(Position::f2, Piece::Pawn, Colour::White);
      b.set(Position::d8, Piece::King, Colour::Black);
      b.set(Position::c5, Piece::Pawn, Colour::Black);
      b.set(Position::f7, Piece::Pawn, Colour::Black);
      b.set(Position::g3, Piece::Pawn, Colour::Black);

      s.active = Colour::White;

      auto numMoves = gen.exec(Piece::Pawn, moves);
      EXPECT_EQ(numMoves, 1);
      EXPECT_CAPTURE(moves[0],
                     Colour::White,
                     Position::b4,
                     Position::c5,
                     Piece::Pawn,
                     Piece::Pawn);
   }

   {
      b.clear();
      b.set(Position::e2, Piece::King, Colour::White);
      b.set(Position::f3, Piece::Pawn, Colour::White);
      b.set(Position::f6, Piece::Pawn, Colour::White);
      b.set(Position::e4, Piece::King, Colour::Black);
      b.set(Position::g4, Piece::Pawn, Colour::Black);
      b.set(Position::g7, Piece::Pawn, Colour::Black);

      s.active = Colour::Black;

      auto numMoves = gen.exec(Piece::Pawn, moves);
      EXPECT_EQ(numMoves, 1);
      EXPECT_CAPTURE(moves[0],
                     Colour::Black,
                     Position::g4,
                     Position::f3,
                     Piece::Pawn,
                     Piece::Pawn);
   }
}

TEST(GenerateLegalTest, knightThreat)
{
   MoveArray moves;
   Board b;
   State s;
   MoveGenerator gen{b, s};
   gen.setSortMovesOnOutput(true);

   {
      b.clear();
      b.set(Position::a1, Piece::King, Colour::White);
      b.set(Position::b4, Piece::Knight, Colour::White);
      b.set(Position::d8, Piece::King, Colour::Black);
      b.set(Position::c2, Piece::Knight, Colour::Black);

      s.active = Colour::White;

      auto numMoves = gen.exec(Piece::Knight, moves);
      EXPECT_EQ(numMoves, 1);
      EXPECT_CAPTURE(moves[0],
                     Colour::White,
                     Position::b4,
                     Position::c2,
                     Piece::Knight,
                     Piece::Knight);
   }

   {
      b.clear();
      b.set(Position::d1, Piece::King, Colour::White);
      b.set(Position::e6, Piece::Knight, Colour::White);
      b.set(Position::d8, Piece::King, Colour::Black);
      b.set(Position::c5, Piece::Knight, Colour::Black);
      b.set(Position::f8, Piece::Knight, Colour::Black);

      s.active = Colour::Black;

      auto numMoves = gen.exec(Piece::Knight, moves);
      EXPECT_EQ(numMoves, 2);
      EXPECT_CAPTURE(moves[0],
                     Colour::Black,
                     Position::c5,
                     Position::e6,
                     Piece::Knight,
                     Piece::Knight);
      EXPECT_CAPTURE(moves[1],
                     Colour::Black,
                     Position::f8,
                     Position::e6,
                     Piece::Knight,
                     Piece::Knight);
   }
}

TEST(GenerateLegalTest, bishopThreat)
{
   MoveArray moves;
   Board b;
   State s;
   MoveGenerator gen{b, s};
   gen.setSortMovesOnOutput(true);

   {
      b.clear();
      b.set(Position::d1, Piece::King, Colour::White);
      b.set(Position::b5, Piece::Bishop, Colour::White);
      b.set(Position::e6, Piece::Bishop, Colour::White);
      b.set(Position::f4, Piece::Bishop, Colour::White);
      b.set(Position::d8, Piece::King, Colour::Black);
      b.set(Position::g4, Piece::Bishop, Colour::Black);

      s.active = Colour::White;

      auto numMoves = gen.exec(Piece::Bishop, moves);
      EXPECT_EQ(numMoves, 2);
      EXPECT_MOVE(
         moves[0], Colour::White, Position::b5, Position::e2, Piece::Bishop);
      EXPECT_CAPTURE(moves[1],
                     Colour::White,
                     Position::e6,
                     Position::g4,
                     Piece::Bishop,
                     Piece::Bishop);
   }

   {
      b.clear();
      b.set(Position::d1, Piece::King, Colour::White);
      b.set(Position::a5, Piece::Bishop, Colour::White);
      b.set(Position::d8, Piece::King, Colour::Black);
      b.set(Position::c7, Piece::Bishop, Colour::Black);

      s.active = Colour::Black;

      auto numMoves = gen.exec(Piece::Bishop, moves);
      EXPECT_EQ(numMoves, 2);
      EXPECT_CAPTURE(moves[0],
                     Colour::Black,
                     Position::c7,
                     Position::a5,
                     Piece::Bishop,
                     Piece::Bishop);
      EXPECT_MOVE(
         moves[1], Colour::Black, Position::c7, Position::b6, Piece::Bishop);
   }
}

TEST(GenerateLegalTest, rookThreat)
{
   MoveArray moves;
   Board b;
   State s;
   MoveGenerator gen{b, s};
   gen.setSortMovesOnOutput(true);

   {
      b.clear();
      b.set(Position::d1, Piece::King, Colour::White);
      b.set(Position::b5, Piece::Rook, Colour::White);
      b.set(Position::f3, Piece::Rook, Colour::White);
      b.set(Position::h7, Piece::Rook, Colour::White);
      b.set(Position::d8, Piece::King, Colour::Black);
      b.set(Position::d5, Piece::Rook, Colour::Black);

      s.active = Colour::White;

      auto numMoves = gen.exec(Piece::Rook, moves);
      EXPECT_EQ(numMoves, 2);
      EXPECT_CAPTURE(moves[0],
                     Colour::White,
                     Position::b5,
                     Position::d5,
                     Piece::Rook,
                     Piece::Rook);
      EXPECT_MOVE(
         moves[1], Colour::White, Position::f3, Position::d3, Piece::Rook);
   }

   {
      b.clear();
      b.set(Position::d1, Piece::King, Colour::White);
      b.set(Position::a8, Piece::Rook, Colour::White);
      b.set(Position::d8, Piece::King, Colour::Black);
      b.set(Position::c8, Piece::Rook, Colour::Black);

      s.active = Colour::Black;

      auto numMoves = gen.exec(Piece::Rook, moves);
      EXPECT_EQ(numMoves, 2);
      EXPECT_CAPTURE(moves[0],
                     Colour::Black,
                     Position::c8,
                     Position::a8,
                     Piece::Rook,
                     Piece::Rook);
      EXPECT_MOVE(
         moves[1], Colour::Black, Position::c8, Position::b8, Piece::Rook);
   }
}

TEST(GenerateLegalTest, queenThreat)
{
   MoveArray moves;
   Board b;
   State s;
   MoveGenerator gen{b, s};
   gen.setSortMovesOnOutput(true);

   {
      b.clear();
      b.set(Position::d1, Piece::King, Colour::White);
      b.set(Position::a3, Piece::Queen, Colour::White);
      b.set(Position::b7, Piece::Queen, Colour::White);
      b.set(Position::f5, Piece::Queen, Colour::White);
      b.set(Position::f2, Piece::Queen, Colour::White);
      b.set(Position::d8, Piece::King, Colour::Black);
      b.set(Position::b3, Piece::Queen, Colour::Black);

      s.active = Colour::White;

      auto numMoves = gen.exec(Piece::Queen, moves);
      EXPECT_EQ(numMoves, 4);
      EXPECT_CAPTURE(moves[0],
                     Colour::White,
                     Position::a3,
                     Position::b3,
                     Piece::Queen,
                     Piece::Queen);
      EXPECT_CAPTURE(moves[1],
                     Colour::White,
                     Position::b7,
                     Position::b3,
                     Piece::Queen,
                     Piece::Queen);
      EXPECT_MOVE(
         moves[2], Colour::White, Position::f2, Position::c2, Piece::Queen);
      EXPECT_MOVE(
         moves[3], Colour::White, Position::f5, Position::c2, Piece::Queen);
   }

   {
      b.clear();
      b.set(Position::d1, Piece::King, Colour::White);
      b.set(Position::a5, Piece::Queen, Colour::White);
      b.set(Position::d4, Piece::Queen, Colour::White);
      b.set(Position::d8, Piece::King, Colour::Black);
      b.set(Position::b6, Piece::Queen, Colour::Black);
      b.set(Position::d5, Piece::Queen, Colour::Black);

      s.active = Colour::Black;

      auto numMoves = gen.exec(Piece::Queen, moves);
      EXPECT_EQ(numMoves, 5);
      EXPECT_CAPTURE(moves[0],
                     Colour::Black,
                     Position::b6,
                     Position::a5,
                     Piece::Queen,
                     Piece::Queen);
      EXPECT_MOVE(
         moves[1], Colour::Black, Position::b6, Position::c7, Piece::Queen);
      EXPECT_CAPTURE(moves[2],
                     Colour::Black,
                     Position::d5,
                     Position::d4,
                     Piece::Queen,
                     Piece::Queen);
      EXPECT_MOVE(
         moves[3], Colour::Black, Position::d5, Position::d6, Piece::Queen);
      EXPECT_MOVE(
         moves[4], Colour::Black, Position::d5, Position::d7, Piece::Queen);
   }
}

TEST(GenerateLegalTest, kingEscapeCheck)
{
   MoveArray moves;
   Board b;
   State s;
   MoveGenerator gen{b, s};
   gen.setSortMovesOnOutput(true);

   {
      b.clear();
      b.set(Position::d1, Piece::King, Colour::White);
      b.set(Position::b1, Piece::Bishop, Colour::White);
      b.set(Position::e2, Piece::Pawn, Colour::White);
      b.set(Position::h1, Piece::Queen, Colour::White);
      b.set(Position::d8, Piece::King, Colour::Black);
      b.set(Position::a1, Piece::Rook, Colour::Black);
      b.set(Position::d3, Piece::Queen, Colour::Black);
      b.set(Position::g2, Piece::Knight, Colour::Black);
      b.set(Position::h5, Piece::Bishop, Colour::Black);

      s.active = Colour::White;

      auto numMoves = gen.exec(moves);
      EXPECT_EQ(numMoves, 1);
      EXPECT_MOVE(
         moves[0], Colour::White, Position::d1, Position::c1, Piece::King);
   }
}

TEST(GenerateLegalTest, pawnPromotionThreat)
{
   MoveArray moves;
   Board b;
   State s;
   MoveGenerator gen{b, s};
   gen.setSortMovesOnOutput(true);

   {
      b.clear();
      b.set(Position::h8, Piece::King, Colour::White);
      b.set(Position::a7, Piece::Pawn, Colour::White);
      b.set(Position::d7, Piece::Pawn, Colour::White);
      b.set(Position::h1, Piece::King, Colour::Black);
      b.set(Position::c8, Piece::Queen, Colour::Black);

      s.active = Colour::White;

      auto numMoves = gen.exec(Piece::Pawn, moves);
      EXPECT_EQ(numMoves, 8);
      EXPECT_CAPTURE_PROMOTION(moves[0],
                               Colour::White,
                               Position::d7,
                               Position::c8,
                               Piece::Queen,
                               Piece::Bishop);
      EXPECT_CAPTURE_PROMOTION(moves[1],
                               Colour::White,
                               Position::d7,
                               Position::c8,
                               Piece::Queen,
                               Piece::Knight);
      EXPECT_CAPTURE_PROMOTION(moves[2],
                               Colour::White,
                               Position::d7,
                               Position::c8,
                               Piece::Queen,
                               Piece::Queen);
      EXPECT_CAPTURE_PROMOTION(moves[3],
                               Colour::White,
                               Position::d7,
                               Position::c8,
                               Piece::Queen,
                               Piece::Rook);
      EXPECT_PROMOTION(
         moves[4], Colour::White, Position::d7, Position::d8, Piece::Bishop);
      EXPECT_PROMOTION(
         moves[5], Colour::White, Position::d7, Position::d8, Piece::Knight);
      EXPECT_PROMOTION(
         moves[6], Colour::White, Position::d7, Position::d8, Piece::Queen);
      EXPECT_PROMOTION(
         moves[7], Colour::White, Position::d7, Position::d8, Piece::Rook);
   }

   {
      b.clear();
      b.set(Position::a1, Piece::King, Colour::White);
      b.set(Position::h1, Piece::Bishop, Colour::White);
      b.set(Position::a8, Piece::King, Colour::Black);
      b.set(Position::g2, Piece::Pawn, Colour::Black);

      s.active = Colour::Black;

      auto numMoves = gen.exec(Piece::Pawn, moves);
      EXPECT_EQ(numMoves, 4);
      EXPECT_CAPTURE_PROMOTION(moves[0],
                               Colour::Black,
                               Position::g2,
                               Position::h1,
                               Piece::Bishop,
                               Piece::Bishop);
      EXPECT_CAPTURE_PROMOTION(moves[1],
                               Colour::Black,
                               Position::g2,
                               Position::h1,
                               Piece::Bishop,
                               Piece::Knight);
      EXPECT_CAPTURE_PROMOTION(moves[2],
                               Colour::Black,
                               Position::g2,
                               Position::h1,
                               Piece::Bishop,
                               Piece::Queen);
      EXPECT_CAPTURE_PROMOTION(moves[3],
                               Colour::Black,
                               Position::g2,
                               Position::h1,
                               Piece::Bishop,
                               Piece::Rook);
   }
}

TEST(GenerateLegalTest, enPassantThreat)
{
   MoveArray moves;
   Board b;
   State s;
   MoveGenerator gen{b, s};
   gen.setSortMovesOnOutput(true);

   {
      b.clear();
      b.set(Position::d4, Piece::King, Colour::White);
      b.set(Position::b5, Piece::Pawn, Colour::White);
      b.set(Position::d5, Piece::Pawn, Colour::White);
      b.set(Position::h8, Piece::King, Colour::Black);
      b.set(Position::c5, Piece::Pawn, Colour::Black);
      b.set(Position::d8, Piece::Rook, Colour::Black);

      s.active    = Colour::White;
      s.enPassant = Position::c6;

      auto numMoves = gen.exec(Piece::Pawn, moves);
      EXPECT_EQ(numMoves, 1);
      EXPECT_ENPASSANT(moves[0], Colour::White, Position::b5, Position::c6);
   }

   {
      b.clear();
      b.set(Position::h3, Piece::King, Colour::Black);
      b.set(Position::b4, Piece::Pawn, Colour::Black);
      b.set(Position::h8, Piece::King, Colour::White);
      b.set(Position::c4, Piece::Pawn, Colour::White);
      b.set(Position::a3, Piece::Rook, Colour::White);

      s.active    = Colour::Black;
      s.enPassant = Position::c3;

      auto numMoves = gen.exec(Piece::Pawn, moves);
      EXPECT_EQ(numMoves, 3);
      EXPECT_MOVE(
         moves[0], Colour::Black, Position::b4, Position::a3, Piece::Pawn);
      EXPECT_MOVE(
         moves[1], Colour::Black, Position::b4, Position::b3, Piece::Pawn);
      EXPECT_ENPASSANT(moves[2], Colour::Black, Position::b4, Position::c3);
   }
}

TEST(GenerateLegalTest, castlingThreat)
{
   MoveArray moves;
   Board b;
   State s;
   MoveGenerator gen{b, s};
   gen.setSortMovesOnOutput(true);

   {
      b.clear();
      b.set(Position::e1, Piece::King, Colour::White);
      b.set(Position::a1, Piece::Rook, Colour::White);
      b.set(Position::h1, Piece::Rook, Colour::White);
      b.set(Position::e8, Piece::King, Colour::Black);
      b.set(Position::e7, Piece::Rook, Colour::Black);

      s.active = Colour::White;

      auto numMoves = gen.exec(Piece::King, moves);
      EXPECT_EQ(numMoves, 4);
      EXPECT_MOVE(
         moves[0], Colour::White, Position::e1, Position::d1, Piece::King);
      EXPECT_MOVE(
         moves[1], Colour::White, Position::e1, Position::d2, Piece::King);
      EXPECT_MOVE(
         moves[2], Colour::White, Position::e1, Position::f1, Piece::King);
      EXPECT_MOVE(
         moves[3], Colour::White, Position::e1, Position::f2, Piece::King);
   }

   {
      b.clear();
      b.set(Position::e1, Piece::King, Colour::White);
      b.set(Position::c7, Piece::Rook, Colour::White);
      b.set(Position::g7, Piece::Bishop, Colour::White);
      b.set(Position::e8, Piece::King, Colour::Black);
      b.set(Position::a8, Piece::Rook, Colour::Black);
      b.set(Position::h8, Piece::Rook, Colour::Black);

      s.active = Colour::Black;

      auto numMoves = gen.exec(Piece::King, moves);
      EXPECT_EQ(numMoves, 1);
      EXPECT_MOVE(
         moves[0], Colour::Black, Position::e8, Position::d8, Piece::King);
   }
}
