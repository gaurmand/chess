#ifndef CONSTANTS_H
#define CONSTANTS_H

#define NUM_ROWS 8
#define NUM_COLS 8

#define MAX_ROW NUM_ROWS-1
#define MIN_ROW 0
#define MAX_COL NUM_COLS-1
#define MIN_COL 0

#define NUM_CHESS_PIECES 16
#define MIN_PIECE_ID 0
#define MAX_PIECE_ID 16

#define NUM_CHESS_PIECE_TYPES 6
#define NUM_PLAYERS 2

#define toDigit(x) x - '0'

#define INIT_BOARD_STATE "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
#define INIT_BOARD "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR"

#include <string>

namespace Chess
{

enum Player{White, Black};
enum class PieceType{King, Queen, Rook, Bishop, Knight, Pawn};
enum class ResultType{Incomplete, Checkmate, Stalemate, Draw};
enum class PlayerType{Human, AI};

typedef std::string ForsythEdwardsNotationState;
typedef ForsythEdwardsNotationState FENState;
};

#endif // CONSTANTS_H
