#include "test.h"
#include "../chess/position.h"

#include <sstream>

namespace
{

using namespace Chess;

void verifyInitalState(const Position& b)
{
    // verify initial state
    VERIFY(b.numHalfMoves() == 0);
    VERIFY(b.numFullMoves() == 1);
    VERIFY(b.activePlayer() == Player::White);
    VERIFY(b.canShortCastle(Player::Black) == true);
    VERIFY(b.canShortCastle(Player::White) == true);
    VERIFY(b.canLongCastle(Player::Black) == true);
    VERIFY(b.canLongCastle(Player::White) == true);
    VERIFY(b.canEnPassant() == false);
    VERIFY(b.enPassantPosition() == BP(-1, -1));

    // verify pieces
    auto bpieces = b.pieces(Player::Black);
    VERIFY(bpieces.size() == 16);

    verifyPiece(bpieces[0], Player::Black, PieceType::Rook);
    verifyPiece(bpieces[1], Player::Black, PieceType::Knight);
    verifyPiece(bpieces[2], Player::Black, PieceType::Bishop);
    verifyPiece(bpieces[3], Player::Black, PieceType::Queen);
    verifyPiece(bpieces[4], Player::Black, PieceType::King);
    verifyPiece(bpieces[5], Player::Black, PieceType::Bishop);
    verifyPiece(bpieces[6], Player::Black, PieceType::Knight);
    verifyPiece(bpieces[7], Player::Black, PieceType::Rook);
    verifyPiece(bpieces[8], Player::Black, PieceType::Pawn);
    verifyPiece(bpieces[9], Player::Black, PieceType::Pawn);
    verifyPiece(bpieces[10], Player::Black, PieceType::Pawn);
    verifyPiece(bpieces[11], Player::Black, PieceType::Pawn);
    verifyPiece(bpieces[12], Player::Black, PieceType::Pawn);
    verifyPiece(bpieces[13], Player::Black, PieceType::Pawn);
    verifyPiece(bpieces[14], Player::Black, PieceType::Pawn);
    verifyPiece(bpieces[15], Player::Black, PieceType::Pawn);

    auto wpieces = b.pieces(Player::White);
    VERIFY(wpieces.size() == 16);

    verifyPiece(wpieces[0], Player::White, PieceType::Pawn);
    verifyPiece(wpieces[1], Player::White, PieceType::Pawn);
    verifyPiece(wpieces[2], Player::White, PieceType::Pawn);
    verifyPiece(wpieces[3], Player::White, PieceType::Pawn);
    verifyPiece(wpieces[4], Player::White, PieceType::Pawn);
    verifyPiece(wpieces[5], Player::White, PieceType::Pawn);
    verifyPiece(wpieces[6], Player::White, PieceType::Pawn);
    verifyPiece(wpieces[7], Player::White, PieceType::Pawn);
    verifyPiece(wpieces[8], Player::White, PieceType::Rook);
    verifyPiece(wpieces[9], Player::White, PieceType::Knight);
    verifyPiece(wpieces[10], Player::White, PieceType::Bishop);
    verifyPiece(wpieces[11], Player::White, PieceType::Queen);
    verifyPiece(wpieces[12], Player::White, PieceType::King);
    verifyPiece(wpieces[13], Player::White, PieceType::Bishop);
    verifyPiece(wpieces[14], Player::White, PieceType::Knight);
    verifyPiece(wpieces[15], Player::White,  PieceType::Rook);

    // Verify board addresses

    VERIFY(b.piece(BP(0,0)) == bpieces[0]);
    VERIFY(b.piece(BP(0,1)) == bpieces[1]);
    VERIFY(b.piece(BP(0,2)) == bpieces[2]);
    VERIFY(b.piece(BP(0,3)) == bpieces[3]);
    VERIFY(b.piece(BP(0,4)) == bpieces[4]);
    VERIFY(b.piece(BP(0,5)) == bpieces[5]);
    VERIFY(b.piece(BP(0,6)) == bpieces[6]);
    VERIFY(b.piece(BP(0,7)) == bpieces[7]);

    VERIFY(b.piece(BP(1,0)) == bpieces[8]);
    VERIFY(b.piece(BP(1,1)) == bpieces[9]);
    VERIFY(b.piece(BP(1,2)) == bpieces[10]);
    VERIFY(b.piece(BP(1,3)) == bpieces[11]);
    VERIFY(b.piece(BP(1,4)) == bpieces[12]);
    VERIFY(b.piece(BP(1,5)) == bpieces[13]);
    VERIFY(b.piece(BP(1,6)) == bpieces[14]);
    VERIFY(b.piece(BP(1,7)) == bpieces[15]);

    VERIFY(b.piece(BP(2,0)).isValid() == false);
    VERIFY(b.piece(BP(2,1)).isValid() == false);
    VERIFY(b.piece(BP(2,2)).isValid() == false);
    VERIFY(b.piece(BP(2,3)).isValid() == false);
    VERIFY(b.piece(BP(2,4)).isValid() == false);
    VERIFY(b.piece(BP(2,5)).isValid() == false);
    VERIFY(b.piece(BP(2,6)).isValid() == false);
    VERIFY(b.piece(BP(2,7)).isValid() == false);

    VERIFY(b.piece(BP(3,0)).isValid() == false);
    VERIFY(b.piece(BP(3,1)).isValid() == false);
    VERIFY(b.piece(BP(3,2)).isValid() == false);
    VERIFY(b.piece(BP(3,3)).isValid() == false);
    VERIFY(b.piece(BP(3,4)).isValid() == false);
    VERIFY(b.piece(BP(3,5)).isValid() == false);
    VERIFY(b.piece(BP(3,6)).isValid() == false);
    VERIFY(b.piece(BP(3,7)).isValid() == false);

    VERIFY(b.piece(BP(4,0)).isValid() == false);
    VERIFY(b.piece(BP(4,1)).isValid() == false);
    VERIFY(b.piece(BP(4,2)).isValid() == false);
    VERIFY(b.piece(BP(4,3)).isValid() == false);
    VERIFY(b.piece(BP(4,4)).isValid() == false);
    VERIFY(b.piece(BP(4,5)).isValid() == false);
    VERIFY(b.piece(BP(4,6)).isValid() == false);
    VERIFY(b.piece(BP(4,7)).isValid() == false);

    VERIFY(b.piece(BP(5,0)).isValid() == false);
    VERIFY(b.piece(BP(5,1)).isValid() == false);
    VERIFY(b.piece(BP(5,2)).isValid() == false);
    VERIFY(b.piece(BP(5,3)).isValid() == false);
    VERIFY(b.piece(BP(5,4)).isValid() == false);
    VERIFY(b.piece(BP(5,5)).isValid() == false);
    VERIFY(b.piece(BP(5,6)).isValid() == false);
    VERIFY(b.piece(BP(5,7)).isValid() == false);

    VERIFY(b.piece(BP(6,0)) == wpieces[0]);
    VERIFY(b.piece(BP(6,1)) == wpieces[1]);
    VERIFY(b.piece(BP(6,2)) == wpieces[2]);
    VERIFY(b.piece(BP(6,3)) == wpieces[3]);
    VERIFY(b.piece(BP(6,4)) == wpieces[4]);
    VERIFY(b.piece(BP(6,5)) == wpieces[5]);
    VERIFY(b.piece(BP(6,6)) == wpieces[6]);
    VERIFY(b.piece(BP(6,7)) == wpieces[7]);

    VERIFY(b.piece(BP(7,0)) == wpieces[8]);
    VERIFY(b.piece(BP(7,1)) == wpieces[9]);
    VERIFY(b.piece(BP(7,2)) == wpieces[10]);
    VERIFY(b.piece(BP(7,3)) == wpieces[11]);
    VERIFY(b.piece(BP(7,4)) == wpieces[12]);
    VERIFY(b.piece(BP(7,5)) == wpieces[13]);
    VERIFY(b.piece(BP(7,6)) == wpieces[14]);
    VERIFY(b.piece(BP(7,7)) == wpieces[15]);

    // verify king positions
    VERIFY(b.kingPosition(Player::White) == BP("e1"));
    VERIFY(b.kingPosition(Player::Black) == BP("e8"));
}

void positionTest1()
{
    Position p;
    verifyInitalState(p);
}

void positionTest2()
{
    VERIFY(Position::isValidFENState("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq e4 0 1"));
    VERIFY(Position::isValidFENState("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w Kq - 0 1"));
    VERIFY(Position::isValidFENState("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR b - - 50 120"));

    VERIFY(!Position::isValidFENState("rnbqk1bnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"));
    VERIFY(!Position::isValidFENState("pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"));
    VERIFY(!Position::isValidFENState("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR q KQkq - 0 1"));
    VERIFY(!Position::isValidFENState("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w Kekq - 0 1"));
    VERIFY(!Position::isValidFENState("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - e 1"));
    VERIFY(!Position::isValidFENState("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq e4 0 f"));
    VERIFY(!Position::isValidFENState("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNRw KQkq e1"));
}

void positionTest3()
{
    Position p;
    Position saved = p.save();
    p.setFENState("r1Q2rk1/1p1b1N2/2pp1n1p/Q5p1/6P1/B1b5/P1K1PP1P/RN3BR1 b K--q e4 11 120");
    p.restore(saved);

    verifyInitalState(p);
}


}; // unnamed namespace

void Chess::Tester::positionTests()
{
    RUN(positionTest1);
    RUN(positionTest2);
    RUN(positionTest3);
}
