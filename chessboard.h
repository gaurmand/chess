#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include "constants.h"

class ChessPiece;

class ChessBoard
{
public:
    ChessBoard();
    ChessPiece* performMove(IBP src, IBP dst);
    ChessPiece* performMove(ABP src, ABP dst);
    ChessPiece* getPiece(IBP src);
    ChessPiece* getPiece(ABP src);

public:
    static IBP tranlateABPoIBP(ABP pos);
    static ABP tranlateIBPoABP(IBP pos);

private:
    ChessPiece* board[NUM_ROWS][NUM_COLS];
};

#endif // CHESSBOARD_H
