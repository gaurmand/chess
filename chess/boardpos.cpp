#include "boardpos.h"
#include "common.h"

#include <map>
#include <array>
#include <sstream>

namespace Chess {

BP::BoardPosition(const ANBP& pos): BoardPosition()
{
    static const std::map<int, int> rankToRow = {{1, 7}, {2, 6}, {3, 5}, {4, 4}, {5, 3}, {6, 2}, {7, 1}, {8, 0}};
    static const std::map<char, int> fileToCol = {{'a', 0}, {'b', 1}, {'c', 2}, {'d', 3}, {'e', 4}, {'f', 5}, {'g', 6}, {'h', 7}};

    std::istringstream ss(pos);
    char file;
    int rank;
    if (!(ss >> file) || !(ss >> rank))
    {
        return;
    }

    try
    {
        row_ = rankToRow.at(rank);
        col_ = fileToCol.at(file);
    }
    catch (std::out_of_range)
    {
        return;
    }
}

ANBP BP::toANBP() const
{
    static const std::array<char, 8> rowToRank = {'8', '7', '6', '5', '4', '3', '2', '1'};
    static const std::array<char, 8> colToFile = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};

    try
    {
        const char rank = rowToRank.at(row_);
        const char file = colToFile.at(col_);
        return ANBP{file, rank};
    }
    catch (std::out_of_range)
    {
        return ANBP();
    }

}

bool BP::isValid() const
{
    return row_ >= MIN_ROW && row_ <= MAX_ROW && col_ >= MIN_COL && col_ <= MAX_COL;
}

std::ostream& operator<<(std::ostream& os, const BP& rhs)
{
    return os << rhs.toANBP();
}

bool operator==(const BP& lhs, const BP& rhs)
{
    return lhs.row_ == rhs.row_ && lhs.col_ == rhs.col_;
}

bool operator!=(const BP& lhs, const BP& rhs)
{
    return !(lhs == rhs);
}

};  // namespace Chess
