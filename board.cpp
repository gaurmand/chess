#include "board.h"
#include "chesspiece.h"

Board::Board()
{
    clearBoard();
}

ChessPiece* Board::getPiece(IBP pos)
{
    return board[pos.row][pos.col];
}

void Board::setPiece(ChessPiece* piece, IBP pos)
{
    board[pos.row][pos.col] = piece;
    if(piece) {
        piece->setIBPos(pos);
    }
}

ChessPiece* Board::movePiece(IBP src, IBP dst)
{
    ChessPiece* dstPiece = board[dst.row][dst.col];
    ChessPiece* srcPiece = board[src.row][src.col];

    if(srcPiece == nullptr) {
        //do nothing if no piece at src position
        return nullptr;
    }

    setPiece(srcPiece, dst);
    setPiece(nullptr, src);
    return dstPiece;
}

void Board::clearBoard()
{
    for(int i=0; i<NUM_ROWS; i++) {
        for(int j=0; j<NUM_COLS; j++) {
            board[i][j] = nullptr;
        }
    }
}
