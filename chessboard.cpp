#include "chessboard.h"
#include "chesspiece.h"

ChessBoard::ChessBoard()
{
    for(int i=0; i<NUM_ROWS; i++) {
        for(int j=0; j<NUM_COLS; j++) {
            board[i][j] = nullptr;
        }
    }
}

ChessPiece* ChessBoard::movePiece(IBP src, IBP dst)
{
    ChessPiece* dstPiece = getPiece(dst);
    ChessPiece* srcPiece = getPiece(src);

    if(srcPiece == nullptr)
        return nullptr;

    setPiece(srcPiece, dst);
    return dstPiece;
}

ChessPiece* ChessBoard::getPiece(IBP src)
{
    return board[src.row][src.col];
}

void ChessBoard::setPiece(ChessPiece* piece, IBP dst)
{
    if(piece) {
        piece->setIBPos(dst);
    }
    board[dst.row][dst.col] = piece;
}
