#include "test_helper.h"

#include <chessmodel/game.h>
#include <gtest/gtest.h>

using namespace chess;

TEST(GameTest, normalMove)
{
   Board b;
   State s;
   b.set(Position::d1, Piece::Queen, Colour::White);
   Game g(b, s);

   Move mv;
   mv.setActive(Colour::White);
   mv.setSrc(Position::d1);
   mv.setDst(Position::d8);
   mv.setPiece(Piece::Queen);

   EXPECT_TRUE(g.board().occupied(Position::d1));
   EXPECT_FALSE(g.board().occupied(Position::d8));
   EXPECT_EQ(g.board().piece(Position::d1), Piece::Queen);
   EXPECT_EQ(g.board().colour(Position::d1), Colour::White);

   g.move(mv);

   EXPECT_FALSE(g.board().occupied(Position::d1));
   EXPECT_TRUE(g.board().occupied(Position::d8));
   EXPECT_EQ(g.board().piece(Position::d8), Piece::Queen);
   EXPECT_EQ(g.board().colour(Position::d8), Colour::White);

   g.unmove(mv);

   EXPECT_TRUE(g.board().occupied(Position::d1));
   EXPECT_FALSE(g.board().occupied(Position::d8));
   EXPECT_EQ(g.board().piece(Position::d1), Piece::Queen);
   EXPECT_EQ(g.board().colour(Position::d1), Colour::White);
}

TEST(GameTest, capture)
{
   Board b;
   State s;
   b.set(Position::b6, Piece::Queen, Colour::Black);
   b.set(Position::g6, Piece::Pawn, Colour::White);
   Game g(b, s);

   Move mv;
   mv.setActive(Colour::Black);
   mv.setSrc(Position::b6);
   mv.setDst(Position::g6);
   mv.setPiece(Piece::Queen);
   mv.setCapturedPiece(Piece::Pawn);

   EXPECT_TRUE(g.board().occupied(Position::b6));
   EXPECT_TRUE(g.board().occupied(Position::g6));
   EXPECT_EQ(g.board().piece(Position::b6), Piece::Queen);
   EXPECT_EQ(g.board().colour(Position::b6), Colour::Black);
   EXPECT_TRUE(g.board().occupied(Position::g6));
   EXPECT_EQ(g.board().piece(Position::g6), Piece::Pawn);
   EXPECT_EQ(g.board().colour(Position::g6), Colour::White);

   g.move(mv);

   EXPECT_FALSE(g.board().occupied(Position::b6));
   EXPECT_TRUE(g.board().occupied(Position::g6));
   EXPECT_EQ(g.board().piece(Position::g6), Piece::Queen);
   EXPECT_EQ(g.board().colour(Position::g6), Colour::Black);

   g.unmove(mv);

   EXPECT_TRUE(g.board().occupied(Position::b6));
   EXPECT_TRUE(g.board().occupied(Position::g6));
   EXPECT_EQ(g.board().piece(Position::b6), Piece::Queen);
   EXPECT_EQ(g.board().colour(Position::b6), Colour::Black);
   EXPECT_TRUE(g.board().occupied(Position::g6));
   EXPECT_EQ(g.board().piece(Position::g6), Piece::Pawn);
   EXPECT_EQ(g.board().colour(Position::g6), Colour::White);
}

TEST(GameTest, promotion)
{
   Board b;
   State s;
   b.set(Position::e7, Piece::Pawn, Colour::White);
   Game g(b, s);

   Move mv;
   mv.setActive(Colour::White);
   mv.setSrc(Position::e7);
   mv.setDst(Position::e8);
   mv.setPiece(Piece::Pawn);
   mv.setPromotedPiece(Piece::Queen);

   EXPECT_TRUE(g.board().occupied(Position::e7));
   EXPECT_FALSE(g.board().occupied(Position::e8));
   EXPECT_EQ(g.board().piece(Position::e7), Piece::Pawn);
   EXPECT_EQ(g.board().colour(Position::e7), Colour::White);

   g.move(mv);

   EXPECT_FALSE(g.board().occupied(Position::e7));
   EXPECT_TRUE(g.board().occupied(Position::e8));
   EXPECT_EQ(g.board().piece(Position::e8), Piece::Queen);
   EXPECT_EQ(g.board().colour(Position::e8), Colour::White);

   g.unmove(mv);

   EXPECT_TRUE(g.board().occupied(Position::e7));
   EXPECT_FALSE(g.board().occupied(Position::e8));
   EXPECT_EQ(g.board().piece(Position::e7), Piece::Pawn);
   EXPECT_EQ(g.board().colour(Position::e7), Colour::White);
}

TEST(GameTest, promotionCapture)
{
   Board b;
   State s;
   b.set(Position::b2, Piece::Pawn, Colour::Black);
   b.set(Position::a1, Piece::Rook, Colour::White);
   Game g(b, s);

   Move mv;
   mv.setActive(Colour::Black);
   mv.setSrc(Position::b2);
   mv.setDst(Position::a1);
   mv.setPiece(Piece::Pawn);
   mv.setCapturedPiece(Piece::Rook);
   mv.setPromotedPiece(Piece::Knight);

   EXPECT_TRUE(g.board().occupied(Position::b2));
   EXPECT_EQ(g.board().piece(Position::b2), Piece::Pawn);
   EXPECT_EQ(g.board().colour(Position::b2), Colour::Black);
   EXPECT_TRUE(g.board().occupied(Position::a1));
   EXPECT_EQ(g.board().piece(Position::a1), Piece::Rook);
   EXPECT_EQ(g.board().colour(Position::a1), Colour::White);

   g.move(mv);

   EXPECT_FALSE(g.board().occupied(Position::b2));
   EXPECT_TRUE(g.board().occupied(Position::a1));
   EXPECT_EQ(g.board().piece(Position::a1), Piece::Knight);
   EXPECT_EQ(g.board().colour(Position::a1), Colour::Black);

   g.unmove(mv);

   EXPECT_TRUE(g.board().occupied(Position::b2));
   EXPECT_EQ(g.board().piece(Position::b2), Piece::Pawn);
   EXPECT_EQ(g.board().colour(Position::b2), Colour::Black);
   EXPECT_TRUE(g.board().occupied(Position::a1));
   EXPECT_EQ(g.board().piece(Position::a1), Piece::Rook);
   EXPECT_EQ(g.board().colour(Position::a1), Colour::White);
}

