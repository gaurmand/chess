#include "test.h"
#include "../chess/move.h"

#include <sstream>

namespace
{

using namespace Chess;

void moveTest1()
{
    Move m;
    VERIFY(m.src() == BP(-1, -1));
    VERIFY(m.dst() == BP(-1, -1));
    VERIFY(m.type() == MoveType::Unknown);
}

void moveTest2()
{
    Move m(BP(0, 0), BP(1, 1));
    VERIFY(m.src() == BP(0, 0));
    VERIFY(m.dst() == BP(1, 1));
    VERIFY(m.type() == MoveType::Unknown);

}

void moveTest3()
{
    Move m(BP(6, 6), BP(7, 7), MoveType::ShortCastle);
    VERIFY(m.src() == BP(6, 6));
    VERIFY(m.dst() == BP(7, 7));
    VERIFY(m.type() == MoveType::ShortCastle);
}

void moveTest4()
{
    Move m1;
    VERIFY(!m1.isValid());

    Move m2(BP(6, 6), BP(7, 7), MoveType::ShortCastle);
    VERIFY(m2.isValid());

    Move m3(BP(6, -1), BP(7, 7), MoveType::ShortCastle);
    VERIFY(!m3.isValid());
}

void moveTest5()
{
    Move m1(BP(6, 6), BP(7, 7), MoveType::ShortCastle);
    Move m2(BP(6, 6), BP(7, 7), MoveType::LongCastle);
    VERIFY(m1 == m2);

    Move m3(BP(6, 5), BP(7, 7), MoveType::LongCastle);
    VERIFY(m1 != m3);
}

void moveTest6()
{
    VERIFY(Move(BP("a1"), BP("h8"), MoveType::ShortCastle).toLANMove() == "a1 h8");
    VERIFY(Move(BP("b8"), BP("g5"), MoveType::ShortCastle).toLANMove() == "b8 g5");
}

void moveTest7()
{
    VERIFY(Move("a1 h8") == Move(BP("a1"), BP("h8")));
    VERIFY(Move("b8 g5") == Move(BP("b8"), BP("g5")));
}

void moveTest8()
{
    VERIFY(Move(BP(-1, 0), BP(1,1)).toLANMove() == "");
    VERIFY(Move(BP(0, 0), BP(1,11)).toLANMove() == "");

    VERIFY(!Move("").isValid());
    VERIFY(!Move("a9 h8").isValid());
    VERIFY(!Move("b2 r8").isValid());
    VERIFY(!Move("b3 f").isValid());
    VERIFY(!Move("g8 f*").isValid());
}

void moveTest9()
{
    std::ostringstream ss;
    ss << Move("c4 h2") << Move("f7 a1");
    VERIFY(ss.str() == "c4 h2f7 a1");
}

}; // unnamed namespace

void Chess::Tester::moveTests()
{
    RUN(moveTest1);
    RUN(moveTest2);
    RUN(moveTest3);
    RUN(moveTest4);
    RUN(moveTest5);
    RUN(moveTest6);
    RUN(moveTest7);
    RUN(moveTest8);
    RUN(moveTest9);
}

