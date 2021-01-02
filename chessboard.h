#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include "constants.h"
#include "board.h"
#include "boardposition.h"

class ChessPiece;

class ChessBoard : public Board
{
public:
    ChessBoard() : Board() {}
    bool isMoveValid(ChessMove move);
    bool isMoveLegal(ChessMove move, bool checkIfValid = true);
    bool isMoveCapture(ChessMove move);
    virtual bool performMove(ChessMove move);

protected:
    ChessMoves* getLegalMoves(ChessPiece* piece);
    ChessMoves* getValidKingMoves(ChessPiece* piece);
    ChessMoves* getValidQueenMoves(ChessPiece* piece);
    ChessMoves* getValidRookMoves(ChessPiece* piece);
    ChessMoves* getValidBishopMoves(ChessPiece* piece);
    ChessMoves* getValidKnightMoves(ChessPiece* piece);
    ChessMoves* getValidPawnMoves(ChessPiece* piece);

    ChessMove createMove(IBP src, IBP dst, bool isPromotion = false, PieceType promotionType = QUEEN);

    bool canShortCastle[NUM_PLAYERS];
    bool canLongCastle[NUM_PLAYERS];
    bool canEnPassant;
    IBP enPassantPosition;
};

#endif // CHESSBOARD_H