TEST(GameTest, enPassant)
{
   Board b;
   State s;
   s.enPassant = Position::e6;
   b.set(Position::d5, Piece::Pawn, Colour::White);
   b.set(Position::e5, Piece::Pawn, Colour::Black);
   Game g(b, s);

   Move mv;
   mv.setActive(Colour::White);
   mv.setSrc(Position::d5);
   mv.setDst(Position::e6);
   mv.setPiece(Piece::Pawn);
   mv.setCapturedPiece(Piece::Pawn);
   mv.setEnPassant(true);

   EXPECT_EQ(g.state().enPassant, Position::e6);
   EXPECT_TRUE(g.board().occupied(Position::d5));
   EXPECT_EQ(g.board().piece(Position::d5), Piece::Pawn);
   EXPECT_EQ(g.board().colour(Position::d5), Colour::White);
   EXPECT_TRUE(g.board().occupied(Position::e5));
   EXPECT_EQ(g.board().piece(Position::e5), Piece::Pawn);
   EXPECT_EQ(g.board().colour(Position::e5), Colour::Black);

   g.move(mv);

   EXPECT_FALSE(g.board().occupied(Position::d5));
   EXPECT_FALSE(g.board().occupied(Position::e5));
   EXPECT_TRUE(g.board().occupied(Position::e6));
   EXPECT_EQ(g.board().piece(Position::e6), Piece::Pawn);
   EXPECT_EQ(g.board().colour(Position::e6), Colour::White);
   EXPECT_EQ(g.state().enPassant, Position::Invalid);

   g.unmove(mv);

   EXPECT_EQ(g.state().enPassant, Position::e6);
   EXPECT_TRUE(g.board().occupied(Position::d5));
   EXPECT_EQ(g.board().piece(Position::d5), Piece::Pawn);
   EXPECT_EQ(g.board().colour(Position::d5), Colour::White);
   EXPECT_TRUE(g.board().occupied(Position::e5));
   EXPECT_EQ(g.board().piece(Position::e5), Piece::Pawn);
   EXPECT_EQ(g.board().colour(Position::e5), Colour::Black);
}

TEST(GameTest, enPassantMissedOpportunity)
{
   Board b;
   State s;
   s.enPassant = Position::e6;
   b.set(Position::d5, Piece::Pawn, Colour::White);
   b.set(Position::e5, Piece::Pawn, Colour::Black);
   Game g(b, s);

   Move mv;
   mv.setActive(Colour::White);
   mv.setSrc(Position::d5);
   mv.setDst(Position::d6);
   mv.setPiece(Piece::Pawn);

   EXPECT_EQ(g.state().enPassant, Position::e6);
   EXPECT_TRUE(g.board().occupied(Position::d5));
   EXPECT_EQ(g.board().piece(Position::d5), Piece::Pawn);
   EXPECT_EQ(g.board().colour(Position::d5), Colour::White);
   EXPECT_TRUE(g.board().occupied(Position::e5));
   EXPECT_EQ(g.board().piece(Position::e5), Piece::Pawn);
   EXPECT_EQ(g.board().colour(Position::e5), Colour::Black);

   g.move(mv);

   EXPECT_EQ(g.state().enPassant, Position::Invalid);
   EXPECT_FALSE(g.board().occupied(Position::d5));
   EXPECT_TRUE(g.board().occupied(Position::d6));
   EXPECT_EQ(g.board().piece(Position::d6), Piece::Pawn);
   EXPECT_EQ(g.board().colour(Position::d6), Colour::White);

   g.unmove(mv);

   EXPECT_EQ(g.state().enPassant, Position::e6);
   EXPECT_TRUE(g.board().occupied(Position::d5));
   EXPECT_EQ(g.board().piece(Position::d5), Piece::Pawn);
   EXPECT_EQ(g.board().colour(Position::d5), Colour::White);
   EXPECT_TRUE(g.board().occupied(Position::e5));
   EXPECT_EQ(g.board().piece(Position::e5), Piece::Pawn);
   EXPECT_EQ(g.board().colour(Position::e5), Colour::Black);
}

TEST(GameTest, wdoubleAdvance)
{
   Board b;
   State s;
   b.set(Position::c2, Piece::Pawn, Colour::White);
   Game g(b, s);

   Move mv;
   mv.setActive(Colour::White);
   mv.setSrc(Position::c2);
   mv.setDst(Position::c4);
   mv.setPiece(Piece::Pawn);

   EXPECT_EQ(g.state().enPassant, Position::Invalid);
   EXPECT_TRUE(g.board().occupied(Position::c2));
   EXPECT_EQ(g.board().piece(Position::c2), Piece::Pawn);
   EXPECT_EQ(g.board().colour(Position::c2), Colour::White);

   g.move(mv);

   EXPECT_EQ(g.state().enPassant, Position::c3);
   EXPECT_TRUE(g.board().occupied(Position::c4));
   EXPECT_EQ(g.board().piece(Position::c4), Piece::Pawn);
   EXPECT_EQ(g.board().colour(Position::c4), Colour::White);

   g.unmove(mv);

   EXPECT_EQ(g.state().enPassant, Position::Invalid);
   EXPECT_TRUE(g.board().occupied(Position::c2));
   EXPECT_EQ(g.board().piece(Position::c2), Piece::Pawn);
   EXPECT_EQ(g.board().colour(Position::c2), Colour::White);
}

TEST(GameTest, bdoubleAdvance)
{
   Board b;
   State s;
   b.set(Position::f7, Piece::Pawn, Colour::Black);
   Game g(b, s);

   Move mv;
   mv.setActive(Colour::Black);
   mv.setSrc(Position::f7);
   mv.setDst(Position::f5);
   mv.setPiece(Piece::Pawn);

   EXPECT_TRUE(g.board().occupied(Position::f7));
   EXPECT_EQ(g.board().piece(Position::f7), Piece::Pawn);
   EXPECT_EQ(g.board().colour(Position::f7), Colour::Black);

   g.move(mv);

   EXPECT_EQ(g.state().enPassant, Position::f6);
   EXPECT_TRUE(g.board().occupied(Position::f5));
   EXPECT_EQ(g.board().piece(Position::f5), Piece::Pawn);
   EXPECT_EQ(g.board().colour(Position::f5), Colour::Black);

   g.unmove(mv);

   EXPECT_TRUE(g.board().occupied(Position::f7));
   EXPECT_EQ(g.board().piece(Position::f7), Piece::Pawn);
   EXPECT_EQ(g.board().colour(Position::f7), Colour::Black);
}

