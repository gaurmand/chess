#include "test.h"
#include "../chess/board.h"

#include <sstream>
#include <array>

namespace
{

using namespace Chess;

void verifyInitalState(const Board& b)
{
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

void boardTest1()
{
    Board b("r1Q2rk1/1p1b1N2/2pp1n1p/Q5p1/6P1/B1b5/P1K1PP1P/RN3BR1");

    // verify pieces
    auto bpieces = b.pieces(Player::Black);
    VERIFY(bpieces.size() == 11);

    verifyPiece(bpieces[0], Player::Black, PieceType::Rook);
    verifyPiece(bpieces[1], Player::Black, PieceType::Rook);
    verifyPiece(bpieces[2], Player::Black, PieceType::King);
    verifyPiece(bpieces[3], Player::Black, PieceType::Pawn);
    verifyPiece(bpieces[4], Player::Black, PieceType::Bishop);
    verifyPiece(bpieces[5], Player::Black, PieceType::Pawn);
    verifyPiece(bpieces[6], Player::Black, PieceType::Pawn);
    verifyPiece(bpieces[7], Player::Black, PieceType::Knight);
    verifyPiece(bpieces[8], Player::Black, PieceType::Pawn);
    verifyPiece(bpieces[9], Player::Black, PieceType::Pawn);
    verifyPiece(bpieces[10], Player::Black, PieceType::Bishop);

    auto wpieces = b.pieces(Player::White);
    VERIFY(wpieces.size() == 14);

    verifyPiece(wpieces[0], Player::White, PieceType::Queen);
    verifyPiece(wpieces[1], Player::White, PieceType::Knight);
    verifyPiece(wpieces[2], Player::White, PieceType::Queen);
    verifyPiece(wpieces[3], Player::White, PieceType::Pawn);
    verifyPiece(wpieces[4], Player::White, PieceType::Bishop);
    verifyPiece(wpieces[5], Player::White, PieceType::Pawn);
    verifyPiece(wpieces[6], Player::White, PieceType::King);
    verifyPiece(wpieces[7], Player::White, PieceType::Pawn);
    verifyPiece(wpieces[8], Player::White, PieceType::Pawn);
    verifyPiece(wpieces[9], Player::White, PieceType::Pawn);
    verifyPiece(wpieces[10], Player::White, PieceType::Rook);
    verifyPiece(wpieces[11], Player::White, PieceType::Knight);
    verifyPiece(wpieces[12], Player::White, PieceType::Bishop);
    verifyPiece(wpieces[13], Player::White,  PieceType::Rook);

    // Verify board addresses
    VERIFY(b.piece(BP(0,0)) == bpieces[0]);
    VERIFY(b.piece(BP(0,1)).isValid() == false);
    VERIFY(b.piece(BP(0,2)) == wpieces[0]);
    VERIFY(b.piece(BP(0,3)).isValid() == false);
    VERIFY(b.piece(BP(0,4)).isValid() == false);
    VERIFY(b.piece(BP(0,5)) == bpieces[1]);
    VERIFY(b.piece(BP(0,6)) == bpieces[2]);
    VERIFY(b.piece(BP(0,7)).isValid() == false);

    VERIFY(b.piece(BP(1,0)).isValid() == false);
    VERIFY(b.piece(BP(1,1)) == bpieces[3]);
    VERIFY(b.piece(BP(1,2)).isValid() == false);
    VERIFY(b.piece(BP(1,3)) == bpieces[4]);
    VERIFY(b.piece(BP(1,4)).isValid() == false);
    VERIFY(b.piece(BP(1,5)) == wpieces[1]);
    VERIFY(b.piece(BP(1,6)).isValid() == false);
    VERIFY(b.piece(BP(1,7)).isValid() == false);

    VERIFY(b.piece(BP(2,0)).isValid() == false);
    VERIFY(b.piece(BP(2,1)).isValid() == false);
    VERIFY(b.piece(BP(2,2)) == bpieces[5]);
    VERIFY(b.piece(BP(2,3)) == bpieces[6]);
    VERIFY(b.piece(BP(2,4)).isValid() == false);
    VERIFY(b.piece(BP(2,5)) == bpieces[7]);
    VERIFY(b.piece(BP(2,6)).isValid() == false);
    VERIFY(b.piece(BP(2,7)) == bpieces[8]);

    VERIFY(b.piece(BP(3,0)) == wpieces[2]);
    VERIFY(b.piece(BP(3,1)).isValid() == false);
    VERIFY(b.piece(BP(3,2)).isValid() == false);
    VERIFY(b.piece(BP(3,3)).isValid() == false);
    VERIFY(b.piece(BP(3,4)).isValid() == false);
    VERIFY(b.piece(BP(3,5)).isValid() == false);
    VERIFY(b.piece(BP(3,6)) == bpieces[9]);
    VERIFY(b.piece(BP(3,7)).isValid() == false);

    VERIFY(b.piece(BP(4,0)).isValid() == false);
    VERIFY(b.piece(BP(4,1)).isValid() == false);
    VERIFY(b.piece(BP(4,2)).isValid() == false);
    VERIFY(b.piece(BP(4,3)).isValid() == false);
    VERIFY(b.piece(BP(4,4)).isValid() == false);
    VERIFY(b.piece(BP(4,5)).isValid() == false);
    VERIFY(b.piece(BP(4,6)) == wpieces[3]);
    VERIFY(b.piece(BP(4,7)).isValid() == false);

    VERIFY(b.piece(BP(5,0)) == wpieces[4]);
    VERIFY(b.piece(BP(5,1)).isValid() == false);
    VERIFY(b.piece(BP(5,2)) == bpieces[10]);
    VERIFY(b.piece(BP(5,3)).isValid() == false);
    VERIFY(b.piece(BP(5,4)).isValid() == false);
    VERIFY(b.piece(BP(5,5)).isValid() == false);
    VERIFY(b.piece(BP(5,6)).isValid() == false);
    VERIFY(b.piece(BP(5,7)).isValid() == false);

    VERIFY(b.piece(BP(6,0)) == wpieces[5]);
    VERIFY(b.piece(BP(6,1)).isValid() == false);
    VERIFY(b.piece(BP(6,2)) == wpieces[6]);
    VERIFY(b.piece(BP(6,3)).isValid() == false);
    VERIFY(b.piece(BP(6,4)) == wpieces[7]);
    VERIFY(b.piece(BP(6,5)) == wpieces[8]);
    VERIFY(b.piece(BP(6,6)).isValid() == false);
    VERIFY(b.piece(BP(6,7)) == wpieces[9]);

    VERIFY(b.piece(BP(7,0)) == wpieces[10]);
    VERIFY(b.piece(BP(7,1)) == wpieces[11]);
    VERIFY(b.piece(BP(7,2)).isValid() == false);
    VERIFY(b.piece(BP(7,3)).isValid() == false);
    VERIFY(b.piece(BP(7,4)).isValid() == false);
    VERIFY(b.piece(BP(7,5)) == wpieces[12]);
    VERIFY(b.piece(BP(7,6)) == wpieces[13]);
    VERIFY(b.piece(BP(7,7)).isValid() == false);

    // verify king positions
    VERIFY(b.kingPosition(Player::White) == BP("c2"));
    VERIFY(b.kingPosition(Player::Black) == BP("g8"));
}

void boardTest2()
{
    Board b("r1Q2rk1/1p1b1N2/2pp1n1p/Q5p1/6P1/B1b5/P1K1PP1P/RN3BR1 ");
    b.setFENState("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");

    verifyInitalState(b);
}

void boardTest3()
{
    Board b("r1Q2rk1/1p1b1N2/2pp1n1p/Q5p1/6P1/B1b5/P1K1PP1P/RN3BR1");
    Board c("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");

    b = c;

    verifyInitalState(b);
}

void boardTest4()
{
    Board b("r1Q2rk1/1p1b1N2/2pp1n1p/Q5p1/6P1/B1b5/P1K1PP1P/RN3BR1");
    const Board& br = b;

    VERIFY(b.hasPiece(BP(0,0)));
    VERIFY(!b.hasPiece(BP(7,7)));

    verifyPiece(b.piece(BP("c3")), Player::Black, PieceType::Bishop);
    verifyPiece(br.piece(BP("f7")), Player::White, PieceType::Knight);
    verifyPiece(br.piece(BP("g5")), Player::Black, PieceType::Pawn);

    VERIFY(!b.piece(BP("e5")).isValid());
    VERIFY(!b.piece(BP("h1")).isValid());
    VERIFY(!b.piece(BP("b8")).isValid());
}

void boardTest5()
{
    Board b("r1Q2rk1/1p1b1N2/2pp1n1p/Q5p1/6P1/B1b5/P1K1PP1P/RN3BR1");

    auto queens = b.pieces(Player::White, PieceType::Queen);
    VERIFY(queens.size() == 2);
    verifyPiece(queens[0], Player::White, PieceType::Queen);
    verifyPiece(queens[1], Player::White, PieceType::Queen);
    VERIFY(queens[0].pos() == BP("c8"));
    VERIFY(queens[1].pos() == BP("a5"));

    auto pawns = b.pieces(Player::Black, PieceType::Pawn);
    VERIFY(pawns.size() == 5);
    verifyPiece(pawns[0], Player::Black, PieceType::Pawn);
    verifyPiece(pawns[1], Player::Black, PieceType::Pawn);
    verifyPiece(pawns[2], Player::Black, PieceType::Pawn);
    verifyPiece(pawns[3], Player::Black, PieceType::Pawn);
    verifyPiece(pawns[4], Player::Black, PieceType::Pawn);
    VERIFY(pawns[0].pos() == BP("b7"));
    VERIFY(pawns[1].pos() == BP("c6"));
    VERIFY(pawns[2].pos() == BP("d6"));
    VERIFY(pawns[3].pos() == BP("h6"));
    VERIFY(pawns[4].pos() == BP("g5"));

    auto bqueens = b.pieces(Player::Black, PieceType::Queen);
    VERIFY(bqueens.size() == 0);
}

void boardTest6()
{
    Board b("6R1/7k/7p/1p2r3/2B1PP2/1K6/r6P/8");

    auto white = b.pieces(Player::White);
    VERIFY(white.size() == 6);
    verifyPiece(white[0], Player::White, PieceType::Rook);
    verifyPiece(white[1], Player::White, PieceType::Bishop);
    verifyPiece(white[2], Player::White, PieceType::Pawn);
    verifyPiece(white[3], Player::White, PieceType::Pawn);
    verifyPiece(white[4], Player::White, PieceType::King);
    verifyPiece(white[5], Player::White, PieceType::Pawn);

    VERIFY(white[0].pos() == BP("g8"));
    VERIFY(white[1].pos() == BP("c4"));
    VERIFY(white[2].pos() == BP("e4"));
    VERIFY(white[3].pos() == BP("f4"));
    VERIFY(white[4].pos() == BP("b3"));
    VERIFY(white[5].pos() == BP("h2"));

    auto black = b.pieces(Player::Black);
    VERIFY(black.size() == 5);
    verifyPiece(black[0], Player::Black, PieceType::King);
    verifyPiece(black[1], Player::Black, PieceType::Pawn);
    verifyPiece(black[2], Player::Black, PieceType::Pawn);
    verifyPiece(black[3], Player::Black, PieceType::Rook);
    verifyPiece(black[4], Player::Black, PieceType::Rook);

    VERIFY(black[0].pos() == BP("h7"));
    VERIFY(black[1].pos() == BP("h6"));
    VERIFY(black[2].pos() == BP("b5"));
    VERIFY(black[3].pos() == BP("e5"));
    VERIFY(black[4].pos() == BP("a2"));
}

void boardTest7()
{
    Board b("r1Q2rk1/1p1b1N2/2pp1n1p/Q5p1/6P1/B1b5/P1K1PP1P/RN3BR1");

    verifyPiece(b.piece(BP("a5")), Player::White, PieceType::Queen);

    b.performMove(BP("a5"), BP("f5"));

    VERIFY(b.hasPiece(BP("f5")));
    VERIFY(!b.hasPiece(BP("a5")));
    verifyPiece(b.piece(BP("f5")), Player::White, PieceType::Queen);
}

void boardTest8()
{
    Board b("r1Q2rk1/1p1b1N2/2pp1n1p/Q5p1/6P1/B1b5/P1K1PP1P/RN3BR1");

    verifyPiece(b.piece(BP("a5")), Player::White, PieceType::Queen);
    verifyPiece(b.piece(BP("c3")), Player::Black, PieceType::Bishop);

    b.performMove(BP("a5"), BP("c3"));

    VERIFY(b.hasPiece(BP("c3")));
    VERIFY(!b.hasPiece(BP("a5")));
    verifyPiece(b.piece(BP("c3")), Player::White, PieceType::Queen);
}

void boardTest9()
{
    Board b("r1Q2rk1/1p1b1N2/2pp1n1p/Q5p1/6P1/B1b5/P1K1PP1P/RN3BR1");

    verifyPiece( b.piece(BP("c2")), Player::White, PieceType::King);
    VERIFY(b.kingPosition(Player::White) == BP("c2"));

    b.performMove(BP("c2"), BP("c3"));

    verifyPiece( b.piece(BP("c3")), Player::White, PieceType::King);
    VERIFY(b.kingPosition(Player::White) == BP("c3"));
}

void boardTest10()
{
    Board b("6R1/7k/7p/1p2r3/2B1PP2/1K6/r6P/8");

    std::ostringstream ss;
    ss << b;
    VERIFY(ss.str() == ". . . . . . R . \n. . . . . . . k \n. . . . . . . p \n. p . . r . . . \n. . B . P P . . \n. K . . . . . . \nr . . . . . . P \n. . . . . . . . \n")
}

}; // unnamed namespace

void Chess::Tester::boardTests()
{
    RUN(boardTest1);
    RUN(boardTest2);
    RUN(boardTest3);
    RUN(boardTest4);
    RUN(boardTest5);
    RUN(boardTest6);
    RUN(boardTest7);
    RUN(boardTest8);
    RUN(boardTest9);
    RUN(boardTest10);
}
