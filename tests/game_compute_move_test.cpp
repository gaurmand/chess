#include "test.h"
#include "../chess/game.h"

#include <vector>

namespace
{

using namespace Chess;

// Initial state
void gameTest1()
{
    Game g;

    VERIFY(g.moves(BP("a2")) == std::vector<Move>{Move("a2 a3"), Move("a2 a4")});
    VERIFY(g.moves(BP("b2")) == std::vector<Move>{Move("b2 b3"), Move("b2 b4")});
    VERIFY(g.moves(BP("c2")) == std::vector<Move>{Move("c2 c3"), Move("c2 c4")});
    VERIFY(g.moves(BP("d2")) == std::vector<Move>{Move("d2 d3"), Move("d2 d4")});
    VERIFY(g.moves(BP("e2")) == std::vector<Move>{Move("e2 e3"), Move("e2 e4")});
    VERIFY(g.moves(BP("f2")) == std::vector<Move>{Move("f2 f3"), Move("f2 f4")});
    VERIFY(g.moves(BP("g2")) == std::vector<Move>{Move("g2 g3"), Move("g2 g4")});
    VERIFY(g.moves(BP("h2")) == std::vector<Move>{Move("h2 h3"), Move("h2 h4")});

    VERIFY(g.moves(BP("a1")) == std::vector<Move>{});
    VERIFY(g.moves(BP("b1")) == std::vector<Move>{Move("b1 c3"), Move("b1 a3")});
    VERIFY(g.moves(BP("c1")) == std::vector<Move>{});
    VERIFY(g.moves(BP("d1")) == std::vector<Move>{});
    VERIFY(g.moves(BP("e1")) == std::vector<Move>{});
    VERIFY(g.moves(BP("f1")) == std::vector<Move>{});
    VERIFY(g.moves(BP("g1")) == std::vector<Move>{Move("g1 h3"), Move("g1 f3")});
    VERIFY(g.moves(BP("h1")) == std::vector<Move>{});

    VERIFY(g.moves(BP("a3")) == std::vector<Move>{});
    VERIFY(g.moves(BP("b3")) == std::vector<Move>{});
    VERIFY(g.moves(BP("c3")) == std::vector<Move>{});
    VERIFY(g.moves(BP("d3")) == std::vector<Move>{});
    VERIFY(g.moves(BP("e3")) == std::vector<Move>{});
    VERIFY(g.moves(BP("f3")) == std::vector<Move>{});
    VERIFY(g.moves(BP("g3")) == std::vector<Move>{});
    VERIFY(g.moves(BP("h3")) == std::vector<Move>{});

    VERIFY(g.moves(BP("a4")) == std::vector<Move>{});
    VERIFY(g.moves(BP("b4")) == std::vector<Move>{});
    VERIFY(g.moves(BP("c4")) == std::vector<Move>{});
    VERIFY(g.moves(BP("d4")) == std::vector<Move>{});
    VERIFY(g.moves(BP("e4")) == std::vector<Move>{});
    VERIFY(g.moves(BP("f4")) == std::vector<Move>{});
    VERIFY(g.moves(BP("g4")) == std::vector<Move>{});
    VERIFY(g.moves(BP("h4")) == std::vector<Move>{});

    VERIFY(g.moves(BP("a5")) == std::vector<Move>{});
    VERIFY(g.moves(BP("b5")) == std::vector<Move>{});
    VERIFY(g.moves(BP("c5")) == std::vector<Move>{});
    VERIFY(g.moves(BP("d5")) == std::vector<Move>{});
    VERIFY(g.moves(BP("e5")) == std::vector<Move>{});
    VERIFY(g.moves(BP("f5")) == std::vector<Move>{});
    VERIFY(g.moves(BP("g5")) == std::vector<Move>{});
    VERIFY(g.moves(BP("h5")) == std::vector<Move>{});

    VERIFY(g.moves(BP("a6")) == std::vector<Move>{});
    VERIFY(g.moves(BP("b6")) == std::vector<Move>{});
    VERIFY(g.moves(BP("c6")) == std::vector<Move>{});
    VERIFY(g.moves(BP("d6")) == std::vector<Move>{});
    VERIFY(g.moves(BP("e6")) == std::vector<Move>{});
    VERIFY(g.moves(BP("f6")) == std::vector<Move>{});
    VERIFY(g.moves(BP("g6")) == std::vector<Move>{});
    VERIFY(g.moves(BP("h6")) == std::vector<Move>{});

    VERIFY(g.moves(BP("a7")) == std::vector<Move>{});
    VERIFY(g.moves(BP("b7")) == std::vector<Move>{});
    VERIFY(g.moves(BP("c7")) == std::vector<Move>{});
    VERIFY(g.moves(BP("d7")) == std::vector<Move>{});
    VERIFY(g.moves(BP("e7")) == std::vector<Move>{});
    VERIFY(g.moves(BP("f7")) == std::vector<Move>{});
    VERIFY(g.moves(BP("g7")) == std::vector<Move>{});
    VERIFY(g.moves(BP("h7")) == std::vector<Move>{});

    VERIFY(g.moves(BP("a8")) == std::vector<Move>{});
    VERIFY(g.moves(BP("b8")) == std::vector<Move>{});
    VERIFY(g.moves(BP("c8")) == std::vector<Move>{});
    VERIFY(g.moves(BP("d8")) == std::vector<Move>{});
    VERIFY(g.moves(BP("e8")) == std::vector<Move>{});
    VERIFY(g.moves(BP("f8")) == std::vector<Move>{});
    VERIFY(g.moves(BP("g8")) == std::vector<Move>{});
    VERIFY(g.moves(BP("h8")) == std::vector<Move>{});
}

// Normal moves & captures
void gameTest2()
{
    Game g("r1Q2rk1/1p1b1N2/2pp1n1p/Q5p1/6P1/B1b5/P1K1PP1P/RN3BR1 b - - 11 120");

    VERIFY(g.moves(BP("a1")) == std::vector<Move>{});
    VERIFY(g.moves(BP("b1")) == std::vector<Move>{});
    VERIFY(g.moves(BP("c1")) == std::vector<Move>{});
    VERIFY(g.moves(BP("d1")) == std::vector<Move>{});
    VERIFY(g.moves(BP("e1")) == std::vector<Move>{});
    VERIFY(g.moves(BP("f1")) == std::vector<Move>{});
    VERIFY(g.moves(BP("g1")) == std::vector<Move>{});
    VERIFY(g.moves(BP("h1")) == std::vector<Move>{});

    VERIFY(g.moves(BP("a2")) == std::vector<Move>{});
    VERIFY(g.moves(BP("b2")) == std::vector<Move>{});
    VERIFY(g.moves(BP("c2")) == std::vector<Move>{});
    VERIFY(g.moves(BP("d2")) == std::vector<Move>{});
    VERIFY(g.moves(BP("e2")) == std::vector<Move>{});
    VERIFY(g.moves(BP("f2")) == std::vector<Move>{});
    VERIFY(g.moves(BP("g2")) == std::vector<Move>{});
    VERIFY(g.moves(BP("h2")) == std::vector<Move>{});

    VERIFY(g.moves(BP("a3")) == std::vector<Move>{});
    VERIFY(g.moves(BP("b3")) == std::vector<Move>{});
    VERIFY(g.moves(BP("c3")) == std::vector<Move>{Move("c3 d4"), Move("c3 e5"), Move("c3 d2"), Move("c3 e1"), Move("c3 b2"), Move("c3 a1"), Move("c3 b4"), Move("c3 a5")});
    VERIFY(g.moves(BP("d3")) == std::vector<Move>{});
    VERIFY(g.moves(BP("e3")) == std::vector<Move>{});
    VERIFY(g.moves(BP("f3")) == std::vector<Move>{});
    VERIFY(g.moves(BP("g3")) == std::vector<Move>{});
    VERIFY(g.moves(BP("h3")) == std::vector<Move>{});

    VERIFY(g.moves(BP("a4")) == std::vector<Move>{});
    VERIFY(g.moves(BP("b4")) == std::vector<Move>{});
    VERIFY(g.moves(BP("c4")) == std::vector<Move>{});
    VERIFY(g.moves(BP("d4")) == std::vector<Move>{});
    VERIFY(g.moves(BP("e4")) == std::vector<Move>{});
    VERIFY(g.moves(BP("f4")) == std::vector<Move>{});
    VERIFY(g.moves(BP("g4")) == std::vector<Move>{});
    VERIFY(g.moves(BP("h4")) == std::vector<Move>{});

    VERIFY(g.moves(BP("a5")) == std::vector<Move>{});
    VERIFY(g.moves(BP("b5")) == std::vector<Move>{});
    VERIFY(g.moves(BP("c5")) == std::vector<Move>{});
    VERIFY(g.moves(BP("d5")) == std::vector<Move>{});
    VERIFY(g.moves(BP("e5")) == std::vector<Move>{});
    VERIFY(g.moves(BP("f5")) == std::vector<Move>{});
    VERIFY(g.moves(BP("g5")) == std::vector<Move>{});
    VERIFY(g.moves(BP("h5")) == std::vector<Move>{});

    VERIFY(g.moves(BP("a6")) == std::vector<Move>{});
    VERIFY(g.moves(BP("b6")) == std::vector<Move>{});
    VERIFY(g.moves(BP("c6")) == std::vector<Move>{Move("c6 c5")});
    VERIFY(g.moves(BP("d6")) == std::vector<Move>{Move("d6 d5")});
    VERIFY(g.moves(BP("e6")) == std::vector<Move>{});
    VERIFY(g.moves(BP("f6")) == std::vector<Move>{Move("f6 h7"), Move("f6 h5"), Move("f6 g4"), Move("f6 e4"), Move("f6 d5"), Move("f6 e8")});
    VERIFY(g.moves(BP("g6")) == std::vector<Move>{});
    VERIFY(g.moves(BP("h6")) == std::vector<Move>{Move("h6 h5")});

    VERIFY(g.moves(BP("a7")) == std::vector<Move>{});
    VERIFY(g.moves(BP("b7")) == std::vector<Move>{Move("b7 b6"), Move("b7 b5")});
    VERIFY(g.moves(BP("c7")) == std::vector<Move>{});
    VERIFY(g.moves(BP("d7")) == std::vector<Move>{Move("d7 e8"), Move("d7 e6"), Move("d7 f5"), Move("d7 g4"), Move("d7 c8")});
    VERIFY(g.moves(BP("e7")) == std::vector<Move>{});
    VERIFY(g.moves(BP("f7")) == std::vector<Move>{});
    VERIFY(g.moves(BP("g7")) == std::vector<Move>{});
    VERIFY(g.moves(BP("h7")) == std::vector<Move>{});

    VERIFY(g.moves(BP("a8")) == std::vector<Move>{Move("a8 b8"), Move("a8 c8"), Move("a8 a7"), Move("a8 a6"), Move("a8 a5")});
    VERIFY(g.moves(BP("b8")) == std::vector<Move>{});
    VERIFY(g.moves(BP("c8")) == std::vector<Move>{});
    VERIFY(g.moves(BP("d8")) == std::vector<Move>{});
    VERIFY(g.moves(BP("e8")) == std::vector<Move>{});
    VERIFY(g.moves(BP("f8")) == std::vector<Move>{Move("f8 e8"), Move("f8 d8"), Move("f8 c8")});  // removes check moves
    VERIFY(g.moves(BP("g8")) == std::vector<Move>{Move("g8 h7"), Move("g8 g7"), Move("g8 f7")});  // removes check moves
    VERIFY(g.moves(BP("h8")) == std::vector<Move>{});

    VERIFY(g.moves(BP("a8")).at(0).type() == MoveType::Normal);
    VERIFY(g.moves(BP("a8")).at(1).type() == MoveType::Capture);
    VERIFY(g.moves(BP("f8")).at(0).type() == MoveType::Normal);
    VERIFY(g.moves(BP("f8")).at(1).type() == MoveType::Normal);
    VERIFY(g.moves(BP("f8")).at(2).type() == MoveType::Capture);
    VERIFY(g.moves(BP("g8")).at(0).type() == MoveType::Normal);
    VERIFY(g.moves(BP("g8")).at(1).type() == MoveType::Normal);
    VERIFY(g.moves(BP("g8")).at(2).type() == MoveType::Capture);
}

// Castles, promotions, en passant
void gameTest3()
{
    Game g("r3kb1r/1pp1q1P1/3p1n1p/pP2Q3/2Pn1p2/N3PB1p/PB3P2/R3K2R w Qkq a6 0 24");

    VERIFY(g.moves(BP("a1")) == std::vector<Move>{Move("a1 b1"), Move("a1 c1"), Move("a1 d1")});
    VERIFY(g.moves(BP("b1")) == std::vector<Move>{});
    VERIFY(g.moves(BP("c1")) == std::vector<Move>{});
    VERIFY(g.moves(BP("d1")) == std::vector<Move>{});
    VERIFY(g.moves(BP("e1")) == std::vector<Move>{Move("e1 f1"), Move("e1 d1"), Move("e1 d2"), Move("e1 c1")}); // queen side castle, remove check move
    VERIFY(g.moves(BP("f1")) == std::vector<Move>{});
    VERIFY(g.moves(BP("g1")) == std::vector<Move>{});
    VERIFY(g.moves(BP("h1")) == std::vector<Move>{Move("h1 h2"), Move("h1 h3"), Move("h1 g1"), Move("h1 f1")});

    VERIFY(g.moves(BP("e1")).at(3).type() == MoveType::LongCastle);

    VERIFY(g.moves(BP("a2")) == std::vector<Move>{});
    VERIFY(g.moves(BP("b2")) == std::vector<Move>{Move("b2 c3"), Move("b2 d4"), Move("b2 c1")});
    VERIFY(g.moves(BP("c2")) == std::vector<Move>{});
    VERIFY(g.moves(BP("d2")) == std::vector<Move>{});
    VERIFY(g.moves(BP("e2")) == std::vector<Move>{});
    VERIFY(g.moves(BP("f2")) == std::vector<Move>{});
    VERIFY(g.moves(BP("g2")) == std::vector<Move>{});
    VERIFY(g.moves(BP("h2")) == std::vector<Move>{});

    VERIFY(g.moves(BP("a3")) == std::vector<Move>{Move("a3 c2"), Move("a3 b1")});
    VERIFY(g.moves(BP("b3")) == std::vector<Move>{});
    VERIFY(g.moves(BP("c3")) == std::vector<Move>{});
    VERIFY(g.moves(BP("d3")) == std::vector<Move>{});
    VERIFY(g.moves(BP("e3")) == std::vector<Move>{Move("e3 e4"), Move("e3 d4"), Move("e3 f4")});
    VERIFY(g.moves(BP("f3")) == std::vector<Move>{Move("f3 g4"), Move("f3 h5"), Move("f3 g2"), Move("f3 e2"), Move("f3 d1"), Move("f3 e4"), Move("f3 d5"), Move("f3 c6"), Move("f3 b7")});
    VERIFY(g.moves(BP("g3")) == std::vector<Move>{});
    VERIFY(g.moves(BP("h3")) == std::vector<Move>{});

    VERIFY(g.moves(BP("a4")) == std::vector<Move>{});
    VERIFY(g.moves(BP("b4")) == std::vector<Move>{});
    VERIFY(g.moves(BP("c4")) == std::vector<Move>{Move("c4 c5")});
    VERIFY(g.moves(BP("d4")) == std::vector<Move>{});
    VERIFY(g.moves(BP("e4")) == std::vector<Move>{});
    VERIFY(g.moves(BP("f4")) == std::vector<Move>{});
    VERIFY(g.moves(BP("g4")) == std::vector<Move>{});
    VERIFY(g.moves(BP("h4")) == std::vector<Move>{});

    VERIFY(g.moves(BP("a5")) == std::vector<Move>{});
    VERIFY(g.moves(BP("b5")) == std::vector<Move>{Move("b5 b6"), Move("b5 a6")}); // en-passant move
    VERIFY(g.moves(BP("c5")) == std::vector<Move>{});
    VERIFY(g.moves(BP("d5")) == std::vector<Move>{});
    VERIFY(g.moves(BP("e5")) == std::vector<Move>{Move("e5 e6"), Move("e5 e7"), Move("e5 f5"), Move("e5 g5"), Move("e5 h5"), Move("e5 e4"), Move("e5 d5"), Move("e5 c5"), Move("e5 f6"), Move("e5 f4"), Move("e5 d4"), Move("e5 d6")});
    VERIFY(g.moves(BP("f5")) == std::vector<Move>{});
    VERIFY(g.moves(BP("g5")) == std::vector<Move>{});
    VERIFY(g.moves(BP("h5")) == std::vector<Move>{});

    VERIFY(g.moves(BP("b5")).at(1).type() == MoveType::EnPassant);

    VERIFY(g.moves(BP("a6")) == std::vector<Move>{});
    VERIFY(g.moves(BP("b6")) == std::vector<Move>{});
    VERIFY(g.moves(BP("c6")) == std::vector<Move>{});
    VERIFY(g.moves(BP("d6")) == std::vector<Move>{});
    VERIFY(g.moves(BP("e6")) == std::vector<Move>{});
    VERIFY(g.moves(BP("f6")) == std::vector<Move>{});
    VERIFY(g.moves(BP("g6")) == std::vector<Move>{});
    VERIFY(g.moves(BP("h6")) == std::vector<Move>{});

    VERIFY(g.moves(BP("a7")) == std::vector<Move>{});
    VERIFY(g.moves(BP("b7")) == std::vector<Move>{});
    VERIFY(g.moves(BP("c7")) == std::vector<Move>{});
    VERIFY(g.moves(BP("d7")) == std::vector<Move>{});
    VERIFY(g.moves(BP("e7")) == std::vector<Move>{});
    VERIFY(g.moves(BP("f7")) == std::vector<Move>{});
    VERIFY(g.moves(BP("g7")) == std::vector<Move>{Move("g7 g8"), Move("g7 f8"), Move("g7 h8")}); // promotions
    VERIFY(g.moves(BP("h7")) == std::vector<Move>{});

    VERIFY(g.moves(BP("g7")).at(0).type() == MoveType::Promotion);
    VERIFY(g.moves(BP("g7")).at(1).type() == MoveType::Promotion);
    VERIFY(g.moves(BP("g7")).at(2).type() == MoveType::Promotion);

    VERIFY(g.moves(BP("a8")) == std::vector<Move>{});
    VERIFY(g.moves(BP("b8")) == std::vector<Move>{});
    VERIFY(g.moves(BP("c8")) == std::vector<Move>{});
    VERIFY(g.moves(BP("d8")) == std::vector<Move>{});
    VERIFY(g.moves(BP("e8")) == std::vector<Move>{});
    VERIFY(g.moves(BP("f8")) == std::vector<Move>{});
    VERIFY(g.moves(BP("g8")) == std::vector<Move>{});
    VERIFY(g.moves(BP("h8")) == std::vector<Move>{});
}

// Checks, moves prevented by check, moves()
void gameTest4()
{
    Game g;
    VERIFY(!g.isInCheck());

    g.setFENState("rnbqk1nr/p1p2ppp/1p6/2b1Q3/8/5N2/PPP2PPP/RNB1KB1R b KQkq - 0 7");
    VERIFY(g.isInCheck());

    VERIFY(g.moves(BP("a8")) == std::vector<Move>{});
    VERIFY(g.moves(BP("b8")) == std::vector<Move>{});
    VERIFY(g.moves(BP("c8")) == std::vector<Move>{Move("c8 e6")});
    VERIFY(g.moves(BP("d8")) == std::vector<Move>{Move("d8 e7")});
    VERIFY(g.moves(BP("e8")) == std::vector<Move>{Move("e8 f8"), Move("e8 d7")});
    VERIFY(g.moves(BP("f8")) == std::vector<Move>{});
    VERIFY(g.moves(BP("g8")) == std::vector<Move>{Move("g8 e7")});
    VERIFY(g.moves(BP("h8")) == std::vector<Move>{});

    VERIFY(g.moves(BP("a5")) == std::vector<Move>{});
    VERIFY(g.moves(BP("b5")) == std::vector<Move>{});
    VERIFY(g.moves(BP("c5")) == std::vector<Move>{Move("c5 e7")});
    VERIFY(g.moves(BP("d5")) == std::vector<Move>{});
    VERIFY(g.moves(BP("e5")) == std::vector<Move>{});
    VERIFY(g.moves(BP("f5")) == std::vector<Move>{});
    VERIFY(g.moves(BP("g5")) == std::vector<Move>{});
    VERIFY(g.moves(BP("h5")) == std::vector<Move>{});

    VERIFY(g.moves() == std::vector<Move>{Move("c5 e7"), Move("c8 e6"), Move("d8 e7"), Move("e8 f8"), Move("e8 d7"), Move("g8 e7")});

    g.setFENState("r3kb1r/1pp1q1P1/3p1n1p/pP2Q3/2Pn1p2/N3PB1p/PB3P2/R3K2R w Qkq a6 0 24");
    VERIFY(!g.isInCheck());
}

// Do not allow castle if in check
void gameTest5()
{
    Game g("rnbqk2r/ppp1bppp/2Bp4/4p2n/4P3/3P4/PPP2PPP/RNB1K1NR b KQkq - 1 6");
    VERIFY(g.canShortCastle(Player::Black));
    VERIFY(g.isInCheck());
    VERIFY(g.moves(BP("e8")) == std::vector<Move>{Move("e8 f8")});
}

// Do not allow castle if it results in king in check
void gameTest6()
{
    Game g("r1bnk2r/pppqb1pp/3p1p2/3Bp2n/3PP3/5PP1/PPP4P/RNB1K1NR b KQkq - 0 12");
    VERIFY(g.canShortCastle(Player::Black));
    VERIFY(!g.isInCheck());
    VERIFY(g.moves(BP("e8")) == std::vector<Move>{Move("e8 f8")});
}

// Do not allow castle if intermediate positions are under attack
void gameTest7()
{
    Game g("3rk2r/pbpq1npp/1p6/3npp2/3PP3/B4NP1/PPP4P/RN2K2R b KQk - 6 24");
    VERIFY(g.canShortCastle(Player::Black));
    VERIFY(!g.isInCheck());
    VERIFY(g.moves(BP("e8")) == std::vector<Move>{});
}

// isLegalMove()
void gameTest8()
{
    Game g("r3kb1r/1pp1q1P1/3p1n1p/pP2Q3/2Pn1p2/N3PB1p/PB3P2/R3K2R w Qkq a6 0 24");
    VERIFY(g.isLegalMove(BP("a3"), BP("c2")));
    VERIFY(g.isLegalMove(BP("e5"), BP("e7")));
    VERIFY(!g.isLegalMove(BP("e5"), BP("g7")));
    VERIFY(!g.isLegalMove(BP("d1"), BP("d2")));
}

// moveType()
void gameTest9()
{
    Game g("rnbk2nr/1ppq2pP/5b2/pP1pQ3/4pp2/B1NBPN2/P1PP1PP1/R3K2R w KQ a6 0 16");
    VERIFY(g.moveType(BP("a3"), BP("f8")) == MoveType::Normal);
    VERIFY(g.moveType(BP("e5"), BP("f4")) == MoveType::Capture);
    VERIFY(g.moveType(BP("b5"), BP("a6")) == MoveType::EnPassant);
    VERIFY(g.moveType(BP("e1"), BP("g1")) == MoveType::ShortCastle);
    VERIFY(g.moveType(BP("e1"), BP("c1")) == MoveType::LongCastle);
    VERIFY(g.moveType(BP("h7"), BP("g8")) == MoveType::Promotion);
}

// move()
void gameTest10()
{
    Game g("rnbk2nr/1ppq2pP/5b2/pP1pQ3/4pp2/B1NBPN2/P1PP1PP1/R3K2R w KQ a6 0 16");
    VERIFY(g.move(BP("e5"), BP("f4")) == Move(BP("e5"), BP("f4"), MoveType::Capture));
    VERIFY(g.move(BP("b5"), BP("a6")) == Move(BP("b5"), BP("a6"), MoveType::EnPassant));
    VERIFY(g.move(BP("e1"), BP("g1")) == Move(BP("e1"), BP("g1"), MoveType::ShortCastle));
}

// moves()
void gameTest11()
{
    Game g("8/kp2n3/1r6/p4b2/PB6/3R4/8/1K6 w - - 0 42");
    VERIFY(g.moves() == std::vector<Move>{Move("b1 b2"), Move("b1 c2"), Move("b1 c1"), Move("b1 a1"), Move("b1 a2")});

    g.setFENState("1n2R3/kp6/1r6/p7/P5b1/2Bp4/3R4/1K6 w - - 3 39");
    VERIFY(g.moves() == std::vector<Move>{Move("b1 c1"), Move("b1 a1"), Move("b1 a2"), Move("c3 b2"), Move("c3 b4"), Move("d2 b2")});
}

}; // unnamed namespace

void Chess::Tester::gameComputeMoveTests()
{
    RUN(gameTest1);
    RUN(gameTest2);
    RUN(gameTest3);
    RUN(gameTest4);
    RUN(gameTest5);
    RUN(gameTest6);
    // RUN(gameTest7); // TODO
    RUN(gameTest8);
    RUN(gameTest9);
    RUN(gameTest10);
    RUN(gameTest11);
}