TEST(GameTest, wlongCastle)
{
   Board b;
   State s;
   b.set(Position::e1, Piece::King, Colour::White);
   b.set(Position::a1, Piece::Rook, Colour::White);
   Game g(b, s);

   Move mv;
   mv.setActive(Colour::White);
   mv.setSrc(Position::e1);
   mv.setDst(Position::c1);
   mv.setPiece(Piece::King);
   mv.setLongCastle(true);

   EXPECT_TRUE(g.state().longCastle[Colour::White]);
   EXPECT_TRUE(g.state().shortCastle[Colour::White]);
   EXPECT_TRUE(g.board().occupied(Position::e1));
   EXPECT_EQ(g.board().piece(Position::e1), Piece::King);
   EXPECT_EQ(g.board().colour(Position::e1), Colour::White);
   EXPECT_TRUE(g.board().occupied(Position::a1));
   EXPECT_EQ(g.board().piece(Position::a1), Piece::Rook);
   EXPECT_EQ(g.board().colour(Position::a1), Colour::White);

   g.move(mv);

   EXPECT_FALSE(g.state().longCastle[Colour::White]);
   EXPECT_FALSE(g.state().shortCastle[Colour::White]);
   EXPECT_FALSE(g.board().occupied(Position::e1));
   EXPECT_FALSE(g.board().occupied(Position::a1));
   EXPECT_TRUE(g.board().occupied(Position::c1));
   EXPECT_EQ(g.board().piece(Position::c1), Piece::King);
   EXPECT_EQ(g.board().colour(Position::c1), Colour::White);
   EXPECT_TRUE(g.board().occupied(Position::d1));
   EXPECT_EQ(g.board().piece(Position::d1), Piece::Rook);
   EXPECT_EQ(g.board().colour(Position::d1), Colour::White);

   g.unmove(mv);

   EXPECT_TRUE(g.state().longCastle[Colour::White]);
   EXPECT_TRUE(g.state().shortCastle[Colour::White]);
   EXPECT_TRUE(g.board().occupied(Position::e1));
   EXPECT_EQ(g.board().piece(Position::e1), Piece::King);
   EXPECT_EQ(g.board().colour(Position::e1), Colour::White);
   EXPECT_TRUE(g.board().occupied(Position::a1));
   EXPECT_EQ(g.board().piece(Position::a1), Piece::Rook);
   EXPECT_EQ(g.board().colour(Position::a1), Colour::White);
}

TEST(GameTest, blongCastle)
{
   Board b;
   State s;
   b.set(Position::e8, Piece::King, Colour::Black);
   b.set(Position::a8, Piece::Rook, Colour::Black);
   Game g(b, s);

   Move mv;
   mv.setActive(Colour::Black);
   mv.setSrc(Position::e8);
   mv.setDst(Position::c8);
   mv.setPiece(Piece::King);
   mv.setLongCastle(true);

   EXPECT_TRUE(g.state().longCastle[Colour::Black]);
   EXPECT_TRUE(g.state().shortCastle[Colour::Black]);
   EXPECT_TRUE(g.board().occupied(Position::e8));
   EXPECT_EQ(g.board().piece(Position::e8), Piece::King);
   EXPECT_EQ(g.board().colour(Position::e8), Colour::Black);
   EXPECT_TRUE(g.board().occupied(Position::a8));
   EXPECT_EQ(g.board().piece(Position::a8), Piece::Rook);
   EXPECT_EQ(g.board().colour(Position::a8), Colour::Black);

   g.move(mv);

   EXPECT_FALSE(g.state().longCastle[Colour::Black]);
   EXPECT_FALSE(g.state().shortCastle[Colour::Black]);
   EXPECT_FALSE(g.board().occupied(Position::e8));
   EXPECT_FALSE(g.board().occupied(Position::a8));
   EXPECT_TRUE(g.board().occupied(Position::c8));
   EXPECT_EQ(g.board().piece(Position::c8), Piece::King);
   EXPECT_EQ(g.board().colour(Position::c8), Colour::Black);
   EXPECT_TRUE(g.board().occupied(Position::d8));
   EXPECT_EQ(g.board().piece(Position::d8), Piece::Rook);
   EXPECT_EQ(g.board().colour(Position::d8), Colour::Black);

   g.unmove(mv);

   EXPECT_TRUE(g.state().longCastle[Colour::Black]);
   EXPECT_TRUE(g.state().shortCastle[Colour::Black]);
   EXPECT_TRUE(g.board().occupied(Position::e8));
   EXPECT_EQ(g.board().piece(Position::e8), Piece::King);
   EXPECT_EQ(g.board().colour(Position::e8), Colour::Black);
   EXPECT_TRUE(g.board().occupied(Position::a8));
   EXPECT_EQ(g.board().piece(Position::a8), Piece::Rook);
   EXPECT_EQ(g.board().colour(Position::a8), Colour::Black);
}

