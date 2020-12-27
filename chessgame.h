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
    ~ChessGame();

    bool isValidBoardState(BoardState state);
    bool setBoardState(BoardState state);
    bool setPiecesState(BoardState stat);

    ChessPiece* getChessPiece(Player player, PieceID id);

    void generateMoves();
    void printGeneratedMoves();
    std::string movesToString(ChessMoves* moves);

    bool performMove(ChessMove move);
    bool performMoves(ChessMoves moves);

    Player getActivePlayer();
    void switchActivePlayer();
    void setActivePlayer(Player player);

    bool isCheckmate();
    bool isStalemate();
    bool isValidMoveAvailable();

protected:
    void initChessPiece(Player player, PieceID id, PieceType type, IBP pos);

private:
    bool canKingsideCastle[NUM_PLAYERS] = {true, true};
    bool canQueensideCastle[NUM_PLAYERS] = {true, true};
    bool isInCheck[NUM_PLAYERS] = {false, false};
    int numAvailableMoves = 0;
    int numFullmoves = 0;
    bool isEnPassantPossible = false;
    IBP enPassantPosition = {0,0};
    Player activePlayer = WHITE;
    ChessPiece* pieces[NUM_PLAYERS][NUM_CHESS_PIECES];
    ChessMoves* moves[NUM_PLAYERS][NUM_CHESS_PIECES];
    ChessBoard board;
};

#endif // CHESSGAME_H
