#include <chessmodel/piece.h>
#include <chessmodel/position.h>
#include <gtest/gtest.h>

using namespace chess;

TEST(PositionTest, validity)
{
   EXPECT_TRUE(isValid(Position::a1));
   EXPECT_TRUE(isValid(Position::h8));
   EXPECT_FALSE(isValid(static_cast<Position>(-1)));
   EXPECT_FALSE(isValid(static_cast<Position>(64)));
}

TEST(PositionTest, correctValues)
{
   EXPECT_EQ(Position::a1, static_cast<Position>(0));
   EXPECT_EQ(Position::e4, static_cast<Position>(28));
   EXPECT_EQ(Position::h8, static_cast<Position>(63));
}

TEST(PositionTest, orthogonals)
{
   EXPECT_EQ(positionAt(Position::b1, Direction::N), Position::b2);
   EXPECT_EQ(positionAt(Position::b1, Direction::S), Position::Invalid);

   EXPECT_EQ(positionAt(Position::e8, Direction::S), Position::e7);
   EXPECT_EQ(positionAt(Position::e8, Direction::N), Position::Invalid);

   EXPECT_EQ(positionAt(Position::a5, Direction::E), Position::b5);
   EXPECT_EQ(positionAt(Position::a5, Direction::W), Position::Invalid);

   EXPECT_EQ(positionAt(Position::h3, Direction::W), Position::g3);
   EXPECT_EQ(positionAt(Position::h3, Direction::E), Position::Invalid);
}

TEST(PositionTest, diagonals)
{
   EXPECT_EQ(positionAt(Position::h1, Direction::NW), Position::g2);
   EXPECT_EQ(positionAt(Position::h1, Direction::SE), Position::Invalid);

   EXPECT_EQ(positionAt(Position::a1, Direction::NE), Position::b2);
   EXPECT_EQ(positionAt(Position::a1, Direction::SW), Position::Invalid);

   EXPECT_EQ(positionAt(Position::a8, Direction::SE), Position::b7);
   EXPECT_EQ(positionAt(Position::a8, Direction::NW), Position::Invalid);

   EXPECT_EQ(positionAt(Position::h8, Direction::SW), Position::g7);
   EXPECT_EQ(positionAt(Position::h8, Direction::NE), Position::Invalid);
}

TEST(PositionTest, hooks)
{
   EXPECT_EQ(positionAt(Position::b2, Direction::NNW), Position::a4);
   EXPECT_EQ(positionAt(Position::b2, Direction::NNE), Position::c4);
   EXPECT_EQ(positionAt(Position::b2, Direction::ENE), Position::d3);
   EXPECT_EQ(positionAt(Position::b2, Direction::ESE), Position::d1);
   EXPECT_EQ(positionAt(Position::b2, Direction::SSE), Position::Invalid);
   EXPECT_EQ(positionAt(Position::b2, Direction::SSW), Position::Invalid);
   EXPECT_EQ(positionAt(Position::b2, Direction::WSW), Position::Invalid);
   EXPECT_EQ(positionAt(Position::b2, Direction::WNW), Position::Invalid);

   EXPECT_EQ(positionAt(Position::g7, Direction::NNW), Position::Invalid);
   EXPECT_EQ(positionAt(Position::g7, Direction::NNE), Position::Invalid);
   EXPECT_EQ(positionAt(Position::g7, Direction::ENE), Position::Invalid);
   EXPECT_EQ(positionAt(Position::g7, Direction::ESE), Position::Invalid);
   EXPECT_EQ(positionAt(Position::g7, Direction::SSE), Position::h5);
   EXPECT_EQ(positionAt(Position::g7, Direction::SSW), Position::f5);
   EXPECT_EQ(positionAt(Position::g7, Direction::WSW), Position::e6);
   EXPECT_EQ(positionAt(Position::g7, Direction::WNW), Position::e8);
}
