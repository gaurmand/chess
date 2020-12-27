#include <string>
#include <iostream>
#include "constants.h"
#include "chessgame.h"
#include "chesspiece.h"

ChessGame::ChessGame(GameState state)
{
    //init moves array
    for (int i=0; i<NUM_PLAYERS; i++){
        for(int j=0; j<NUM_CHESS_PIECES; j++){
            moves[i][j] = nullptr;
        }
    }

    setGameState(state);
}

ChessGame::ChessGame()
{
    //init moves array
    for (int i=0; i<NUM_PLAYERS; i++){
        for(int j=0; j<NUM_CHESS_PIECES; j++){
            moves[i][j] = nullptr;
        }
    }

    setInitialGameState();
}

bool ChessGame::isValidGameState(GameState state)
{
    return true;
}

ChessGame::~ChessGame()
{
    for (int i=0; i<NUM_PLAYERS; i++){
        for(int j=0; j<NUM_CHESS_PIECES; j++){
            if(pieces[i][j])
                delete pieces[i][j];
            if(moves[i][j])
                delete moves[i][j];
        }
    }
}


void ChessGame::initChessPiece(Player player, PieceID id, PieceType type, IBP pos)
{
    pieces[player][id] = new ChessPiece(player, type, pos);
    board.setPiece(pieces[player][id], pos);
}

void ChessGame::clearMoves()
{
    for (int i=0; i<NUM_PLAYERS; i++){
        for(int j=0; j<NUM_CHESS_PIECES; j++){
            if(moves[i][j] != nullptr) {
                delete moves[i][j];
                moves[i][j] = nullptr;
            }
        }
    }
    numAvailableMoves = 0;
}

ChessPiece* ChessGame::getChessPiece(Player player, PieceID id) {
    return pieces[player][id];
}

bool ChessGame::setGameState(GameState state)
{
    return true;
}

bool ChessGame::setInitialGameState()
{
    //set game state variables to initial state
    for(int i=0; i<NUM_PLAYERS; i++) {
        canKingsideCastle[i] = true;
        canQueensideCastle[i] = true;
        isInCheck[i] = false;
    }
    isEnPassantPossible = false;
    enPassantPosition = {0,0};
    activePlayer = WHITE;

    //clear moves array and free memory
    clearMoves();

    //set white pieces to initial positions
    initChessPiece(WHITE, K, KING,      {7, 4});
    initChessPiece(WHITE, Q, QUEEN,     {7, 3});
    initChessPiece(WHITE, RA, ROOK,     {7, 0});
    initChessPiece(WHITE, RH, ROOK,     {7, 7});
    initChessPiece(WHITE, BC, BISHOP,   {7, 2});
    initChessPiece(WHITE, BF, BISHOP,   {7, 5});
    initChessPiece(WHITE, NB, KNIGHT,   {7, 1});
    initChessPiece(WHITE, NG, KNIGHT,   {7, 6});
    initChessPiece(WHITE, PA, PAWN,     {6, 0});
    initChessPiece(WHITE, PB, PAWN,     {6, 1});
    initChessPiece(WHITE, PC, PAWN,     {6, 2});
    initChessPiece(WHITE, PD, PAWN,     {6, 3});
    initChessPiece(WHITE, PE, PAWN,     {6, 4});
    initChessPiece(WHITE, PF, PAWN,     {6, 5});
    initChessPiece(WHITE, PG, PAWN,     {6, 6});
    initChessPiece(WHITE, PH, PAWN,     {6, 7});

    //set black pieces to initial positions
    initChessPiece(BLACK, K, KING,      {0, 4});
    initChessPiece(BLACK, Q, QUEEN,     {0, 3});
    initChessPiece(BLACK, RA, ROOK,     {0, 0});
    initChessPiece(BLACK, RH, ROOK,     {0, 7});
    initChessPiece(BLACK, BC, BISHOP,   {0, 2});
    initChessPiece(BLACK, BF, BISHOP,   {0, 5});
    initChessPiece(BLACK, NB, KNIGHT,   {0, 1});
    initChessPiece(BLACK, NG, KNIGHT,   {0, 6});
    initChessPiece(BLACK, PA, PAWN,     {1, 0});
    initChessPiece(BLACK, PB, PAWN,     {1, 1});
    initChessPiece(BLACK, PC, PAWN,     {1, 2});
    initChessPiece(BLACK, PD, PAWN,     {1, 3});
    initChessPiece(BLACK, PE, PAWN,     {1, 4});
    initChessPiece(BLACK, PF, PAWN,     {1, 5});
    initChessPiece(BLACK, PG, PAWN,     {1, 6});
    initChessPiece(BLACK, PH, PAWN,     {1, 7});

    return true;
}

void ChessGame::generateMoves() {
    numAvailableMoves = 0;

    for (int i=0; i<NUM_PLAYERS; i++){
        for(int j=0; j<NUM_CHESS_PIECES; j++){
            ChessPiece* piece = pieces[i][j];

            if(moves[i][j] != nullptr) {
                delete moves[i][j];
            }

            if(piece->isCaptured()) {
                moves[i][j] = nullptr;
            } else {
                moves[i][j] = board.getValidMoves(piece);
            }

            if(moves[i][j] != nullptr) {
                numAvailableMoves += moves[i][j]->size();
            }
        }
    }

    printGeneratedMoves();
}

void ChessGame::printGeneratedMoves()
{
    for (int i=0; i<NUM_PLAYERS; i++){
        for(int j=0; j<NUM_CHESS_PIECES; j++){
            ChessPiece* piece = pieces[i][j];

            std::cout << piece->toString() << ": ";

            if(!piece->isCaptured() && moves[i][j] != nullptr) {
                std::cout << "(" << moves[i][j]->size() << ") " << movesToString(moves[i][j]);
            }

            std::cout << std::endl;
        }
    }
}

std::string ChessGame::movesToString(ChessMoves* moves)
{
    std::string res = "";
    for(ChessMoves::iterator it = moves->begin(); it != moves->end(); ++it) {
        ChessMove move = *it;
        res += move;
    }

    return res;
}

Player ChessGame::getActivePlayer()
{
    return activePlayer;
}

void ChessGame::switchActivePlayer()
{
    if(activePlayer == WHITE)
        activePlayer = BLACK;
    else
        activePlayer = WHITE;
}

void ChessGame::setActivePlayer(Player player)
{
    activePlayer = player;
}

bool ChessGame::isValidMoveAvailable()
{
    return numAvailableMoves != 0;
}


bool ChessGame::isCheckmate()
{
    return isInCheck[activePlayer] && !isValidMoveAvailable();
}

bool ChessGame::isStalemate()
{
    return !isInCheck[activePlayer] && !isValidMoveAvailable();

}