TEST(GameTest, wshortCastle)
{
   Board b;
   State s;
   b.set(Position::e1, Piece::King, Colour::White);
   b.set(Position::h1, Piece::Rook, Colour::White);
   Game g(b, s);

   Move mv;
   mv.setActive(Colour::White);
   mv.setSrc(Position::e1);
   mv.setDst(Position::g1);
   mv.setPiece(Piece::King);
   mv.setShortCastle(true);

   EXPECT_TRUE(g.state().longCastle[Colour::White]);
   EXPECT_TRUE(g.state().shortCastle[Colour::White]);
   EXPECT_TRUE(g.board().occupied(Position::e1));
   EXPECT_EQ(g.board().piece(Position::e1), Piece::King);
   EXPECT_EQ(g.board().colour(Position::e1), Colour::White);
   EXPECT_TRUE(g.board().occupied(Position::h1));
   EXPECT_EQ(g.board().piece(Position::h1), Piece::Rook);
   EXPECT_EQ(g.board().colour(Position::h1), Colour::White);

   g.move(mv);

   EXPECT_FALSE(g.state().longCastle[Colour::White]);
   EXPECT_FALSE(g.state().shortCastle[Colour::White]);
   EXPECT_FALSE(g.board().occupied(Position::e1));
   EXPECT_FALSE(g.board().occupied(Position::h1));
   EXPECT_TRUE(g.board().occupied(Position::g1));
   EXPECT_EQ(g.board().piece(Position::g1), Piece::King);
   EXPECT_EQ(g.board().colour(Position::g1), Colour::White);
   EXPECT_TRUE(g.board().occupied(Position::f1));
   EXPECT_EQ(g.board().piece(Position::f1), Piece::Rook);
   EXPECT_EQ(g.board().colour(Position::f1), Colour::White);

   g.unmove(mv);

   EXPECT_TRUE(g.state().longCastle[Colour::White]);
   EXPECT_TRUE(g.state().shortCastle[Colour::White]);
   EXPECT_TRUE(g.board().occupied(Position::e1));
   EXPECT_EQ(g.board().piece(Position::e1), Piece::King);
   EXPECT_EQ(g.board().colour(Position::e1), Colour::White);
   EXPECT_TRUE(g.board().occupied(Position::h1));
   EXPECT_EQ(g.board().piece(Position::h1), Piece::Rook);
   EXPECT_EQ(g.board().colour(Position::h1), Colour::White);
}

TEST(GameTest, bshortCastle)
{
   Board b;
   State s;
   b.set(Position::e8, Piece::King, Colour::Black);
   b.set(Position::h8, Piece::Rook, Colour::Black);
   Game g(b, s);

   Move mv;
   mv.setActive(Colour::Black);
   mv.setSrc(Position::e8);
   mv.setDst(Position::g8);
   mv.setPiece(Piece::King);
   mv.setShortCastle(true);

   EXPECT_TRUE(g.state().longCastle[Colour::Black]);
   EXPECT_TRUE(g.state().shortCastle[Colour::Black]);
   EXPECT_TRUE(g.board().occupied(Position::e8));
   EXPECT_EQ(g.board().piece(Position::e8), Piece::King);
   EXPECT_EQ(g.board().colour(Position::e8), Colour::Black);
   EXPECT_TRUE(g.board().occupied(Position::h8));
   EXPECT_EQ(g.board().piece(Position::h8), Piece::Rook);
   EXPECT_EQ(g.board().colour(Position::h8), Colour::Black);

   g.move(mv);

   EXPECT_FALSE(g.state().longCastle[Colour::Black]);
   EXPECT_FALSE(g.state().shortCastle[Colour::Black]);
   EXPECT_FALSE(g.board().occupied(Position::e8));
   EXPECT_FALSE(g.board().occupied(Position::h8));
   EXPECT_TRUE(g.board().occupied(Position::g8));
   EXPECT_EQ(g.board().piece(Position::g8), Piece::King);
   EXPECT_EQ(g.board().colour(Position::g8), Colour::Black);
   EXPECT_TRUE(g.board().occupied(Position::f8));
   EXPECT_EQ(g.board().piece(Position::f8), Piece::Rook);
   EXPECT_EQ(g.board().colour(Position::f8), Colour::Black);

   g.unmove(mv);

   EXPECT_TRUE(g.state().longCastle[Colour::Black]);
   EXPECT_TRUE(g.state().shortCastle[Colour::Black]);
   EXPECT_TRUE(g.board().occupied(Position::e8));
   EXPECT_EQ(g.board().piece(Position::e8), Piece::King);
   EXPECT_EQ(g.board().colour(Position::e8), Colour::Black);
   EXPECT_TRUE(g.board().occupied(Position::h8));
   EXPECT_EQ(g.board().piece(Position::h8), Piece::Rook);
   EXPECT_EQ(g.board().colour(Position::h8), Colour::Black);
}

TEST(GameTest, losingCastlingRights)
{
   Board b;
   State s;
   b.set(Position::e1, Piece::King, Colour::White);
   b.set(Position::a1, Piece::Rook, Colour::White);
   b.set(Position::h1, Piece::Rook, Colour::White);
   b.set(Position::e8, Piece::King, Colour::Black);
   b.set(Position::a8, Piece::Rook, Colour::Black);
   b.set(Position::h8, Piece::Rook, Colour::Black);
   Game g(b, s);

   Move mv1;
   mv1.setActive(Colour::White);
   mv1.setSrc(Position::e1);
   mv1.setDst(Position::e2);
   mv1.setPiece(Piece::King);

   Move mv2;
   mv2.setActive(Colour::Black);
   mv2.setSrc(Position::a8);
   mv2.setDst(Position::a4);
   mv2.setPiece(Piece::Rook);

   Move mv3;
   mv3.setActive(Colour::Black);
   mv3.setSrc(Position::h8);
   mv3.setDst(Position::h4);
   mv3.setPiece(Piece::Rook);

   EXPECT_TRUE(g.state().longCastle[Colour::White]);
   EXPECT_TRUE(g.state().shortCastle[Colour::White]);
   EXPECT_TRUE(g.state().longCastle[Colour::Black]);
   EXPECT_TRUE(g.state().shortCastle[Colour::Black]);

   g.move(mv1);

   EXPECT_FALSE(g.state().longCastle[Colour::White]);
   EXPECT_FALSE(g.state().shortCastle[Colour::White]);
   EXPECT_TRUE(g.state().longCastle[Colour::Black]);
   EXPECT_TRUE(g.state().shortCastle[Colour::Black]);

   g.move(mv2);

   EXPECT_FALSE(g.state().longCastle[Colour::White]);
   EXPECT_FALSE(g.state().shortCastle[Colour::White]);
   EXPECT_FALSE(g.state().longCastle[Colour::Black]);
   EXPECT_TRUE(g.state().shortCastle[Colour::Black]);

   g.move(mv3);

   EXPECT_FALSE(g.state().longCastle[Colour::White]);
   EXPECT_FALSE(g.state().shortCastle[Colour::White]);
   EXPECT_FALSE(g.state().longCastle[Colour::Black]);
   EXPECT_FALSE(g.state().shortCastle[Colour::Black]);

   g.unmove(mv3);

   EXPECT_FALSE(g.state().longCastle[Colour::White]);
   EXPECT_FALSE(g.state().shortCastle[Colour::White]);
   EXPECT_FALSE(g.state().longCastle[Colour::Black]);
   EXPECT_TRUE(g.state().shortCastle[Colour::Black]);

   g.unmove(mv2);

   EXPECT_FALSE(g.state().longCastle[Colour::White]);
   EXPECT_FALSE(g.state().shortCastle[Colour::White]);
   EXPECT_TRUE(g.state().longCastle[Colour::Black]);
   EXPECT_TRUE(g.state().shortCastle[Colour::Black]);

   g.unmove(mv1);

   EXPECT_TRUE(g.state().longCastle[Colour::White]);
   EXPECT_TRUE(g.state().shortCastle[Colour::White]);
   EXPECT_TRUE(g.state().longCastle[Colour::Black]);
   EXPECT_TRUE(g.state().shortCastle[Colour::Black]);
}

