#ifndef TEST_H
#define TEST_H

#include "../chess/piece.h"
#include "../chess/move.h"
#include <iostream>

#define RUN_TESTS false
#define RUN_SUITE(suite) SEP1; numSuccesses=0; numTests=0; suite(); if(numSuccesses < numTests) {totalSuccess=false; SUITE_FAIL(suite);} else SUITE_OK(suite);
#define RUN(test) try {test(); TEST_OK(test); numSuccesses++;} catch(...){TEST_FAIL(test);}numTests++;
#define VERIFY(...) if (!(__VA_ARGS__)) {throw 1;}
#define VERIFY_THROWS(...) try {__VA_ARGS__; VERIFY(false);} catch(...){VERIFY(true)}

#define SEP1 std::cout << "=======================\n"
#define SEP2 std::cout << "-----------------------\n"

#define SUITE_OK(name)  SEP2 << #name << " OK " << RATE
#define SUITE_FAIL(name)  SEP2 << #name << " FAIL " << RATE
#define RATE "(" << numSuccesses << "/" << numTests << ")\n"

#define TEST_OK(name)  std::cout << #name << " OK\n"
#define TEST_FAIL(name)  std::cout << #name << " FAIL\n"

namespace Chess
{

class Tester
{
public:
    static void run()
    {
        Tester().runAll();
    }

private:

    void runAll()
    {
        std::cout << "RUNNING ALL TESTS\n";
        RUN_SUITE(boardposTests);
        RUN_SUITE(pieceTests);
        RUN_SUITE(moveTests);
        RUN_SUITE(stateTests);
        RUN_SUITE(boardTests);
        RUN_SUITE(positionTests);
        RUN_SUITE(gameComputeMoveTests);
        RUN_SUITE(gamePerformMoveTests);
        RUN_SUITE(gameResultTests);

        SEP1;
        if (totalSuccess)
        {
            std::cout << "ALL TESTS SUCCESSFUL :)\n\n";
        }
        else
        {
            std::cout << "SOME TESTS FAILED :(\n\n";
        }
    }

    void boardposTests();
    void pieceTests();
    void moveTests();
    void stateTests();
    void boardTests();
    void positionTests();
    void gameComputeMoveTests();
    void gamePerformMoveTests();
    void gameResultTests();

    int numSuccesses = 0;
    int numTests = 0;
    bool totalSuccess = true;
};

inline void verifyPiece(const Piece& p, Player o, PieceType t, bool isCaptured = false)
{
    VERIFY(p.owner() == o && p.type() == t && p.isCaptured() == isCaptured);
}

inline void verifyMove(const Move& m, const BP& src, const BP& dst, MoveType type)
{
    VERIFY(m.src() == src && m.dst() == dst && m.type() == type);
}

}; // namespace Chess

#endif // TEST_H
