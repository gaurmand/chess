#include <chessmodel/board.h>
#include <gtest/gtest.h>

using namespace chess;

TEST(BoardTest, defaultBoard)
{
   Board b;
   EXPECT_TRUE(b.empty());
   EXPECT_FALSE(b.occupied(Position::a1));
   EXPECT_FALSE(b.occupied(Position::b2));
   EXPECT_FALSE(b.occupied(Position::c3));
   EXPECT_FALSE(b.occupied(Position::e4));
   EXPECT_FALSE(b.occupied(Position::f5));
   EXPECT_FALSE(b.occupied(Position::g6));
   EXPECT_FALSE(b.occupied(Position::h7));
   EXPECT_FALSE(b.occupied(Position::h8));
}

TEST(BoardTest, setClear)
{
   Board b;
   b.set(Position::e4, Piece::Pawn, Colour::White);
   EXPECT_TRUE(b.occupied(Position::e4));
   EXPECT_EQ(b.piece(Position::e4), Piece::Pawn);
   EXPECT_EQ(b.colour(Position::e4), Colour::White);
   EXPECT_FALSE(b.empty());

   b.clear(Position::e4, Piece::Pawn, Colour::White);
   EXPECT_FALSE(b.occupied(Position::e4));
   EXPECT_EQ(b.piece(Position::e4), std::nullopt);
   EXPECT_EQ(b.colour(Position::e4), std::nullopt);
   EXPECT_TRUE(b.empty());
}

TEST(BoardTest, clearAll)
{
   Board b;
   b.set(Position::e4, Piece::Pawn, Colour::White);
   b.set(Position::e5, Piece::Queen, Colour::Black);
   b.set(Position::a1, Piece::Rook, Colour::Black);
   b.set(Position::h7, Piece::Bishop, Colour::White);
   EXPECT_FALSE(b.empty());

   b.clear();
   EXPECT_TRUE(b.empty());
}

TEST(BoardTest, move)
{
   Board b;
   b.set(Position::e4, Piece::Pawn, Colour::White);

   b.move(Position::e4, Position::e5, Piece::Pawn, Colour::White);

   EXPECT_FALSE(b.occupied(Position::e4));
   EXPECT_EQ(b.piece(Position::e4), std::nullopt);
   EXPECT_EQ(b.colour(Position::e4), std::nullopt);

   EXPECT_TRUE(b.occupied(Position::e5));
   EXPECT_EQ(b.piece(Position::e5), Piece::Pawn);
   EXPECT_EQ(b.colour(Position::e5), Colour::White);
}

TEST(BoardTest, promote)
{
   Board b;
   b.set(Position::h8, Piece::Pawn, Colour::White);
   b.promote(Position::h8, Piece::Pawn, Piece::Queen);

   EXPECT_TRUE(b.occupied(Position::h8));
   EXPECT_EQ(b.piece(Position::h8), Piece::Queen);
   EXPECT_EQ(b.colour(Position::h8), Colour::White);
}
