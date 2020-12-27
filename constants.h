#ifndef CONSTANTS_H
#define CONSTANTS_H

#define NUM_ROWS 8
#define NUM_COLS 8

#define MAX_ROW_INDEX NUM_ROWS-1
#define MIN_ROW_INDEX 0
#define MAX_COL_INDEX NUM_COLS-1
#define MIN_COL_INDEX 0

#define SQUARE_WIDTH 90
#define BOARD_WIDTH SQUARE_WIDTH*NUM_ROWS
#define BOARD_HEIGHT SQUARE_WIDTH*NUM_COLS

#define NUM_CHESS_PIECES 16
#define NUM_CHESS_PIECE_TYPES 6
#define NUM_PLAYERS 2

#define INIT_BOARD_STATE "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

#include <string>
#include <vector>

typedef std::string BoardState; //FEN string (https://en.wikipedia.org/wiki/Forsyth%E2%80%93Edwards_Notation)
typedef std::string ChessMove;  //modified LAN string (https://en.wikipedia.org/wiki/Algebraic_notation_(chess))
typedef std::vector<ChessMove> ChessMoves;

enum PieceType{KING, QUEEN, ROOK, BISHOP, KNIGHT, PAWN};
enum PieceID{K, Q, RA, RH, BC, BF, NB, NG, PA, PB, PC, PD, PE, PF, PG, PH};
enum Player{WHITE, BLACK};

struct InternalBoardPosition
{
    int row;
    int col;
};

typedef std::string AlgebraicBoardPosition;
typedef AlgebraicBoardPosition ABP;
typedef InternalBoardPosition IBP;

#endif // CONSTANTS_H
