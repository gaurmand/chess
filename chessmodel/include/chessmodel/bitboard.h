#ifndef CHESSMODEL_BITBOARD_H
#define CHESSMODEL_BITBOARD_H

#include <array>
#include <cassert>
#include <chessmodel/position.h>
#include <cstdint>
#include <limits>

//===========================================================================
namespace chess
{

//===========================================================================
namespace internal
{

//===========================================================================
constexpr uint64_t kEmptyBoard = 0;
constexpr uint64_t kFullBoard  = std::numeric_limits<std::uint64_t>::max();

//===========================================================================
constexpr uint64_t toBoardInt(Position pos)
{
   static constexpr std::array<uint64_t, 64> kPositionMask = {
      1ull << 0,  1ull << 1,  1ull << 2,  1ull << 3,  1ull << 4,  1ull << 5,
      1ull << 6,  1ull << 7,  1ull << 8,  1ull << 9,  1ull << 10, 1ull << 11,
      1ull << 12, 1ull << 13, 1ull << 14, 1ull << 15, 1ull << 16, 1ull << 17,
      1ull << 18, 1ull << 19, 1ull << 20, 1ull << 21, 1ull << 22, 1ull << 23,
      1ull << 24, 1ull << 25, 1ull << 26, 1ull << 27, 1ull << 28, 1ull << 29,
      1ull << 30, 1ull << 31, 1ull << 32, 1ull << 33, 1ull << 34, 1ull << 35,
      1ull << 36, 1ull << 37, 1ull << 38, 1ull << 39, 1ull << 40, 1ull << 41,
      1ull << 42, 1ull << 43, 1ull << 44, 1ull << 45, 1ull << 46, 1ull << 47,
      1ull << 48, 1ull << 49, 1ull << 50, 1ull << 51, 1ull << 52, 1ull << 53,
      1ull << 54, 1ull << 55, 1ull << 56, 1ull << 57, 1ull << 58, 1ull << 59,
      1ull << 60, 1ull << 61, 1ull << 62, 1ull << 63};

   return kPositionMask[pos];
}

//===========================================================================
constexpr void resetLS1B(uint64_t& x)
{
   x &= (x - 1);
}

//===========================================================================
// https://www.chessprogramming.org/Population_Count#Brian_Kernighan.27s_way
constexpr int popCount1(uint64_t x)
{
   int count = 0;
   while (x)
   {
      count++;
      resetLS1B(x);
   }
   return count;
}

//===========================================================================
// https://www.chessprogramming.org/Population_Count#The_PopCount_routine
constexpr int popCount2(uint64_t x)
{
   static constexpr uint64_t k1 = 0x5555555555555555; /*  -1/3   */
   static constexpr uint64_t k2 = 0x3333333333333333; /*  -1/5   */
   static constexpr uint64_t k4 = 0x0f0f0f0f0f0f0f0f; /*  -1/17  */
   static constexpr uint64_t kf = 0x0101010101010101; /*  -1/255 */

   x = x - ((x >> 1) & k1); /* put count of each 2 bits into those 2 bits */
   x = (x & k2) +
       ((x >> 2) & k2);     /* put count of each 4 bits into those 4 bits */
   x = (x + (x >> 4)) & k4; /* put count of each 8 bits into those 8 bits */
   x = (x * kf) >> 56;      /* returns 8 most significant bits of x + (x<<8) +
                               (x<<16) + (x<<24) + ...  */
   return static_cast<int>(x);
}

//===========================================================================
// https://www.chessprogramming.org/BitScan#With_separated_LS1B
constexpr int bitScanForward(uint64_t x)
{
   static constexpr int kIndex64[64] = {
      0,  47, 1,  56, 48, 27, 2,  60, 57, 49, 41, 37, 28, 16, 3,  61,
      54, 58, 35, 52, 50, 42, 21, 44, 38, 32, 29, 23, 17, 11, 4,  62,
      46, 55, 26, 59, 40, 36, 15, 53, 34, 51, 20, 43, 31, 22, 10, 45,
      25, 39, 14, 33, 19, 30, 9,  24, 13, 18, 8,  12, 7,  6,  5,  63};

   static constexpr uint64_t kDebruijn64 = 0x03f79d71b4cb0a89;
   return kIndex64[((x ^ (x - 1)) * kDebruijn64) >> 58];
}

} // namespace internal

class Bitboard
{
public:
   //===========================================================================
   constexpr Bitboard() = default;
   constexpr explicit Bitboard(uint64_t board) : board_(board) {}
   constexpr explicit Bitboard(Position pos) : board_(internal::toBoardInt(pos))
   {
   }

   //===========================================================================
   constexpr uint64_t to_uint64() const { return board_; }

   //===========================================================================
   constexpr auto operator<=>(const Bitboard&) const = default;

   //===========================================================================
   constexpr Bitboard operator|(const Bitboard& rhs) const
   {
      return Bitboard{*this} |= rhs;
   }
   constexpr Bitboard operator&(const Bitboard& rhs) const
   {
      return Bitboard{*this} &= rhs;
   }
   constexpr Bitboard operator^(const Bitboard& rhs) const
   {
      return Bitboard{*this} ^= rhs;
   }
   constexpr Bitboard operator~() const { return Bitboard{*this}.invert(); }

   //===========================================================================
   constexpr Bitboard& operator|=(const Bitboard& rhs)
   {
      board_ |= rhs.board_;
      return *this;
   }
   constexpr Bitboard& operator&=(const Bitboard& rhs)
   {
      board_ &= rhs.board_;
      return *this;
   }
   constexpr Bitboard& operator^=(const Bitboard& rhs)
   {
      board_ ^= rhs.board_;
      return *this;
   }
   constexpr Bitboard& invert()
   {
      board_ = ~board_;
      return *this;
   };

   //===========================================================================
   constexpr bool test(Position pos) const
   {
      return board_ & internal::toBoardInt(pos);
   }

   //===========================================================================
   constexpr int count() const { return internal::popCount2(board_); }

   //==========================================================================
   constexpr bool none() const { return board_ == internal::kEmptyBoard; }
   constexpr bool any() const { return !none(); }
   constexpr bool all() const { return board_ == internal::kFullBoard; }

   //==========================================================================
   constexpr void set(Position pos) { board_ |= internal::toBoardInt(pos); }
   constexpr void reset(Position pos) { board_ &= ~internal::toBoardInt(pos); }
   constexpr void toggle(Position pos) { board_ ^= internal::toBoardInt(pos); }

   //==========================================================================
   constexpr void clear() { board_ = internal::kEmptyBoard; }
   constexpr void fill() { board_ = internal::kFullBoard; }

   //==========================================================================
   class Iterator
   {
   public:
      constexpr Iterator(uint64_t init) : state_(init) {}
      constexpr auto operator<=>(const Iterator&) const = default;

      constexpr Position operator*() const
      {
         assert(state_ != 0);
         return static_cast<Position>(internal::bitScanForward(state_));
      }
      constexpr Iterator& operator++()
      {
         internal::resetLS1B(state_);
         return *this;
      }
      constexpr Iterator operator++(int)
      {
         auto old = state_;
         internal::resetLS1B(state_);
         return Iterator{old};
      }

   private:
      uint64_t state_{0};
   };

   //==========================================================================
   constexpr Iterator begin() const { return Iterator(board_); }
   constexpr Iterator end() const { return Iterator(internal::kEmptyBoard); }

private:
   //===========================================================================
   uint64_t board_{internal::kEmptyBoard};
};

//===========================================================================
std::ostream& operator<<(std::ostream&, const Bitboard&);

} // namespace chess

#endif
