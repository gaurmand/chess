
#include <algorithm>
#include <cctype>
#include <charconv>
#include <chessmodel/game.h>
#include <chessmodel/generate.h>
#include <iostream>
#include <ostream>
#include <ranges>
#include <sstream>
#include <vector>

namespace chess
{

namespace
{

//===========================================================================
static constexpr std::array kLongRookStartPos{Position::a1, Position::a8};
static constexpr std::array kLongRookEndPos{Position::d1, Position::d8};
static constexpr std::array kShortRookStartPos{Position::h1, Position::h8};
static constexpr std::array kShortRookEndPos{Position::f1, Position::f8};

//===========================================================================
int toDigit(char x)
{
   return x - '0';
}

//===========================================================================
std::optional<Board> parseBoard(std::string_view boardStr)
{
   Board b;
   int row{0}, col{0};
   for (char c : boardStr)
   {
      if (std::isalpha(c))
      {
         if (row < 0 || row > 7 || col < 0 || col > 7)
         {
            return {};
         }

         Position pos = positionFromRowColIndex(row, col);

         switch (c)
         {
            case 'P': b.set(pos, Piece::Pawn, Colour::White); break;
            case 'p': b.set(pos, Piece::Pawn, Colour::Black); break;
            case 'N': b.set(pos, Piece::Knight, Colour::White); break;
            case 'n': b.set(pos, Piece::Knight, Colour::Black); break;
            case 'B': b.set(pos, Piece::Bishop, Colour::White); break;
            case 'b': b.set(pos, Piece::Bishop, Colour::Black); break;
            case 'R': b.set(pos, Piece::Rook, Colour::White); break;
            case 'r': b.set(pos, Piece::Rook, Colour::Black); break;
            case 'Q': b.set(pos, Piece::Queen, Colour::White); break;
            case 'q': b.set(pos, Piece::Queen, Colour::Black); break;
            case 'K': b.set(pos, Piece::King, Colour::White); break;
            case 'k': b.set(pos, Piece::King, Colour::Black); break;
            default: return {};
         }
         col++; // next col
      }
      else if (std::isdigit(c))
      {
         const int numBlankSquares = toDigit(c);
         if (numBlankSquares < 0 || numBlankSquares > 8)
         {
            return {};
         }
         col += numBlankSquares; // skip cols
      }
      else if (c == '/')
      {
         row++;   // next row
         col = 0; // first col
      }
      else
      {
         return {};
      }
   }

   return b;
}

//===========================================================================
std::optional<Colour> parseActive(std::string_view colourStr)
{
   if (colourStr.size() != 1)
   {
      return {};
   }

   if (colourStr[0] == 'w')
   {
      return Colour::White;
   }
   else if (colourStr[0] == 'b')
   {
      return Colour::Black;
   }
   return {};
}

//===========================================================================
struct Castling
{
   std::array<bool, 2> longCastle{false, false};
   std::array<bool, 2> shortCastle{false, false};
};

std::optional<Castling> parseCastling(std::string_view castlingStr)
{
   Castling res;

   for (char c : castlingStr)
   {
      switch (c)
      {
         case 'K': res.shortCastle[Colour::White] = true; break;
         case 'k': res.shortCastle[Colour::Black] = true; break;
         case 'Q': res.longCastle[Colour::White] = true; break;
         case 'q': res.longCastle[Colour::Black] = true; break;
         case '-': return res;
         default: return {};
      }
   }

   return res;
}

//===========================================================================
std::optional<Position> parseEnPassant(std::string_view epStr)
{
   if (epStr.size() == 1 && epStr[0] == '-')
   {
      return Position::Invalid;
   }

   std::string_view view{epStr.begin(), epStr.end()};
   auto pos = fromAlgebraic(view);
   if (pos != Position::Invalid)
   {
      return pos;
   }
   return {};
}

//===========================================================================
std::optional<std::uint16_t> parseClock(std::string_view clockStr)
{
   if (!std::ranges::all_of(clockStr, [](char c) { return std::isdigit(c); }))
   {
      return {};
   }

   std::uint16_t clock;
   auto [_, ec] = std::from_chars(clockStr.begin(), clockStr.end(), clock);

   if (ec == std::errc())
   {
      return clock;
   }

   return {};
}

//===========================================================================
void outputBoard(const Board& b, std::ostream& stream)
{
   for (int row = 0; row < 8; row++)
   {
      int numBlankSquares{0};
      for (int col = 0; col < 8; col++)
      {
         Position pos = positionFromRowColIndex(row, col);
         if (b.occupied(pos))
         {
            if (numBlankSquares > 0)
            {
               stream << numBlankSquares;
               numBlankSquares = 0;
            }

            Piece p  = b.piece(pos).value();
            Colour c = b.colour(pos).value();

            if (c == Colour::White)
            {
               switch (p)
               {
                  case Piece::Pawn: stream << "P"; break;
                  case Piece::Knight: stream << "N"; break;
                  case Piece::Bishop: stream << "B"; break;
                  case Piece::Rook: stream << "R"; break;
                  case Piece::Queen: stream << "Q"; break;
                  case Piece::King: stream << "K"; break;
                  default: break;
               }
            }
            else
            {
               switch (p)
               {
                  case Piece::Pawn: stream << "p"; break;
                  case Piece::Knight: stream << "n"; break;
                  case Piece::Bishop: stream << "b"; break;
                  case Piece::Rook: stream << "r"; break;
                  case Piece::Queen: stream << "q"; break;
                  case Piece::King: stream << "k"; break;
                  default: break;
               }
            }
         }
         else
         {
            numBlankSquares++;
         }
      }

      if (numBlankSquares > 0)
      {
         stream << numBlankSquares;
         numBlankSquares = 0;
      }

      if (row < 7)
      {
         stream << "/"; // only output if not last row
      }
   }
}

//===========================================================================
void outputState(const State& s, std::ostream& stream)
{
   if (s.active == Colour::White)
   {
      stream << "w";
   }
   else
   {
      stream << "b";
   }

   stream << " ";

   bool castle{false};
   if (s.shortCastle[Colour::White])
   {
      castle = true;
      stream << "K";
   }
   if (s.longCastle[Colour::White])
   {
      castle = true;
      stream << "Q";
   }
   if (s.shortCastle[Colour::Black])
   {
      castle = true;
      stream << "k";
   }
   if (s.longCastle[Colour::Black])
   {
      castle = true;
      stream << "q";
   }
   if (!castle)
   {
      stream << "-";
   }

   stream << " ";

   if (s.enPassant != Position::Invalid)
   {
      stream << toAlgebraic(s.enPassant);
   }
   else
   {
      stream << "-";
   }

   stream << " " << s.halfMoveClock << " " << s.fullMoveClock;
}

} // namespace

//===========================================================================
std::optional<Game> Game::fromFEN(std::string_view fen)
{
   std::vector<std::string_view> parts;
   for (auto str : std::views::split(fen, ' '))
   {
      parts.emplace_back(str);
   }

   if (parts.size() != 6)
   {
      return {};
   }

   auto board = parseBoard(parts[0]);
   if (!board.has_value())
   {
      return {};
   }

   auto active = parseActive(parts[1]);
   if (!active.has_value())
   {
      return {};
   }

   auto castling = parseCastling(parts[2]);
   if (!castling.has_value())
   {
      return {};
   }

   auto enPassant = parseEnPassant(parts[3]);
   if (!enPassant.has_value())
   {
      return {};
   }

   auto halfMoveClock = parseClock(parts[4]);
   if (!halfMoveClock.has_value())
   {
      return {};
   }

   auto fullMoveClock = parseClock(parts[5]);
   if (!fullMoveClock.has_value())
   {
      return {};
   }

   State state;
   state.active        = active.value();
   state.longCastle    = castling->longCastle;
   state.shortCastle   = castling->shortCastle;
   state.enPassant     = enPassant.value();
   state.halfMoveClock = halfMoveClock.value();
   state.fullMoveClock = fullMoveClock.value();

   return Game{board.value(), state};
}

//===========================================================================
std::string Game::toFEN() const
{
   std::ostringstream fen;
   outputBoard(board_, fen);
   fen << " ";
   outputState(state_, fen);

   return fen.str();
}

//===========================================================================
int Game::generate(MoveArray& moves) const
{
   return MoveGenerator{board_, state_}.exec(moves);
}

//===========================================================================
void Game::move(const Move& mv)
{
   Colour active   = mv.active();
   Colour inactive = opponent(active);

   Position src = mv.src();
   Position dst = mv.dst();

   Piece piece = mv.piece();

   // 1. Capture/en passant
   const bool capture = mv.capture();
   if (capture)
   {
      if (mv.enPassant())
      {
         Position capturePos = enPassantCapturePosition(state_.enPassant);
         board_.clear(capturePos, Piece::Pawn, inactive);
      }
      else
      {
         board_.clear(dst, mv.capturedPiece(), inactive);
      }
   }

   // 2. Move
   board_.move(src, dst, piece, active);

   // 3. Promote
   if (mv.promotion())
   {
      board_.promote(dst, piece, mv.promotedPiece());
   }

   // 4. Castle (move rook only)
   if (mv.longCastle())
   {
      board_.move(kLongRookStartPos[active],
                  kLongRookEndPos[active],
                  Piece::Rook,
                  active);
   }
   else if (mv.shortCastle())
   {
      board_.move(kShortRookStartPos[active],
                  kShortRookEndPos[active],
                  Piece::Rook,
                  active);
   }

   // 5. State update
   stack_.push(state_);

   // Castling
   static constexpr std::array kKingStartPos{Position::e1, Position::e8};
   static constexpr std::array kLongRookPos{Position::a1, Position::a8};
   static constexpr std::array kShortRookPos{Position::h1, Position::h8};

   if (state_.longCastle[active] &&
       (src == kKingStartPos[active] || src == kLongRookPos[active]))
   {
      state_.longCastle[active] = false;
   }
   if (state_.shortCastle[active] &&
       (src == kKingStartPos[active] || src == kShortRookPos[active]))
   {
      state_.shortCastle[active] = false;
   }

   // En passant
   if (piece == Piece::Pawn && isDoubleAdvance(src, dst))
   {
      state_.enPassant = enPassantPosition(dst);
   }
   else if (state_.enPassant != Position::Invalid)
   {
      state_.enPassant = Position::Invalid;
   }

   // Clocks
   if (state_.active == Colour::Black)
   {
      state_.fullMoveClock += 1;
   }
   if (piece == Piece::Pawn || capture)
   {
      state_.halfMoveClock = 0;
   }
   else
   {
      state_.halfMoveClock += 1;
   }

   state_.active = inactive;
}

//===========================================================================
void Game::unmove(const Move& mv)
{
   Colour active   = mv.active();
   Colour inactive = opponent(active);

   Position src = mv.src();
   Position dst = mv.dst();

   Piece piece = mv.piece();

   // 1. Unpromote
   if (mv.promotion())
   {
      board_.promote(dst, mv.promotedPiece(), piece);
   }

   // 2. Unmove
   board_.move(dst, src, piece, active);

   // 3. Uncapture/en passant
   if (mv.capture())
   {
      if (mv.enPassant())
      {
         auto enPassant = enPassantCapturePosition(dst);
         board_.set(enPassant, Piece::Pawn, inactive);
      }
      else
      {
         board_.set(dst, mv.capturedPiece(), inactive);
      }
   }

   // 4. Uncastle (rook only)
   if (mv.longCastle())
   {
      board_.move(kLongRookEndPos[active],
                  kLongRookStartPos[active],
                  Piece::Rook,
                  active);
   }
   else if (mv.shortCastle())
   {
      board_.move(kShortRookEndPos[active],
                  kShortRookStartPos[active],
                  Piece::Rook,
                  active);
   }

   // 5. Undo state update
   state_ = stack_.pop();
}

} // namespace chess