TEST(GameTest, activeState)
{
   Board b;
   State s;
   b.set(Position::e2, Piece::Pawn, Colour::White);
   b.set(Position::e7, Piece::Pawn, Colour::Black);
   Game g(b, s);

   Move mv1;
   mv1.setActive(Colour::White);
   mv1.setSrc(Position::e2);
   mv1.setDst(Position::e3);
   mv1.setPiece(Piece::Pawn);

   Move mv2;
   mv2.setActive(Colour::Black);
   mv2.setSrc(Position::e7);
   mv2.setDst(Position::e6);
   mv2.setPiece(Piece::Pawn);

   Move mv3;
   mv3.setActive(Colour::White);
   mv3.setSrc(Position::e3);
   mv3.setDst(Position::e4);
   mv3.setPiece(Piece::Pawn);

   EXPECT_EQ(g.state().active, Colour::White);
   g.move(mv1);
   EXPECT_EQ(g.state().active, Colour::Black);
   g.move(mv2);
   EXPECT_EQ(g.state().active, Colour::White);
   g.move(mv3);
   EXPECT_EQ(g.state().active, Colour::Black);
   g.unmove(mv3);
   EXPECT_EQ(g.state().active, Colour::White);
   g.unmove(mv2);
   EXPECT_EQ(g.state().active, Colour::Black);
   g.unmove(mv1);
   EXPECT_EQ(g.state().active, Colour::White);
}

TEST(GameTest, fullMoveClock)
{
   Board b;
   State s;
   b.set(Position::e2, Piece::Pawn, Colour::White);
   b.set(Position::e7, Piece::Pawn, Colour::Black);
   Game g(b, s);

   Move mv1;
   mv1.setActive(Colour::White);
   mv1.setSrc(Position::e2);
   mv1.setDst(Position::e3);
   mv1.setPiece(Piece::Pawn);

   Move mv2;
   mv2.setActive(Colour::Black);
   mv2.setSrc(Position::e7);
   mv2.setDst(Position::e6);
   mv2.setPiece(Piece::Pawn);

   Move mv3;
   mv3.setActive(Colour::White);
   mv3.setSrc(Position::e3);
   mv3.setDst(Position::e4);
   mv3.setPiece(Piece::Pawn);

   Move mv4;
   mv4.setActive(Colour::Black);
   mv4.setSrc(Position::e6);
   mv4.setDst(Position::e5);
   mv4.setPiece(Piece::Pawn);

   EXPECT_EQ(g.state().fullMoveClock, 1);
   g.move(mv1);
   EXPECT_EQ(g.state().fullMoveClock, 1);
   g.move(mv2);
   EXPECT_EQ(g.state().fullMoveClock, 2);
   g.move(mv3);
   EXPECT_EQ(g.state().fullMoveClock, 2);
   g.move(mv4);
   EXPECT_EQ(g.state().fullMoveClock, 3);
   g.unmove(mv4);
   EXPECT_EQ(g.state().fullMoveClock, 2);
   g.unmove(mv3);
   EXPECT_EQ(g.state().fullMoveClock, 2);
   g.unmove(mv2);
   EXPECT_EQ(g.state().fullMoveClock, 1);
   g.unmove(mv1);
   EXPECT_EQ(g.state().fullMoveClock, 1);
}

TEST(GameTest, halfMoveClock)
{
   Board b;
   State s;
   b.set(Position::e2, Piece::Pawn, Colour::White);
   b.set(Position::d1, Piece::Queen, Colour::White);
   b.set(Position::e7, Piece::Pawn, Colour::Black);
   b.set(Position::d8, Piece::Queen, Colour::Black);
   Game g(b, s);

   Move wqueen1;
   wqueen1.setActive(Colour::White);
   wqueen1.setSrc(Position::d1);
   wqueen1.setDst(Position::d2);
   wqueen1.setPiece(Piece::Queen);

   Move wqueen2;
   wqueen1.setActive(Colour::White);
   wqueen1.setSrc(Position::d2);
   wqueen1.setDst(Position::d1);
   wqueen1.setPiece(Piece::Queen);

   Move wqueencap;
   wqueencap.setActive(Colour::White);
   wqueencap.setSrc(Position::d1);
   wqueencap.setDst(Position::d8);
   wqueencap.setPiece(Piece::Queen);
   wqueencap.setCapturedPiece(Piece::Queen);

   Move bqueen1;
   bqueen1.setActive(Colour::Black);
   bqueen1.setSrc(Position::d8);
   bqueen1.setDst(Position::d7);
   bqueen1.setPiece(Piece::Queen);

   Move bqueen2;
   bqueen1.setActive(Colour::Black);
   bqueen1.setSrc(Position::d7);
   bqueen1.setDst(Position::d8);
   bqueen1.setPiece(Piece::Queen);

   Move wpawn;
   wpawn.setActive(Colour::White);
   wpawn.setSrc(Position::e3);
   wpawn.setDst(Position::e4);
   wpawn.setPiece(Piece::Pawn);

   Move bpawn;
   bpawn.setActive(Colour::Black);
   bpawn.setSrc(Position::e6);
   bpawn.setDst(Position::e5);
   bpawn.setPiece(Piece::Pawn);

   EXPECT_EQ(g.state().halfMoveClock, 0);
   g.move(wqueen1);
   EXPECT_EQ(g.state().halfMoveClock, 1);
   g.move(bqueen1);
   EXPECT_EQ(g.state().halfMoveClock, 2);
   g.move(wqueen2);
   EXPECT_EQ(g.state().halfMoveClock, 3);
   g.move(bqueen2);
   EXPECT_EQ(g.state().halfMoveClock, 4);
   g.move(wpawn);
   EXPECT_EQ(g.state().halfMoveClock, 0);
   g.unmove(wpawn);
   EXPECT_EQ(g.state().halfMoveClock, 4);
   g.move(wqueencap);
   EXPECT_EQ(g.state().halfMoveClock, 0);
   g.unmove(wqueencap);
   EXPECT_EQ(g.state().halfMoveClock, 4);
   g.unmove(bqueen2);
   EXPECT_EQ(g.state().halfMoveClock, 3);
   g.unmove(wqueen2);
   EXPECT_EQ(g.state().halfMoveClock, 2);
   g.unmove(bqueen1);
   EXPECT_EQ(g.state().halfMoveClock, 1);
   g.unmove(wqueen1);
   EXPECT_EQ(g.state().halfMoveClock, 0);
}

