#ifndef BOARD_H
#define BOARD_H

#include "common.h"
#include "boardpos.h"
#include "piece.h"

#include <array>
#include <vector>
#include <iostream>
#include <functional>

namespace Chess
{

typedef std::reference_wrapper<const Piece> PieceRef;

class Board
{
public:
    Board() {}
    Board(const FENState& state): Board() { setFENState(state); }
    void setFENState(const FENState& state);
    FENState toFENState() const { return FENState();} // TODO

    Board& operator=(const Board& board);
    Board(const Board& board) { *this = board; }

    Piece piece(const BP& pos) const;
    PieceRef pieceRef(const BP& pos);
    const Piece* piecePtr(int player, int id) const { return &pieces_[player][id]; }
    bool hasPiece(const BP& pos) const { return pieceAt(pos) != nullptr; }
    std::vector<Piece> pieces(Player player) const;
    std::vector<Piece> pieces(Player player, PieceType type) const;
    std::vector<PieceRef> pieceRefs(Player player);
    BP kingPosition(Player owner) const { return kingPosition_[owner]; }

    void performMove(const BP& src, const BP& dst, bool checkLegal = false);

    friend std::ostream& operator<<(std::ostream&, const Board&);

protected:
    const Piece* pieceAt(const BP& pos) const { return board_[pos.row()][pos.col()];}
    Piece* pieceAt(const BP& pos) { return board_[pos.row()][pos.col()];}
    void setPiece(const BP& pos, Piece* piece) {board_[pos.row()][pos.col()] = piece;}

    std::array<std::array<Piece, NUM_CHESS_PIECES>, NUM_PLAYERS> pieces_;
    std::array<std::array<Piece*, NUM_COLS>, NUM_ROWS> board_;
    std::array<BP, NUM_PLAYERS> kingPosition_;
};

}; // namespace Chess

#endif // BOARD_H
