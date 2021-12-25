#include "test.h"
#include "../chess/boardpos.h"

#include <sstream>

namespace
{

using namespace Chess;

void boardposTest1()
{
    BP pos(5, 6);
    VERIFY(pos.row() == 5);
    VERIFY(pos.col() == 6);
}

void boardposTest2()
{
    BP p1(0, 7);
    VERIFY(p1.isValid())

    BP p2(-1, -1);
    VERIFY(!p2.isValid());

    BP p3(0, 9);
    VERIFY(!p3.isValid());

    BP p4(-1, 7);
    VERIFY(!p4.isValid());
}

void boardposTest3()
{
    BP p1(0, 7);
    BP p2(0, 7);
    VERIFY(p1 == p2);

    BP p3(0, 8);
    VERIFY(p1 != p3);
}

void boardposTest4()
{
    VERIFY(BP(0, 0).toANBP() == "a8");
    VERIFY(BP(1, 1).toANBP() == "b7");
    VERIFY(BP(2, 2).toANBP() == "c6");
    VERIFY(BP(3, 3).toANBP() == "d5");
    VERIFY(BP(4, 4).toANBP() == "e4");
    VERIFY(BP(5, 5).toANBP() == "f3");
    VERIFY(BP(6, 6).toANBP() == "g2");
    VERIFY(BP(7, 7).toANBP() == "h1");
}

void boardposTest5()
{
    VERIFY(BP("a8") == BP(0, 0));
    VERIFY(BP("b7") == BP(1, 1));
    VERIFY(BP("c6") == BP(2, 2));
    VERIFY(BP("d5") == BP(3, 3));
    VERIFY(BP("e4") == BP(4, 4));
    VERIFY(BP("f3") == BP(5, 5));
    VERIFY(BP("g2") == BP(6, 6));
    VERIFY(BP("h1") == BP(7, 7));
}

void boardposTest6()
{
    VERIFY(BP(-1, 0).toANBP() == "");
    VERIFY(BP(0, 11).toANBP() == "");

    VERIFY(!BP("").isValid());
    VERIFY(!BP("a9").isValid());
    VERIFY(!BP("j1").isValid());
}

void boardposTest7()
{
    std::ostringstream ss;
    ss << BP("a1") << BP("d4");
    VERIFY(ss.str() == "a1d4");
}

}; // unnamed namespace

void Chess::Tester::boardposTests()
{
    RUN(boardposTest1);
    RUN(boardposTest2);
    RUN(boardposTest3);
    RUN(boardposTest4);
    RUN(boardposTest5);
    RUN(boardposTest6);
    RUN(boardposTest7);
}