TEST(GameTest, fen1)
{
   static constexpr std::string_view fen{
      "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"};
   auto g = Game::fromFEN(fen);
   ASSERT_TRUE(g.has_value());

   EXPECT_OCCUPIED(g->board(), Position::a1, Colour::White, Piece::Rook);
   EXPECT_OCCUPIED(g->board(), Position::b1, Colour::White, Piece::Knight);
   EXPECT_OCCUPIED(g->board(), Position::c1, Colour::White, Piece::Bishop);
   EXPECT_OCCUPIED(g->board(), Position::d1, Colour::White, Piece::Queen);
   EXPECT_OCCUPIED(g->board(), Position::e1, Colour::White, Piece::King);
   EXPECT_OCCUPIED(g->board(), Position::f1, Colour::White, Piece::Bishop);
   EXPECT_OCCUPIED(g->board(), Position::g1, Colour::White, Piece::Knight);
   EXPECT_OCCUPIED(g->board(), Position::h1, Colour::White, Piece::Rook);
   EXPECT_OCCUPIED(g->board(), Position::a2, Colour::White, Piece::Pawn);
   EXPECT_OCCUPIED(g->board(), Position::b2, Colour::White, Piece::Pawn);
   EXPECT_OCCUPIED(g->board(), Position::c2, Colour::White, Piece::Pawn);
   EXPECT_OCCUPIED(g->board(), Position::d2, Colour::White, Piece::Pawn);
   EXPECT_OCCUPIED(g->board(), Position::e2, Colour::White, Piece::Pawn);
   EXPECT_OCCUPIED(g->board(), Position::f2, Colour::White, Piece::Pawn);
   EXPECT_OCCUPIED(g->board(), Position::g2, Colour::White, Piece::Pawn);
   EXPECT_OCCUPIED(g->board(), Position::h2, Colour::White, Piece::Pawn);

   EXPECT_EMPTY(g->board(), Position::a3);
   EXPECT_EMPTY(g->board(), Position::b3);
   EXPECT_EMPTY(g->board(), Position::c3);
   EXPECT_EMPTY(g->board(), Position::d3);
   EXPECT_EMPTY(g->board(), Position::e3);
   EXPECT_EMPTY(g->board(), Position::f3);
   EXPECT_EMPTY(g->board(), Position::g3);
   EXPECT_EMPTY(g->board(), Position::h3);
   EXPECT_EMPTY(g->board(), Position::a4);
   EXPECT_EMPTY(g->board(), Position::b4);
   EXPECT_EMPTY(g->board(), Position::c4);
   EXPECT_EMPTY(g->board(), Position::d4);
   EXPECT_EMPTY(g->board(), Position::e4);
   EXPECT_EMPTY(g->board(), Position::f4);
   EXPECT_EMPTY(g->board(), Position::g4);
   EXPECT_EMPTY(g->board(), Position::h4);
   EXPECT_EMPTY(g->board(), Position::a5);
   EXPECT_EMPTY(g->board(), Position::b5);
   EXPECT_EMPTY(g->board(), Position::c5);
   EXPECT_EMPTY(g->board(), Position::d5);
   EXPECT_EMPTY(g->board(), Position::e5);
   EXPECT_EMPTY(g->board(), Position::f5);
   EXPECT_EMPTY(g->board(), Position::g5);
   EXPECT_EMPTY(g->board(), Position::h5);
   EXPECT_EMPTY(g->board(), Position::a6);
   EXPECT_EMPTY(g->board(), Position::b6);
   EXPECT_EMPTY(g->board(), Position::c6);
   EXPECT_EMPTY(g->board(), Position::d6);
   EXPECT_EMPTY(g->board(), Position::e6);
   EXPECT_EMPTY(g->board(), Position::f6);
   EXPECT_EMPTY(g->board(), Position::g6);
   EXPECT_EMPTY(g->board(), Position::h6);

   EXPECT_OCCUPIED(g->board(), Position::a7, Colour::Black, Piece::Pawn);
   EXPECT_OCCUPIED(g->board(), Position::b7, Colour::Black, Piece::Pawn);
   EXPECT_OCCUPIED(g->board(), Position::c7, Colour::Black, Piece::Pawn);
   EXPECT_OCCUPIED(g->board(), Position::d7, Colour::Black, Piece::Pawn);
   EXPECT_OCCUPIED(g->board(), Position::e7, Colour::Black, Piece::Pawn);
   EXPECT_OCCUPIED(g->board(), Position::f7, Colour::Black, Piece::Pawn);
   EXPECT_OCCUPIED(g->board(), Position::g7, Colour::Black, Piece::Pawn);
   EXPECT_OCCUPIED(g->board(), Position::h7, Colour::Black, Piece::Pawn);
   EXPECT_OCCUPIED(g->board(), Position::a8, Colour::Black, Piece::Rook);
   EXPECT_OCCUPIED(g->board(), Position::b8, Colour::Black, Piece::Knight);
   EXPECT_OCCUPIED(g->board(), Position::c8, Colour::Black, Piece::Bishop);
   EXPECT_OCCUPIED(g->board(), Position::d8, Colour::Black, Piece::Queen);
   EXPECT_OCCUPIED(g->board(), Position::e8, Colour::Black, Piece::King);
   EXPECT_OCCUPIED(g->board(), Position::f8, Colour::Black, Piece::Bishop);
   EXPECT_OCCUPIED(g->board(), Position::g8, Colour::Black, Piece::Knight);
   EXPECT_OCCUPIED(g->board(), Position::h8, Colour::Black, Piece::Rook);

   EXPECT_EQ(g->state().active, Colour::White);
   EXPECT_TRUE(g->state().longCastle[Colour::White]);
   EXPECT_TRUE(g->state().longCastle[Colour::Black]);
   EXPECT_TRUE(g->state().shortCastle[Colour::White]);
   EXPECT_TRUE(g->state().shortCastle[Colour::Black]);
   EXPECT_EQ(g->state().enPassant, Position::Invalid);
   EXPECT_EQ(g->state().halfMoveClock, 0);
   EXPECT_EQ(g->state().fullMoveClock, 1);

   EXPECT_EQ(g->toFEN(), fen);
}

