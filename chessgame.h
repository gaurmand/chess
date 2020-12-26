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

    bool isValidBoardState(BoardState state);
    bool setBoardState(BoardState state);
    bool setPiecesState(BoardState stat);

    bool generateValidMoves();
    ChessMoves* getValidMoves(ChessPiece* piece);
    ChessMoves* getMoves(ChessPiece* piece);
    bool isValidMove(ChessMove move);

    bool performMove(ChessMove move);
    bool performMoves(ChessMoves moves);

    bool getActivePlayer();
    bool switchActivePlayer();
    bool setActivePlayer();

    ChessPiece* getChessPiece(Player player, PieceID id);

private:
    void initChessPiece(Player player, PieceID id, PieceType type, IBP pos);

private:
    bool isCastleAvailable[NUM_PLAYERS];
    IBP enPassantPosition;
    Player activePlayer;
    ChessPiece* pieces[NUM_PLAYERS][NUM_CHESS_PIECES];
    ChessBoard board;
};

#endif // CHESSGAME_H
