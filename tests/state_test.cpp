#include "test.h"
#include "../chess/state.h"

#include <sstream>
#include <array>

namespace
{

using namespace Chess;

void stateTest1()
{
    State s;
    VERIFY(s.numHalfMoves() == 0);
    VERIFY(s.numFullMoves() == 1);
    VERIFY(s.activePlayer() == Player::White);
    VERIFY(s.canShortCastle(Player::Black) == true);
    VERIFY(s.canShortCastle(Player::White) == true);
    VERIFY(s.canLongCastle(Player::Black) == true);
    VERIFY(s.canLongCastle(Player::White) == true);
    VERIFY(s.canEnPassant() == false);
    VERIFY(s.enPassantPosition() == BP(-1, -1));
}

void stateTest2()
{
    State s;
    VERIFY(s.activePlayer() == Player::White);
    s.setActivePlayer(Player::Black);
    VERIFY(s.activePlayer() == Player::Black);
    s.switchActivePlayer();
    VERIFY(s.activePlayer() == Player::White);
}

void stateTest3()
{
    State s("b Kq e4 11 120");
    VERIFY(s.numHalfMoves() == 11);
    VERIFY(s.numFullMoves() == 120);
    VERIFY(s.activePlayer() == Player::Black);
    VERIFY(s.canShortCastle(Player::Black) == false);
    VERIFY(s.canShortCastle(Player::White) == true);
    VERIFY(s.canLongCastle(Player::Black) == true);
    VERIFY(s.canLongCastle(Player::White) == false);
    VERIFY(s.canEnPassant() == true);
    VERIFY(s.enPassantPosition() == BP("e4"));

    s.setFENState("w KQkq - 0 1");
    VERIFY(s.numHalfMoves() == 0);
    VERIFY(s.numFullMoves() == 1);
    VERIFY(s.activePlayer() == Player::White);
    VERIFY(s.canShortCastle(Player::Black) == true);
    VERIFY(s.canShortCastle(Player::White) == true);
    VERIFY(s.canLongCastle(Player::Black) == true);
    VERIFY(s.canLongCastle(Player::White) == true);
    VERIFY(s.canEnPassant() == false);
    VERIFY(s.enPassantPosition() == BP(-1, -1));

    s.setFENState(" b Qk g8 1 99");
    VERIFY(s.numHalfMoves() == 1);
    VERIFY(s.numFullMoves() == 99);
    VERIFY(s.activePlayer() == Player::Black);
    VERIFY(s.canShortCastle(Player::Black) == true);
    VERIFY(s.canShortCastle(Player::White) == false);
    VERIFY(s.canLongCastle(Player::Black) == false);
    VERIFY(s.canLongCastle(Player::White) == true);
    VERIFY(s.canEnPassant() == true);
    VERIFY(s.enPassantPosition() == BP("g8"));
}

}; // unnamed namespace

void Chess::Tester::stateTests()
{
    RUN(stateTest1);
    RUN(stateTest2);
    RUN(stateTest3);
}

