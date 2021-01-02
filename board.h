#ifndef BOARD_H
#define BOARD_H

#include "constants.h"
#include "boardposition.h"

class ChessPiece;

class Board
{
public:
    Board();
    ChessPiece* getPiece(IBP pos);
    void printBoard();

protected:
    void setPiece(ChessPiece* piece, IBP pos);
    ChessPiece* movePiece(IBP src, IBP dst);
    void clearBoard();

private:
    ChessPiece* board[NUM_ROWS][NUM_COLS];
};

#endif // BOARD_H
