#include "test.h"
#include "../chess/game.h"

namespace
{

using namespace Chess;

// Update player
void gameTest1()
{
    Game g;
    VERIFY(g.activePlayer() == Player::White);
    g.performMove(Move("e2 e4"));
    VERIFY(g.activePlayer() == Player::Black);
    g.performMove(Move("e7 e5"));
    VERIFY(g.activePlayer() == Player::White);

    g.setFENState("r1Q2rk1/1p1b1N2/2pp1n1p/Q5p1/6P1/B1b5/P1K1PP1P/RN3BR1 b - - 11 120");
    VERIFY(g.activePlayer() == Player::Black);
    g.performMove(Move("f6 g4"));
    VERIFY(g.activePlayer() == Player::White);
    g.performMove(Move("a5 d8"));
    VERIFY(g.activePlayer() == Player::Black);
}

// Update full move clock
void gameTest2()
{
    Game g;
    VERIFY(g.numFullMoves() == 1);
    g.performMove(Move("e2 e4"));
    VERIFY(g.numFullMoves() == 1);
    g.performMove(Move("e7 e5"));
    VERIFY(g.numFullMoves() == 2);
    g.performMove(Move("g1 h3"));
    VERIFY(g.numFullMoves() == 2);
    g.performMove(Move("d7 d6"));
    VERIFY(g.numFullMoves() == 3);
}

// Update half move clock
void gameTest3()
{
    Game g;
    VERIFY(g.numHalfMoves() == 0);
    g.performMove(Move("e2 e4"));
    VERIFY(g.numHalfMoves() == 0);
    g.performMove(Move("d7 d5"));
    VERIFY(g.numHalfMoves() == 0);
    g.performMove(Move("g1 f3"));
    VERIFY(g.numHalfMoves() == 1);
    g.performMove(Move("b8 c6"));
    VERIFY(g.numHalfMoves() == 2);
    g.performMove(Move("e4 d5"));
    VERIFY(g.numHalfMoves() == 0);
    g.performMove(Move("g8 f6"));
    VERIFY(g.numHalfMoves() == 1);
    g.performMove(Move("a2 a4"));
    VERIFY(g.numHalfMoves() == 0);
}

// Checkmate
void gameTest4()
{
    Game g;
    VERIFY(g.result() == ResultType::Incomplete);

    g.setFENState("rnb1k1nr/pppp1ppp/5q2/2b1p3/N3P3/1P6/P1PP1PPP/R1BQKBNR b KQkq - 0 4");
    VERIFY(g.result() == ResultType::Incomplete);
    g.performMove(Move("f6 f2"));
    VERIFY(g.activePlayer() == Player::White);
    VERIFY(g.isInCheck());
    VERIFY(g.result() == ResultType::Checkmate);


    g.setFENState("Q7/1p3N1k/2pp1n1p/8/2B3R1/B1b1P2b/P1K2P1P/RN6 w - - 1 126");
    VERIFY(g.result() == ResultType::Incomplete);
    g.performMove(Move("a8 h8"));
    VERIFY(g.activePlayer() == Player::Black);
    VERIFY(g.isInCheck());
    VERIFY(g.result() == ResultType::Checkmate);
}

// Stalemate
void gameTest5()
{
    Game g("1R6/5N1k/7p/7P/2Qp1P2/B7/P1K5/8 w - - 0 139");
    VERIFY(g.result() == ResultType::Incomplete);
    g.performMove(Move("c4 d4"));
    VERIFY(g.activePlayer() == Player::Black);
    VERIFY(!g.isInCheck());
    VERIFY(g.result() == ResultType::Stalemate);

    g.setFENState("8/2p1k1bp/2P5/p7/Pp2b1n1/8/K7/3r4 b - - 13 37");
    VERIFY(g.result() == ResultType::Incomplete);
    g.performMove(Move("d1 b1"));
    VERIFY(g.activePlayer() == Player::White);
    VERIFY(!g.isInCheck());
    VERIFY(g.result() == ResultType::Stalemate);
}

// Trying to perform move after game over
void gameTest6()
{
    Game g("8/2p1k1bp/2P5/p7/Pp2b1n1/8/K7/1r6 w - - 14 38");
    VERIFY(g.activePlayer() == Player::White);
    VERIFY(!g.isInCheck());
    VERIFY(g.result() == ResultType::Stalemate);

    VERIFY_THROWS(g.performMove(Move("a2 a3")));
}

}; // unnamed namespace

void Chess::Tester::gameResultTests()
{
    RUN(gameTest1);
    RUN(gameTest2);
    RUN(gameTest3);
    RUN(gameTest4);
    RUN(gameTest5);
    RUN(gameTest6);
}
