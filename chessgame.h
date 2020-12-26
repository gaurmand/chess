#ifndef CHESSGAME_H
#define CHESSGAME_H

#include "constants.h"
#include "chesspiece.h"
#include "chessboard.h"

class ChessGame
{
public:
    ChessGame(BoardState state);
    ChessGame() : ChessGame(INIT_BOARD_STATE) {};

    bool setBoardState(BoardState state);
    bool isValidBoardState(BoardState state);

    bool generateValidMoves();
    ChessMoves* getValidMoves(ChessPiece* piece);
    ChessMoves* getMoves(ChessPiece* piece);
    bool isValidMove(ChessMove move);

    bool performMove(ChessMove move);
    bool performMoves(ChessMoves moves);

    bool getActivePlayer();
    bool switchActivePlayer();
    bool setActivePlayer();

private:
    bool isCastleAvailable[NUM_PLAYERS];
    IBP enPassantPosition;
    Player activePlayer;
    ChessPiece pieces[NUM_PLAYERS][NUM_CHESS_PIECES];
    ChessBoard board;
};

#endif // CHESSGAME_H
