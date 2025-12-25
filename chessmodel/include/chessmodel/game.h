#ifndef CHESSMODEL_GAME_H
#define CHESSMODEL_GAME_H

#include <chessmodel/board.h>
#include <chessmodel/state.h>
#include <optional>
#include <string_view>

//===========================================================================
namespace chess
{

//===========================================================================
template <std::size_t N>
class StateStack
{
public:
   //===========================================================================
   void push(const State& state)
   {
      assert(top_ < N);
      stack_[top_++] = state;
   }
   State pop()
   {
      assert(top_ > 0);
      return stack_[--top_];
   }

private:
   //===========================================================================
   std::array<State, N> stack_;
   std::size_t top_{0};
};

//===========================================================================
class Game
{
public:
   //===========================================================================
   Game() = default;
   Game(const Board& b, const State& s) : board_(b), state_(s) {}

   //===========================================================================
   static std::optional<Game> fromFEN(std::string_view fen);
   std::string toFEN() const;

   //===========================================================================
   const Board& board() const { return board_; }
   const State& state() const { return state_; }

   //===========================================================================
   int generate(MoveArray& moves) const;

   //===========================================================================
   void move(const Move& mv);
   void unmove(const Move& mv);

private:
   //===========================================================================
   // Note: We cannot determine the previous en passant state, castling rights,
   // or halfmove clock from the current board/state. That's why we have to
   // store/restore the state from a stack.
   StateStack<16> stack_;

   //===========================================================================
   Board board_;
   State state_;
};

} // namespace chess

#endif
