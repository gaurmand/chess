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

typedef std::string GameState; //FEN string (https://en.wikipedia.org/wiki/Forsyth%E2%80%93Edwards_Notation)

enum PieceType{KING, QUEEN, ROOK, BISHOP, KNIGHT, PAWN};
enum PieceID{K, Q, RA, RH, BC, BF, NB, NG, PA, PB, PC, PD, PE, PF, PG, PH};
enum Player{WHITE, BLACK};

/*
 * CHESS MOVES
 * ChessMove: string containing src and dst positions of move (modified long algebraic notation)
 * e.g. e2e4, e7e5, e1g1 (white short castling), e7e8q (for promotion)
 * https://en.wikipedia.org/wiki/Algebraic_notation_(chess)
*/

typedef std::string ChessMove;
typedef std::vector<ChessMove> ChessMoves;

enum class ChessMoveType {NONE, NORMAL, CAPTURE, SHORT_CASTLE, LONG_CASTLE, PROMOTION, EN_PASSANT};

enum class BoardGraphicalState{NONE, SOURCE, NORMAL_MOVE, CAPTURE, CASTLE, PROMOTION, CHECK};
typedef BoardGraphicalState BGS;

#endif // CONSTANTS_H
