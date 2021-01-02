#ifndef BOARDPOSITION_H
#define BOARDPOSITION_H

#include "constants.h"
#include <map>

/*
 * BOARD POSITION
 * InternalBoardPosition: (row, column) pair representing postions on the chess board
 * (0,0) (0,1) .. (0,7)
 * (1,0) (1,1) .. (1,7)
 *   :     :        :
 * (7,0) (7,1) .. (7,7)
 *
 * AlgebraicBoardPosition: (file, rank) pair representing postions on the chess board
 * a8 b8 .. h8
 * a7 b7 .. h7
 * :  :     :
 * a1 b1 .. h1
*/

struct InternalBoardPosition
{
    int row;
    int col;
};

typedef InternalBoardPosition IBP;
typedef std::string AlgebraicBoardPosition;
typedef AlgebraicBoardPosition ABP;

namespace  BoardPosition{
    IBP tranlateABPoIBP(ABP pos);
    ABP tranlateIBPoABP(IBP pos);
    IBP getMoveSrcIBP(ChessMove move);
    IBP getMoveDstIBP(ChessMove move);
    ABP getMoveSrcABP(ChessMove move);
    ABP getMoveDstABP(ChessMove move);

    extern int rowToRank[];
    extern char colToFile[];
    extern std::map<int, int> rankToRow;
    extern std::map<char, int> fileToCol;
}

#endif // BOARDPOSITION_H
