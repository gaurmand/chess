#ifndef MOVE_VERIFY_H
#define MOVE_VERIFY_H

#define EXPECT_MOVE(mv, c, s, d, p)                                            \
   EXPECT_EQ(mv.active(), c);                                                  \
   EXPECT_EQ(mv.src(), s);                                                     \
   EXPECT_EQ(mv.dst(), d);                                                     \
   EXPECT_EQ(mv.piece(), p);

#define EXPECT_CAPTURE(mv, c, s, d, p, cap)                                    \
   EXPECT_MOVE(mv, c, s, d, p)                                                 \
   EXPECT_TRUE(mv.capture());                                                  \
   EXPECT_EQ(mv.capturedPiece(), cap);

#define EXPECT_PROMOTION(mv, c, s, d, prom)                                    \
   EXPECT_MOVE(mv, c, s, d, Piece::Pawn)                                       \
   EXPECT_TRUE(mv.promotion());                                                \
   EXPECT_EQ(mv.promotedPiece(), prom);

#define EXPECT_CAPTURE_PROMOTION(mv, c, s, d, cap, prom)                       \
   EXPECT_CAPTURE(mv, c, s, d, Piece::Pawn, cap)                               \
   EXPECT_TRUE(mv.promotion());                                                \
   EXPECT_EQ(mv.promotedPiece(), prom);

#define EXPECT_ENPASSANT(mv, c, s, d)                                          \
   EXPECT_CAPTURE(mv, c, s, d, Piece::Pawn, Piece::Pawn)                       \
   EXPECT_TRUE(mv.enPassant());

#define EXPECT_LONG_CASTLE(mv, c, s, d)                                        \
   EXPECT_MOVE(mv, c, s, d, Piece::King)                                       \
   EXPECT_TRUE(mv.longCastle());

#define EXPECT_SHORT_CASTLE(mv, c, s, d)                                       \
   EXPECT_MOVE(mv, c, s, d, Piece::King)                                       \
   EXPECT_TRUE(mv.shortCastle());

#define EXPECT_OCCUPIED(board, pos, c, p)                                      \
   EXPECT_TRUE(board.occupied(pos));                                           \
   EXPECT_EQ(board.colour(pos).value(), c);                                    \
   EXPECT_EQ(board.piece(pos).value(), p);

#define EXPECT_EMPTY(board, pos) EXPECT_FALSE(board.occupied(pos));

#endif
