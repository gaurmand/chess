#ifndef CHESSGAME_H
#define CHESSGAME_H

#include <string>

#include "constants.h"
#include "gamestate.h"
#include "chesspiece.h"
#include "chessboard.h"

typedef std::map<ABP, ChessMoves*> ABPToMovesMap;

class ChessGame : public ChessBoard
{
public:
    ChessGame();
    ~ChessGame();
    bool setGameState(FGS state);
    void setInitialGameState();
    static bool isValidGameState(FGS state);
    FGS toFENString() override;

    ChessPiece* getChessPiece(Player player, PieceID id);
    ChessMoves* getChessMoves(ABP pos);

    bool performMove(ChessMove move) override;
    bool isMoveValid(ChessMove move);
    bool isMoveLegal(ChessMove move);

    Player getActivePlayer();

    bool isValidMoveAvailable();
    bool isCheck();
    bool isCheckmate();
    bool isStalemate();

    IBP getKingPos(Player player);

protected:
    void initChessPiece(PieceID id, Player player, PieceType type, IBP pos);
    void clearMoves();
    void setBoardState(std::string FENString);

    void computeAvailableMoves();
    void printAvailableMoves();
    static  std::string movesToString(ChessMoves* moves);

    void switchActivePlayer();
    void setActivePlayer(Player player);

    ChessMoves* getLegalMoves(ChessPiece* piece);
    bool isPlayerInCheck(Player player);

private:
    int numHalfMoves;
    int numFullMoves;
    Player active;

    bool canShortCastle[NUM_PLAYERS];
    bool canLongCastle[NUM_PLAYERS];
    bool canEnPassant;
    IBP enPassantPosition;

    ChessPiece* pieces[NUM_PLAYERS][NUM_CHESS_PIECES];
    ABPToMovesMap movesMap;

    int numAvailableMoves;
    bool _isCheck;
    bool _isCheckmate;
    bool _isStalemate;
};

#endif // CHESSGAME_H
