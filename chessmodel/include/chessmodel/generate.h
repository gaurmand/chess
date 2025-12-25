#ifndef CHESSMODEL_GENERATE_H
#define CHESSMODEL_GENERATE_H

#include <chessmodel/bitboard.h>
#include <chessmodel/board.h>
#include <chessmodel/move.h>
#include <chessmodel/state.h>
#include <iterator>

//===========================================================================
namespace chess
{

//===========================================================================
template <typename OutputIt>
   requires std::output_iterator<OutputIt, Move>
class MoveWriter
{
public:
   //===========================================================================
   MoveWriter(OutputIt sink) : sink_{sink} {}

   //===========================================================================
   int numOutput() const { return numOutput_; }

   //===========================================================================
   MoveWriter& operator<<(const Move& mv)
   {
      *sink_ = mv;
      sink_++;
      numOutput_++;
      return *this;
   }

private:
   //===========================================================================
   OutputIt sink_;
   int numOutput_{0};
};

//===========================================================================
class MoveGenerator
{
public:
   //===========================================================================
   MoveGenerator(const Board& board, const State& state);

   //===========================================================================
   // Generates moves for all pieces. Returns number of moves generated.
   int exec(MoveArray& moves) const;

   //===========================================================================
   // Generates moves for the given piece type. Returns number of moves
   // generated.
   int exec(Piece pc, MoveArray& moves) const;

   //===========================================================================
   bool sortMovesOnOutput() const { return sortMovesOnOutput_; }
   void setSortMovesOnOutput(bool sort) { sortMovesOnOutput_ = sort; }

   //===========================================================================
   bool checkMoveLegality() const { return checkMoveLegality_; }
   void setCheckMoveLegality(bool check) { checkMoveLegality_ = check; }

private:
   //===========================================================================
   void exec(MoveWriter<MoveArray::iterator>& writer) const;

   //===========================================================================
   void generatePawnMoves(MoveWriter<MoveArray::iterator>& writer) const;
   void generateKingMoves(MoveWriter<MoveArray::iterator>& writer) const;
   void generateKnightMoves(MoveWriter<MoveArray::iterator>& writer) const;
   void generateBishopMoves(MoveWriter<MoveArray::iterator>& writer) const;
   void generateRookMoves(MoveWriter<MoveArray::iterator>& writer) const;
   void generateQueenMoves(MoveWriter<MoveArray::iterator>& writer) const;

   //===========================================================================
   void writeMove(MoveWriter<MoveArray::iterator>& writer,
                  const Move& mv) const;
   void writePromotions(MoveWriter<MoveArray::iterator>& writer,
                        Move& mv) const;
   //===========================================================================
   bool isLegalMove(const Move& mv) const;

   //===========================================================================
   bool inCheck(Position pos, const Board& board) const;

   //===========================================================================
   Bitboard pieces(Piece p, Colour c) const;
   Position getKingPosition(Colour c) const;

private:
   //===========================================================================
   const Board& board_;
   const State& state_;

   //===========================================================================
   bool sortMovesOnOutput_{false};
   bool checkMoveLegality_{true};
};

} // namespace chess

#endif
