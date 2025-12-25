#include <chessmodel/move.h>
#include <chessmodel/position.h>
#include <unordered_map>

namespace chess
{

//===========================================================================
std::string toAlgebraic(const Position& pos)
{
   static constexpr std::array kRowToRank = {
      '1', '2', '3', '4', '5', '6', '7', '8'};
   static constexpr std::array kColToFile = {
      'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};

   if (!isValid(pos))
   {
      return "InvalidPosition";
   }

   const int row   = pos / kNumCols;
   const int col   = pos % kNumCols;
   const char rank = kRowToRank.at(row);
   const char file = kColToFile.at(col);

   return std::string{file, rank};
}

//===========================================================================
Position fromAlgebraic(std::string_view str)
{
   static const std::unordered_map<char, int> kRankToRow = {{'1', 0},
                                                            {'2', 1},
                                                            {'3', 2},
                                                            {'4', 3},
                                                            {'5', 4},
                                                            {'6', 5},
                                                            {'7', 6},
                                                            {'8', 7}};
   static const std::unordered_map<char, int> kFileToCol = {{'a', 0},
                                                            {'b', 1},
                                                            {'c', 2},
                                                            {'d', 3},
                                                            {'e', 4},
                                                            {'f', 5},
                                                            {'g', 6},
                                                            {'h', 7}};

   if (str.size() != 2)
   {
      return Position::Invalid;
   }

   const char rank  = str.at(1);
   const char file  = str.at(0);
   const auto rowIt = kRankToRow.find(rank);
   const auto colIt = kFileToCol.find(file);

   if (rowIt == kRankToRow.end() || colIt == kFileToCol.end())
   {
      return Position::Invalid;
   }

   const int row = rowIt->second;
   const int col = colIt->second;

   return static_cast<Position>(row * kNumCols + col);
}

//===========================================================================
std::string Move::toAlgebraic() const
{
   std::string res = "InvalidMove";

   Position src = this->src();
   Position dst = this->dst();

   if (isValid(src) && isValid(dst))
   {
      res = chess::toAlgebraic(src) + chess::toAlgebraic(dst);

      if (promotion())
      {
         switch (promotedPiece())
         {
            case Piece::Pawn: res += "p"; break;
            case Piece::Knight: res += "n"; break;
            case Piece::Bishop: res += "b"; break;
            case Piece::Rook: res += "r"; break;
            case Piece::Queen: res += "q"; break;
            case Piece::King: res += "k"; break;
            default: break;
         }
      }
   }

   return res;
}

//===========================================================================
std::ostream& operator<<(std::ostream& os, const chess::Position& pos)
{
   return os << toAlgebraic(pos);
}

//===========================================================================
std::ostream& operator<<(std::ostream& os, const Move& mv)
{
   return os << mv.toAlgebraic();
}

//===========================================================================
// bool operator<(const Move& lhs, const Move& rhs)
// {
//    return lhs.toAlgebraic() < rhs.toAlgebraic();
// }

} // namespace chess
