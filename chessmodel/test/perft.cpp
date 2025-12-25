#include "chessmodel/move.h"

#include <chessmodel/game.h>
#include <gtest/gtest.h>
#include <iostream>

using namespace chess;

namespace
{

std::uint64_t perft(Game& g, int depth)
{
   if (depth == 0)
   {
      return 1;
   }

   uint64_t res{0};

   MoveArray moves;
   int n = g.generate(moves);
   if (depth == 1)
   {
      return n;
   }

   for (int i = 0; i < n; i++)
   {
      g.move(moves[i]);
      res += perft(g, depth - 1);
      g.unmove(moves[i]);
   }

   return res;
}

} // namespace

TEST(PerformanceTest, perft1)
{
   auto g =
      Game::fromFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
   ASSERT_TRUE(g.has_value());

   EXPECT_EQ(perft(g.value(), 0), 1);
   EXPECT_EQ(perft(g.value(), 1), 20);
   EXPECT_EQ(perft(g.value(), 2), 400);
   EXPECT_EQ(perft(g.value(), 3), 8902);
   EXPECT_EQ(perft(g.value(), 4), 197281);
   EXPECT_EQ(perft(g.value(), 5), 4865609); // ~4s
   // EXPECT_EQ(perft(g.value(), 6), 119060324);
   // EXPECT_EQ(perft(g.value(), 7), 3195901860);
}

TEST(PerformanceTest, perft2)
{
   auto g = Game::fromFEN(
      "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");

   ASSERT_TRUE(g.has_value());

   EXPECT_EQ(perft(g.value(), 0), 1);
   EXPECT_EQ(perft(g.value(), 1), 48);
   EXPECT_EQ(perft(g.value(), 2), 2039);
   EXPECT_EQ(perft(g.value(), 3), 97862);
   // EXPECT_EQ(perft(g.value(), 4), 4085603);
   // EXPECT_EQ(perft(g.value(), 5), 197876242);
   // EXPECT_EQ(perft(g.value(), 6), 8229523927);
}

TEST(PerformanceTest, perft3)
{
   auto g = Game::fromFEN("n1n5/PPPk4/8/8/8/8/4Kppp/5N1N b - - 0 1");

   ASSERT_TRUE(g.has_value());

   EXPECT_EQ(perft(g.value(), 0), 1);
   EXPECT_EQ(perft(g.value(), 1), 24);
   EXPECT_EQ(perft(g.value(), 2), 496);
   EXPECT_EQ(perft(g.value(), 3), 9483);
   EXPECT_EQ(perft(g.value(), 4), 182838);
   // EXPECT_EQ(perft(g.value(), 5), 3605103);
   // EXPECT_EQ(perft(g.value(), 6), 71179139);
}
