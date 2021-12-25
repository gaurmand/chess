#include "test.h"
#include "../chess/game.h"

#include <vector>

namespace
{

using namespace Chess;

// Basic performMove test
void gameTest1()
{
    Game g("r1Q2rk1/1p1b1N2/2pp1n1p/Q5p1/6P1/B1b5/P1K1PP1P/RN3BR1 b - - 11 120");

    // performMove1
    verifyPiece(g.piece(BP("h6")), Player::Black, PieceType::Pawn);
    VERIFY(!g.piece(BP("h5")).isValid());
    g.performMove(BP("h6"), BP("h5"));
    verifyPiece(g.piece(BP("h5")), Player::Black, PieceType::Pawn);
    VERIFY(!g.piece(BP("h6")).isValid());

    // performMove2
    g.setFENState("r1Q2rk1/1p1b1N2/2pp1n1p/Q5p1/6P1/B1b5/P1K1PP1P/RN3BR1 b - - 11 120");
    verifyPiece(g.piece(BP("h6")), Player::Black, PieceType::Pawn);
    VERIFY(!g.piece(BP("h5")).isValid());
    g.performMove(Move(BP("h6"), BP("h5")));
    verifyPiece(g.piece(BP("h5")), Player::Black, PieceType::Pawn);
    VERIFY(!g.piece(BP("h6")).isValid());

    // performMove1 with no move type specified
    g.setFENState("r1Q2rk1/1p1b1N2/2pp1n1p/Q5p1/6P1/B1b5/P1K1PP1P/RN3BR1 b - - 11 120");
    verifyPiece(g.piece(BP("h6")), Player::Black, PieceType::Pawn);
    VERIFY(!g.piece(BP("h5")).isValid());
    g.performMove(Move(BP("h6"), BP("h5")));
    verifyPiece(g.piece(BP("h5")), Player::Black, PieceType::Pawn);
    VERIFY(!g.piece(BP("h6")).isValid());
}

// Normal moves
void gameTest2()
{
    Game g("r1Q2rk1/1p1b1N2/2pp1n1p/Q5p1/6P1/B1b5/P1K1PP1P/RN3BR1 b - - 11 120");
    verifyPiece(g.piece(BP("d7")), Player::Black, PieceType::Bishop);
    VERIFY(!g.piece(BP("e8")).isValid());
    g.performMove(BP("d7"), BP("e8"));
    verifyPiece(g.piece(BP("e8")), Player::Black, PieceType::Bishop);
    VERIFY(!g.piece(BP("d7")).isValid());

    g.setFENState("r1Q2rk1/1p1b1N2/2pp1n1p/Q5p1/6P1/B1b5/P1K1PP1P/RN3BR1 b - - 11 120");
    verifyPiece(g.piece(BP("f6")), Player::Black, PieceType::Knight);
    VERIFY(!g.piece(BP("e4")).isValid());
    g.performMove(BP("f6"), BP("e4"));
    verifyPiece(g.piece(BP("e4")), Player::Black, PieceType::Knight);
    VERIFY(!g.piece(BP("f6")).isValid());
}

// Captures
void gameTest3()
{
    Game g("r1Q2rk1/1p1b1N2/2pp1n1p/Q5p1/6P1/B1b5/P1K1PP1P/RN3BR1 b - - 11 120");
    verifyPiece(g.piece(BP("a8")), Player::Black, PieceType::Rook);
    verifyPiece(g.piece(BP("a5")), Player::White, PieceType::Queen);
    g.performMove(BP("a8"), BP("a5"));
    verifyPiece(g.piece(BP("a5")), Player::Black, PieceType::Rook);
    VERIFY(!g.piece(BP("a8")).isValid());

    g.setFENState("r1Q2rk1/1p1b1N2/2pp1n1p/Q5p1/6P1/B1b5/P1K1PP1P/RN3BR1 b - - 11 120");
    verifyPiece(g.piece(BP("d7")), Player::Black, PieceType::Bishop);
    verifyPiece(g.piece(BP("g4")), Player::White, PieceType::Pawn);
    g.performMove(BP("d7"), BP("g4"));
    verifyPiece(g.piece(BP("g4")), Player::Black, PieceType::Bishop);
    VERIFY(!g.piece(BP("d7")).isValid());
}

// Trying to perform illegal moves
void gameTest4()
{
    Game g("r1Q2rk1/1p1b1N2/2pp1n1p/Q5p1/6P1/B1b5/P1K1PP1P/RN3BR1 b - - 11 120");

    // illegal moves
    VERIFY_THROWS(g.performMove(BP("f6"), BP("f7")));
    VERIFY_THROWS(g.performMove(Move(BP("c3"), BP("f6"))));

    // legal move, wrong move type
    VERIFY_THROWS(g.performMove(BP("f8"), BP("c8")));
}

// En-passant
void gameTest5()
{
    Game g("r3kb1r/1pp1q1P1/3p1n1p/pP2Q3/2Pn1p2/N3PB1p/PB3P2/R3K2R w Qkq a6 0 24");

    verifyPiece(g.piece(BP("b5")), Player::White, PieceType::Pawn);
    verifyPiece(g.piece(BP("a5")), Player::Black, PieceType::Pawn);
    VERIFY(g.canEnPassant());
    VERIFY(g.enPassantPosition() == BP("a6"));
    g.performMove(BP("b5"), BP("a6"));
    verifyPiece(g.piece(BP("a6")), Player::White, PieceType::Pawn);
    VERIFY(!g.piece(BP("a5")).isValid());
    VERIFY(!g.piece(BP("b5")).isValid());
}

// Promotion (auto queen)
void gameTest6()
{
    Game g("r3kb1r/1pp1q1P1/3p1n1p/pP2Q3/2Pn1p2/N3PB1p/PB3P2/R3K2R w Qkq a6 0 24");

    verifyPiece(g.piece(BP("g7")), Player::White, PieceType::Pawn);
    verifyPiece(g.piece(BP("h8")), Player::Black, PieceType::Rook);
    g.performMove(BP("g7"), BP("h8"));
    VERIFY(!g.piece(BP("g7")).isValid());
    verifyPiece(g.piece(BP("h8")), Player::White, PieceType::Queen);
}

// Short castle
void gameTest7()
{
    Game g("rnb1kbnr/pppq3p/6p1/3p1p2/4p1Q1/BPNBPN2/P1PP1PPP/1R2K2R w Kkq - 0 9");

    verifyPiece(g.piece(BP("e1")), Player::White, PieceType::King);
    verifyPiece(g.piece(BP("h1")), Player::White, PieceType::Rook);
    VERIFY(g.canShortCastle(Player::White));
    g.performMove(BP("e1"), BP("g1"));
    verifyPiece(g.piece(BP("g1")), Player::White, PieceType::King);
    verifyPiece(g.piece(BP("f1")), Player::White, PieceType::Rook);
    VERIFY(!g.piece(BP("h1")).isValid());
}

// Long castle
void gameTest8()
{
    Game g("rnb1kbnr/pppq3p/6p1/3p1p2/4p1Q1/BPNBPN2/P1PP1PPP/R3K1R1 w Qkq - 0 9");
    verifyPiece(g.piece(BP("e1")), Player::White, PieceType::King);
    verifyPiece(g.piece(BP("a1")), Player::White, PieceType::Rook);
    VERIFY(g.canLongCastle(Player::White));
    g.performMove(BP("e1"), BP("c1"));
    verifyPiece(g.piece(BP("c1")), Player::White, PieceType::King);
    verifyPiece(g.piece(BP("d1")), Player::White, PieceType::Rook);
    VERIFY(!g.piece(BP("a1")).isValid());
}

}; // unnamed namespace

void Chess::Tester::gamePerformMoveTests()
{
    RUN(gameTest1);
    RUN(gameTest2);
    RUN(gameTest3);
    RUN(gameTest4);
    RUN(gameTest5);
    RUN(gameTest6);
    RUN(gameTest7);
    RUN(gameTest8);
}
