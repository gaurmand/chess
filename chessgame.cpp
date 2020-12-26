#include <string>
#include "constants.h"
#include "chessgame.h"
#include "chesspiece.h"

ChessGame::ChessGame(BoardState state)
{
    setBoardState(state);
}

bool ChessGame::isValidBoardState(BoardState state)
{
    return true;
}

void ChessGame::initChessPiece(Player player, PieceID id, PieceType type, IBP pos)
{
    pieces[player][id] = new ChessPiece(player, type, pos);
    board.setPiece(pieces[player][id], pos);

}

ChessPiece* ChessGame::getChessPiece(Player player, PieceID id) {
    return pieces[player][id];
}

bool ChessGame::setBoardState(BoardState state)
{
    if(!isValidBoardState(state)) {
        return false;
    }

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
