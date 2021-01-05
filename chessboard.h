#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include "constants.h"
#include "boardposition.h"

class ChessPiece;

class ChessBoard
{
public:
    ChessBoard();
    ChessPiece* getPiece(IBP pos);
    void printBoard();
    bool isMoveCapture(ChessMove move);
    virtual bool performMove(ChessMove move);
    virtual std::string toFENString();

protected:
    void setPiece(ChessPiece* piece, IBP pos);
    ChessPiece* movePiece(IBP src, IBP dst);
    void clearBoard();

    ChessMoves* getValidMoves(ChessPiece* piece, ChessMoveTypeOpt opts = {true, true, true, true, {0,0}});
    ChessMoves* getValidKingMoves(ChessPiece* piece, ChessMoveTypeOpt opts);
    ChessMoves* getValidQueenMoves(ChessPiece* piece);
    ChessMoves* getValidRookMoves(ChessPiece* piece);
    ChessMoves* getValidBishopMoves(ChessPiece* piece);
    ChessMoves* getValidKnightMoves(ChessPiece* piece);
    ChessMoves* getValidPawnMoves(ChessPiece* piece, ChessMoveTypeOpt opts);

    bool pushMove(ChessMoves* moves, ChessPiece* srcPiece, IBP dst);
    bool pushCapture(ChessMoves* moves, ChessPiece* srcPiece, IBP dst);
    bool pushNormalMove(ChessMoves* moves, ChessPiece* srcPiece, IBP dst);
    void pushStraightMoves(ChessMoves* moves, ChessPiece* srcPiece);
    void pushDiagonalMoves(ChessMoves* moves, ChessPiece* srcPiece);
    bool pushMoveHelper(ChessMoves* moves, ChessPiece* srcPiece, IBP dst);

    ChessMove createMove(IBP src, IBP dst, bool isPromotion = false, PieceType promotionType = QUEEN);

    ChessPiece* board[NUM_ROWS][NUM_COLS];
};

#endif // CHESSBOARD_H
