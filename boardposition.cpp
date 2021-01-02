#include "boardposition.h"

namespace BoardPosition{

int rowToRank[8] = {8, 7, 6, 5, 4, 3, 2, 1};
char colToFile[8] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
std::map<int, int> rankToRow = {{1, 7}, {2, 6}, {3, 5}, {4, 4}, {5, 3}, {6, 2}, {7, 1}, {8, 0}};
std::map<char, int> fileToCol = {{'a', 0}, {'b', 1}, {'c', 2}, {'d', 3}, {'e', 4}, {'f', 5}, {'g', 6}, {'h', 7}};

IBP tranlateABPoIBP(ABP pos)
{
    int rank = std::stoi(pos.substr(1,1));
    char file = pos[0];
    return {rankToRow[rank], fileToCol[file]};
}

ABP tranlateIBPoABP(IBP pos)
{
    int rank = rowToRank[pos.row];
    char file = colToFile[pos.col];
    return file + std::to_string(rank);
}

IBP getMoveSrcIBP(ChessMove move)
{
    ABP src = move.substr(0,2);
    return tranlateABPoIBP(src);
}
IBP getMoveDstIBP(ChessMove move)
{
    ABP dst = move.substr(2,2);
    return tranlateABPoIBP(dst);
}
ABP getMoveSrcABP(ChessMove move)
{
    return move.substr(0,2);
}

ABP getMoveDstABP(ChessMove move)
{
    return move.substr(2,2);
}

}
