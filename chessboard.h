#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include "constants.h"

class ChessPiece;

class ChessBoard
{
public:
    ChessBoard();
    void clearBoard();

    ChessPiece* movePiece(IBP src, IBP dst);
    ChessPiece* movePiece(ABP src, ABP dst);
    ChessPiece* getPiece(IBP src);
    ChessPiece* getPiece(ABP src);
    void setPiece(ChessPiece* piece, IBP dst);
    void setPiece(ChessPiece* piece, ABP dst);

    ChessMoves* getValidMoves(ChessPiece* piece);
    ChessMoves* getLegalMoves(ChessPiece* piece);
    bool isValidMove(ChessMove move);

public:
    static IBP tranlateABPoIBP(ABP pos);
    static ABP tranlateIBPoABP(IBP pos);

protected:
    ChessMoves* getPawnMoves(ChessPiece* piece);
    ChessMove createMove(IBP src, IBP dst, bool isPromotion = false, PieceType promotiontType = QUEEN);

private:
    ChessPiece* board[NUM_ROWS][NUM_COLS];
};

#endif // CHESSBOARD_H
