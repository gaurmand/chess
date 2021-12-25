#include "test.h"
#include "../chess/piece.h"

#include <sstream>

namespace
{

using namespace Chess;

void pieceTest1()
{
    Piece p(PieceType::Bishop, Player::Black, BP(5,6), 5);
    VERIFY(p.type() == PieceType::Bishop);
    VERIFY(p.owner() == Player::Black);
    VERIFY(p.pos() == BP(5,6));
    VERIFY(p.id() ==  5);
    VERIFY(!p.isCaptured());
    VERIFY(p.isValid());
}

void pieceTest2()
{
    Piece p{PieceType::Bishop, Player::Black, BP(4,5), 8, true};
    VERIFY(p.type() == PieceType::Bishop);
    VERIFY(p.owner() == Player::Black);
    VERIFY(p.pos() == BP(4,5));
    VERIFY(p.id() ==  8);
    VERIFY(p.isCaptured());
    VERIFY(p.isValid());
}

void pieceTest3()
{
    Piece p{PieceType::Bishop, Player::Black, BP(5,6), 8};
    VERIFY(!p.isCaptured());
    p.capture();
    VERIFY(p.isCaptured());
}

void pieceTest4()
{
    Piece p;
    VERIFY(!p.isValid());
    VERIFY(p.isCaptured());
}

void pieceTest5()
{
    std::ostringstream ss;
    Piece b1{PieceType::King, Player::Black, BP(), 0};
    Piece b2{PieceType::Queen, Player::Black, BP(), 1};
    Piece b3{PieceType::Rook, Player::Black, BP(), 2};
    Piece b4{PieceType::Bishop, Player::Black,BP(), 3};
    Piece b5{PieceType::Knight, Player::Black, BP(), 4};
    Piece b6{PieceType::Pawn, Player::Black, BP(), 5};

    ss << b1 << b2 << b3 << b4 << b5 << b6;

    Piece w1{PieceType::King, Player::White, BP(), 0};
    Piece w2{PieceType::Queen, Player::White, BP(), 1};
    Piece w3{PieceType::Rook, Player::White, BP(), 2};
    Piece w4{PieceType::Bishop, Player::White, BP(), 3};
    Piece w5{PieceType::Knight, Player::White, BP(), 4};
    Piece w6{PieceType::Pawn, Player::White, BP(), 5};

    ss << w1 << w2 << w3 << w4 << w5 << w6;

    VERIFY(ss.str() == "kqrbnpKQRBNP");
}

}; // unnamed namespace

void Chess::Tester::pieceTests()
{
    RUN(pieceTest1);
    RUN(pieceTest2);
    RUN(pieceTest3);
    RUN(pieceTest4);
    RUN(pieceTest5);
}

