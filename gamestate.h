#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "constants.h"
#include "chessboard.h"
#include "boardposition.h"

class ChessPiece;

struct InternalGameState {
    int numHalfMoves;
    int numFullMoves;
    Player active;
    bool canShortCastle[NUM_PLAYERS];
    bool canLongCastle[NUM_PLAYERS];
    bool canEnPassant;
    IBP enPassantPosition;
    ChessBoard board;
};

typedef InternalGameState IGS;
typedef std::string FENGameState; //FEN: https://en.wikipedia.org/wiki/Forsyth%E2%80%93Edwards_Notation
typedef FENGameState FGS;
#endif // GAMESTATE_H
