#include "board.h"
#include "piece.h"

#include <algorithm>
#include <sstream>
#include <cassert>

namespace Chess
{

void Board::setFENState(const FENState& state)
{
    // clear board and pieces
    std::for_each(board_.begin(), board_.end(), [](auto& row){ row.fill(nullptr); });
    std::for_each(pieces_.begin(), pieces_.end(), [](auto& pieces){ pieces.fill(Piece()); });

    // parse fen string
    std::istringstream fen(state);
    int i=0, j=0;
    std::array<int, NUM_PLAYERS> numPieces = {0, 0};

    for (char curr = fen.get(); curr != EOF; curr = fen.get())
    {
        if (std::isalpha(curr)) // chess piece
        {
            const Player owner = islower(curr) ? Player::Black : Player::White;
            const int id = numPieces[owner]++;
            const BP pos(i, j);
            switch(curr) {
                case 'K':
                case 'k':
                    pieces_[owner][id] = Piece(PieceType::King, owner, pos, id);
                    kingPosition_[owner] = pos;
                    break;
                case 'Q':
                case 'q':
                    pieces_[owner][id] = Piece{PieceType::Queen, owner, pos, id};
                    break;
                case 'R':
                case 'r':
                    pieces_[owner][id] = Piece{PieceType::Rook, owner, pos, id};
                    break;
                case 'B':
                case 'b':
                    pieces_[owner][id] = Piece{PieceType::Bishop, owner, pos, id};
                    break;
                case 'N':
                case 'n':
                    pieces_[owner][id] = Piece{PieceType::Knight, owner, pos, id};
                    break;
                case 'P':
                case 'p':
                    pieces_[owner][id] = Piece{PieceType::Pawn, owner, pos, id};
                    break;
                default:
                    throw 1;
            }
            board_[i][j] = &pieces_[owner][id];
            j++; // next col

            if (numPieces[owner] > NUM_CHESS_PIECES)
            {
                throw 1;
            }
        }
        else if(isdigit(curr)) // num of blank squares
        {
            const int numBlanks = toDigit(curr);
            j += numBlanks;
        }
        else if (curr == '/') // row delimiter
        {
            i++;    // next row
            j = 0;  // first col
        }
        else
        {
            break;
        }
    }

    if (i > NUM_ROWS || j > NUM_COLS)
    {
        throw 1;
    }
}

FENState Board::toFENState() const
{
    std::ostringstream fen;
    for (int i = 0; i < NUM_ROWS; i++)
    {
        int numEmptyPositions = 0;
        for (int j = 0; j < NUM_COLS; j++)
        {
            if (board_[i][j] != nullptr)
            {
                if (numEmptyPositions > 0)
                {
                    fen << numEmptyPositions;
                    numEmptyPositions = 0;
                }
                Piece* piece = board_[i][j];
                fen << *piece;
            }
            else
            {
                numEmptyPositions++;
            }
        }

        if (numEmptyPositions > 0)
        {
            fen << numEmptyPositions;
        }

        if (i < NUM_ROWS - 1)
        {
            fen << "/"; // only output if not last row
        }
    }

    return fen.str();
}

Board& Board::operator=(const Board& board)
{
    pieces_ = board.pieces_;
    kingPosition_ = board.kingPosition_;

    // make empty board
    std::for_each(board_.begin(), board_.end(), [](auto& row){ row.fill(nullptr); });

    // update piece addresses in board
    for(auto& pieces: pieces_)
    {
        for(auto& piece: pieces)
        {
            if (!piece.isCaptured())
            {
                setPiece(piece.pos(), &piece);
            }
        }
    }
    return *this;
}

Piece Board::piece(const BP& pos) const
{
    const Piece* piece = pieceAt(pos);
    if (piece != nullptr)
    {
        return *piece;
    }
    return Piece();
}

std::vector<Piece> Board::pieces(Player player, PieceType type) const
{
    std::vector<Piece> res;
    res.reserve(NUM_CHESS_PIECES);
    const auto& playerPieces = pieces_[player];
    std::copy_if(playerPieces.begin(),
                 playerPieces.end(),
                 std::back_inserter(res),
                 [=](const Piece& piece) {return !piece.isCaptured() && piece.type() == type; });
    return res;
}

std::vector<Piece> Board::pieces(Player player) const
{
    std::vector<Piece> res;
    res.reserve(NUM_CHESS_PIECES);
    const auto& playerPieces = pieces_[player];
    std::copy_if(playerPieces.begin(),
                playerPieces.end(),
                std::back_inserter(res),
                [=](const Piece& piece) {return !piece.isCaptured(); });
    return res;
}

void Board::performMove(const BP& src, const BP& dst,  bool checkLegal)
{
    Piece* dstPiece = pieceAt(dst);
    Piece* srcPiece = pieceAt(src);
    assert(srcPiece != nullptr);

    if (dstPiece != nullptr)
    {
        dstPiece->capture();
    }

    setPiece(dst, srcPiece);
    setPiece(src, nullptr);

    // update piece position
    srcPiece->setPos(dst);

    // update king position
    if (srcPiece->type() == PieceType::King)
    {
        kingPosition_[srcPiece->owner()] = dst;
    }
}

std::ostream& operator<<(std::ostream& os, const Board& rhs)
{
    // TODO: print ranks/files
    for(const auto& row: rhs.board_)
    {
        for(const Piece* piece: row)
        {
            if (piece != nullptr)
            {
                os << *piece << " " ;
            }
            else
            {
                os << ". " ;
            }
        }
        os << '\n';
    }
    return os;
}

};  // namespace Chess