TEST(GameTest, fen2)
{
   static constexpr std::string_view fen{
      "rnbqk1B1/p2p3p/5p1n/2p1N1p1/1pP1P1Q1/R2P1P2/1P4PP/1NB1K2R b Kq c3 0 11"};
   auto g = Game::fromFEN(fen);
   ASSERT_TRUE(g.has_value());

   EXPECT_EMPTY(g->board(), Position::a1);
   EXPECT_OCCUPIED(g->board(), Position::b1, Colour::White, Piece::Knight);
   EXPECT_OCCUPIED(g->board(), Position::c1, Colour::White, Piece::Bishop);
   EXPECT_EMPTY(g->board(), Position::d1);
   EXPECT_OCCUPIED(g->board(), Position::e1, Colour::White, Piece::King);
   EXPECT_EMPTY(g->board(), Position::f1);
   EXPECT_EMPTY(g->board(), Position::g1);
   EXPECT_OCCUPIED(g->board(), Position::h1, Colour::White, Piece::Rook);

   EXPECT_EMPTY(g->board(), Position::a2);
   EXPECT_OCCUPIED(g->board(), Position::b2, Colour::White, Piece::Pawn);
   EXPECT_EMPTY(g->board(), Position::c2);
   EXPECT_EMPTY(g->board(), Position::d2);
   EXPECT_EMPTY(g->board(), Position::e2);
   EXPECT_EMPTY(g->board(), Position::f2);
   EXPECT_OCCUPIED(g->board(), Position::g2, Colour::White, Piece::Pawn);
   EXPECT_OCCUPIED(g->board(), Position::h2, Colour::White, Piece::Pawn);

   EXPECT_OCCUPIED(g->board(), Position::a3, Colour::White, Piece::Rook);
   EXPECT_EMPTY(g->board(), Position::b3);
   EXPECT_EMPTY(g->board(), Position::c3);
   EXPECT_OCCUPIED(g->board(), Position::d3, Colour::White, Piece::Pawn);
   EXPECT_EMPTY(g->board(), Position::e3);
   EXPECT_OCCUPIED(g->board(), Position::f3, Colour::White, Piece::Pawn);
   EXPECT_EMPTY(g->board(), Position::g3);
   EXPECT_EMPTY(g->board(), Position::h3);

   EXPECT_EMPTY(g->board(), Position::a4);
   EXPECT_OCCUPIED(g->board(), Position::b4, Colour::Black, Piece::Pawn);
   EXPECT_OCCUPIED(g->board(), Position::c4, Colour::White, Piece::Pawn);
   EXPECT_EMPTY(g->board(), Position::d4);
   EXPECT_OCCUPIED(g->board(), Position::e4, Colour::White, Piece::Pawn);
   EXPECT_EMPTY(g->board(), Position::f4);
   EXPECT_OCCUPIED(g->board(), Position::g4, Colour::White, Piece::Queen);
   EXPECT_EMPTY(g->board(), Position::h4);

   EXPECT_EMPTY(g->board(), Position::a5);
   EXPECT_EMPTY(g->board(), Position::b5);
   EXPECT_OCCUPIED(g->board(), Position::c5, Colour::Black, Piece::Pawn);
   EXPECT_EMPTY(g->board(), Position::d5);
   EXPECT_OCCUPIED(g->board(), Position::e5, Colour::White, Piece::Knight);
   EXPECT_EMPTY(g->board(), Position::f5);
   EXPECT_OCCUPIED(g->board(), Position::g5, Colour::Black, Piece::Pawn);
   EXPECT_EMPTY(g->board(), Position::h5);

   EXPECT_EMPTY(g->board(), Position::a6);
   EXPECT_EMPTY(g->board(), Position::b6);
   EXPECT_EMPTY(g->board(), Position::c6);
   EXPECT_EMPTY(g->board(), Position::d6);
   EXPECT_EMPTY(g->board(), Position::e6);
   EXPECT_OCCUPIED(g->board(), Position::f6, Colour::Black, Piece::Pawn);
   EXPECT_EMPTY(g->board(), Position::g6);
   EXPECT_OCCUPIED(g->board(), Position::h6, Colour::Black, Piece::Knight);

   EXPECT_OCCUPIED(g->board(), Position::a7, Colour::Black, Piece::Pawn);
   EXPECT_EMPTY(g->board(), Position::b7);
   EXPECT_EMPTY(g->board(), Position::c7);
   EXPECT_OCCUPIED(g->board(), Position::d7, Colour::Black, Piece::Pawn);
   EXPECT_EMPTY(g->board(), Position::e7);
   EXPECT_EMPTY(g->board(), Position::f7);
   EXPECT_EMPTY(g->board(), Position::g7);
   EXPECT_OCCUPIED(g->board(), Position::h7, Colour::Black, Piece::Pawn);

   EXPECT_OCCUPIED(g->board(), Position::a8, Colour::Black, Piece::Rook);
   EXPECT_OCCUPIED(g->board(), Position::b8, Colour::Black, Piece::Knight);
   EXPECT_OCCUPIED(g->board(), Position::c8, Colour::Black, Piece::Bishop);
   EXPECT_OCCUPIED(g->board(), Position::d8, Colour::Black, Piece::Queen);
   EXPECT_OCCUPIED(g->board(), Position::e8, Colour::Black, Piece::King);
   EXPECT_EMPTY(g->board(), Position::f8);
   EXPECT_OCCUPIED(g->board(), Position::g8, Colour::White, Piece::Bishop);
   EXPECT_EMPTY(g->board(), Position::h8);

   EXPECT_EQ(g->state().active, Colour::Black);
   EXPECT_FALSE(g->state().longCastle[Colour::White]);
   EXPECT_TRUE(g->state().longCastle[Colour::Black]);
   EXPECT_TRUE(g->state().shortCastle[Colour::White]);
   EXPECT_FALSE(g->state().shortCastle[Colour::Black]);
   EXPECT_EQ(g->state().enPassant, Position::c3);
   EXPECT_EQ(g->state().halfMoveClock, 0);
   EXPECT_EQ(g->state().fullMoveClock, 11);

   EXPECT_EQ(g->toFEN(), fen);
}

