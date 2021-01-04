#ifndef CHESSGAME_H
#define CHESSGAME_H

#include <string>

#include "constants.h"
#include "chesspiece.h"
#include "chessboard.h"

class ChessGame : public ChessBoard
{
public:
    ChessGame();
    ~ChessGame();
    bool setGameState(GameState state);
    void setInitialGameState();
    static bool isValidGameState(GameState state);

    ChessPiece* getChessPiece(Player player, PieceID id);
    ChessMoves* getChessMoves(Player player, PieceID id);

    bool performMove(ChessMove move) override;

    Player getActivePlayer();

    bool isValidMoveAvailable();
    bool isCheck();
    bool isCheckmate();
    bool isStalemate();

protected:
    void initChessPiece(PieceID id, Player player, PieceType type, IBP pos);
    void clearMoves();

    void generateAvailableMoves(Player player, bool checkCastles = false);
    void printAvailableMoves(Player player);
    static  std::string movesToString(ChessMoves* moves);

    void switchActivePlayer();
    void setActivePlayer(Player player);

    bool isPlayerInCheck(Player player);

private:
    int numHalfMoves;
    int numFullMoves;
    Player active;

    ChessPiece* pieces[NUM_PLAYERS][NUM_CHESS_PIECES];
    ChessMoves* moves[NUM_PLAYERS][NUM_CHESS_PIECES];

    int numAvailableMoves;
    bool _isCheck;
    bool _isCheckmate;
    bool _isStalemate;
};

#endif // CHESSGAME_H
