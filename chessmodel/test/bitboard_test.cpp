#include <chessmodel/bitboard.h>
#include <chessmodel/piece.h>
#include <gtest/gtest.h>

using namespace chess;

namespace
{

static constexpr std::uint64_t kTestBoard =
   0b10000000'01000000'00100000'00010000'00001000'00000100'00000010'00000001;

}

TEST(BitboardTest, construction)
{
   EXPECT_EQ(Bitboard{}.to_uint64(), 0);
   EXPECT_EQ(Bitboard{std::numeric_limits<std::uint64_t>::max()}.to_uint64(),
             std::numeric_limits<std::uint64_t>::max());

   EXPECT_EQ(Bitboard{Position::a1}.to_uint64(), 1);
   EXPECT_EQ(Bitboard{Position::e4}.to_uint64(), 1ull << 28);
}

TEST(BitboardTest, bitwiseOperations)
{
   EXPECT_EQ(Bitboard{1} | Bitboard{2}, Bitboard{3});
   EXPECT_EQ(Bitboard{2} & Bitboard{7}, Bitboard{2});
   EXPECT_EQ(Bitboard{2} ^ Bitboard{7}, Bitboard{5});
   EXPECT_EQ(~Bitboard{0}, Bitboard{std::numeric_limits<std::uint64_t>::max()});
}

TEST(BitboardTest, test)
{
   EXPECT_TRUE(Bitboard{kTestBoard}.test(Position::a1));
   EXPECT_TRUE(Bitboard{kTestBoard}.test(Position::b2));
   EXPECT_TRUE(Bitboard{kTestBoard}.test(Position::g7));
   EXPECT_TRUE(Bitboard{kTestBoard}.test(Position::h8));

   EXPECT_FALSE(Bitboard{kTestBoard}.test(Position::h1));
   EXPECT_FALSE(Bitboard{kTestBoard}.test(Position::g2));
   EXPECT_FALSE(Bitboard{kTestBoard}.test(Position::b7));
   EXPECT_FALSE(Bitboard{kTestBoard}.test(Position::a8));
}

TEST(BitboardTest, count)
{
   EXPECT_EQ(Bitboard{}.count(), 0);
   EXPECT_EQ(Bitboard{kTestBoard}.count(), 8);
   EXPECT_EQ(Bitboard{std::numeric_limits<std::uint64_t>::max()}.count(), 64);
}

TEST(BitboardTest, anyNoneAll)
{
   EXPECT_TRUE(Bitboard{}.none());
   EXPECT_FALSE(Bitboard{}.any());
   EXPECT_FALSE(Bitboard{}.all());

   EXPECT_FALSE(Bitboard{kTestBoard}.none());
   EXPECT_TRUE(Bitboard{kTestBoard}.any());
   EXPECT_FALSE(Bitboard{kTestBoard}.all());

   EXPECT_FALSE(Bitboard{std::numeric_limits<std::uint64_t>::max()}.none());
   EXPECT_TRUE(Bitboard{std::numeric_limits<std::uint64_t>::max()}.any());
   EXPECT_TRUE(Bitboard{std::numeric_limits<std::uint64_t>::max()}.all());
}

TEST(BitboardTest, setResetToggle)
{
   Bitboard b;

   b.set(Position::a1);
   b.set(Position::b2);
   b.set(Position::c3);
   b.set(Position::d4);
   b.toggle(Position::e5);
   b.toggle(Position::f6);
   b.toggle(Position::g7);
   b.toggle(Position::h8);

   EXPECT_EQ(b, Bitboard{kTestBoard});

   b.reset(Position::a1);
   b.reset(Position::b2);
   b.reset(Position::c3);
   b.reset(Position::d4);
   b.toggle(Position::e5);
   b.toggle(Position::f6);
   b.toggle(Position::g7);
   b.toggle(Position::h8);

   EXPECT_EQ(b, Bitboard{});
}

TEST(BitboardTest, clearFill)
{
   Bitboard b;

   b.fill();
   EXPECT_EQ(b, Bitboard{std::numeric_limits<std::uint64_t>::max()});

   b.clear();
   EXPECT_EQ(b, Bitboard{});
}

TEST(BitboardTest, iteration)
{
   Bitboard b;
   EXPECT_EQ(b.begin(), b.end());

   b       = Bitboard{kTestBoard};
   auto it = b.begin();
   EXPECT_NE(it, b.end());

   EXPECT_EQ(*(it), Position::a1);
   EXPECT_EQ(*(++it), Position::b2);
   EXPECT_EQ(*(it++), Position::b2); // post increment
   EXPECT_EQ(*(it), Position::c3);

   EXPECT_EQ(*(++it), Position::d4);
   EXPECT_EQ(*(++it), Position::e5);
   EXPECT_EQ(*(++it), Position::f6);
   EXPECT_EQ(*(++it), Position::g7);
   EXPECT_EQ(*(++it), Position::h8);
   EXPECT_EQ(++it, b.end());
   EXPECT_EQ(++it, b.end()); // inc past end should have no effect
}

TEST(BitboardTest, constevalTest)
{
   auto test = []() consteval {
      Bitboard b1{kTestBoard};
      b1.test(Position::a1);
      b1.fill();

      Bitboard b2;
      b2 |= b1;
      for (Position pos : b1)
         b2.toggle(pos);
   };

   test();
}