TEST(GameTest, fen3)
{
   static constexpr std::string_view fen{
      "4r1k1/7p/7B/8/1p6/3p4/1P1N2PP/5RK1 b - - 7 32"};
   auto g = Game::fromFEN(fen);
   ASSERT_TRUE(g.has_value());

   EXPECT_EMPTY(g->board(), Position::a1);
   EXPECT_EMPTY(g->board(), Position::b1);
   EXPECT_EMPTY(g->board(), Position::c1);
   EXPECT_EMPTY(g->board(), Position::d1);
   EXPECT_EMPTY(g->board(), Position::e1);
   EXPECT_OCCUPIED(g->board(), Position::f1, Colour::White, Piece::Rook);
   EXPECT_OCCUPIED(g->board(), Position::g1, Colour::White, Piece::King);
   EXPECT_EMPTY(g->board(), Position::h1);

   EXPECT_EMPTY(g->board(), Position::a2);
   EXPECT_OCCUPIED(g->board(), Position::b2, Colour::White, Piece::Pawn);
   EXPECT_EMPTY(g->board(), Position::c2);
   EXPECT_OCCUPIED(g->board(), Position::d2, Colour::White, Piece::Knight);
   EXPECT_EMPTY(g->board(), Position::e2);
   EXPECT_EMPTY(g->board(), Position::f2);
   EXPECT_OCCUPIED(g->board(), Position::g2, Colour::White, Piece::Pawn);
   EXPECT_OCCUPIED(g->board(), Position::h2, Colour::White, Piece::Pawn);

   EXPECT_EMPTY(g->board(), Position::a3);
   EXPECT_EMPTY(g->board(), Position::b3);
   EXPECT_EMPTY(g->board(), Position::c3);
   EXPECT_OCCUPIED(g->board(), Position::d3, Colour::Black, Piece::Pawn);
   EXPECT_EMPTY(g->board(), Position::e3);
   EXPECT_EMPTY(g->board(), Position::f3);
   EXPECT_EMPTY(g->board(), Position::g3);
   EXPECT_EMPTY(g->board(), Position::h3);

   EXPECT_EMPTY(g->board(), Position::a4);
   EXPECT_OCCUPIED(g->board(), Position::b4, Colour::Black, Piece::Pawn);
   EXPECT_EMPTY(g->board(), Position::c4);
   EXPECT_EMPTY(g->board(), Position::d4);
   EXPECT_EMPTY(g->board(), Position::e4);
   EXPECT_EMPTY(g->board(), Position::f4);
   EXPECT_EMPTY(g->board(), Position::g4);
   EXPECT_EMPTY(g->board(), Position::h4);

   EXPECT_EMPTY(g->board(), Position::a5);
   EXPECT_EMPTY(g->board(), Position::b5);
   EXPECT_EMPTY(g->board(), Position::c5);
   EXPECT_EMPTY(g->board(), Position::d5);
   EXPECT_EMPTY(g->board(), Position::e5);
   EXPECT_EMPTY(g->board(), Position::f5);
   EXPECT_EMPTY(g->board(), Position::g5);
   EXPECT_EMPTY(g->board(), Position::h5);

   EXPECT_EMPTY(g->board(), Position::a6);
   EXPECT_EMPTY(g->board(), Position::b6);
   EXPECT_EMPTY(g->board(), Position::c6);
   EXPECT_EMPTY(g->board(), Position::d6);
   EXPECT_EMPTY(g->board(), Position::e6);
   EXPECT_EMPTY(g->board(), Position::f6);
   EXPECT_EMPTY(g->board(), Position::g6);
   EXPECT_OCCUPIED(g->board(), Position::h6, Colour::White, Piece::Bishop);

   EXPECT_EMPTY(g->board(), Position::a7);
   EXPECT_EMPTY(g->board(), Position::b7);
   EXPECT_EMPTY(g->board(), Position::c7);
   EXPECT_EMPTY(g->board(), Position::d7);
   EXPECT_EMPTY(g->board(), Position::e7);
   EXPECT_EMPTY(g->board(), Position::f7);
   EXPECT_EMPTY(g->board(), Position::g7);
   EXPECT_OCCUPIED(g->board(), Position::h7, Colour::Black, Piece::Pawn);

   EXPECT_EMPTY(g->board(), Position::a8);
   EXPECT_EMPTY(g->board(), Position::b8);
   EXPECT_EMPTY(g->board(), Position::c8);
   EXPECT_EMPTY(g->board(), Position::d8);
   EXPECT_OCCUPIED(g->board(), Position::e8, Colour::Black, Piece::Rook);
   EXPECT_EMPTY(g->board(), Position::f8);
   EXPECT_OCCUPIED(g->board(), Position::g8, Colour::Black, Piece::King);
   EXPECT_EMPTY(g->board(), Position::h8);

   EXPECT_EQ(g->state().active, Colour::Black);
   EXPECT_FALSE(g->state().longCastle[Colour::White]);
   EXPECT_FALSE(g->state().longCastle[Colour::Black]);
   EXPECT_FALSE(g->state().shortCastle[Colour::White]);
   EXPECT_FALSE(g->state().shortCastle[Colour::Black]);
   EXPECT_EQ(g->state().enPassant, Position::Invalid);
   EXPECT_EQ(g->state().halfMoveClock, 7);
   EXPECT_EQ(g->state().fullMoveClock, 32);

   EXPECT_EQ(g->toFEN(), fen);
}
