#include <chessmodel/bitboard.h>
#include <chessmodel/piece.h>

namespace chess
{

//==========================================================================
// constexpr Bitboard::Bitboard(uint64_t board) : board_(board) {}

// //==========================================================================
// constexpr Bitboard Bitboard::operator|(const Bitboard& rhs) const
// {
//    return Bitboard{board_ | rhs.board_};
// }

// //==========================================================================
// constexpr Bitboard Bitboard::operator&(const Bitboard& rhs) const
// {
//    return Bitboard{board_ & rhs.board_};
// }

// //==========================================================================
// constexpr Bitboard Bitboard::operator^(const Bitboard& rhs) const
// {
//    return Bitboard{board_ ^ rhs.board_};
// }

// //==========================================================================
// constexpr Bitboard Bitboard::operator~() const
// {
//    return Bitboard{~board_};
// }

//==========================================================================
std::ostream& operator<<(std::ostream& os, const Bitboard& bitboard)
{
   std::uint64_t bb = bitboard.to_uint64();
   for (int row = 0; row < 8; ++row, bb <<= 8)
   {
      int rankNum = 8 - row;
      os << rankNum << " ";

      std::uint64_t rank = (bb & 0xFF00000000000000ull) >> 56;
      for (int col = 0; col < 8; ++col, rank >>= 1)
      {
         if (rank & 0x1ull)
         {
            os << 'x';
         }
         else
         {
            os << '.';
         }
         os << ' ';
      }

      os << '\n';
   }

   os << "  a b c d e f g h\n";

   return os;
}

} // namespace chess
