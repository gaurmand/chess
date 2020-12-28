#ifndef CHESSGAME_H
#define CHESSGAME_H

#include "constants.h"
#include "chesspiece.h"
#include "chessboard.h"

class ChessGame
{
public:
    ChessGame(GameState state);
    ChessGame();
    ~ChessGame();

    bool isValidGameState(GameState state);
    bool setGameState(GameState state);
    bool setInitialGameState();

    ChessPiece* getChessPiece(Player player, PieceID id);
    ChessMoves* getChessMoves(Player player, PieceID id);

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
    void initChessPiece( PieceID id, Player player, PieceType type, IBP pos);
    void clearMoves();

private:
    bool canKingsideCastle[NUM_PLAYERS] = {true, true};
    bool canQueensideCastle[NUM_PLAYERS] = {true, true};
    bool isInCheck[NUM_PLAYERS] = {false, false};
    int numFullmoves = 0;
    bool isEnPassantPossible = false;
    IBP enPassantPosition = {0,0};
    Player activePlayer = WHITE;
    ChessPiece* pieces[NUM_PLAYERS][NUM_CHESS_PIECES];
    ChessMoves* moves[NUM_PLAYERS][NUM_CHESS_PIECES];
    int numAvailableMoves = 0;
    ChessBoard board;
};

#endif // CHESSGAME_H
