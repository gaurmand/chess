#ifndef CHESSBOARDITEM_H
#define CHESSBOARDITEM_H

#include "chess/game.h"
#include "chesspieceitem.h"

#include <QGraphicsItem>

enum class SquareState{NONE, SOURCE, NORMAL_MOVE, CAPTURE, CHECK};

class ChessBoardItem : public QGraphicsItem
{
public:
    ChessBoardItem(const Chess::Game& game);
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    void drawSquare(int i, int j, QPainter *painter);
    void drawBoard(QPainter *painter);

    // Enable the use of qgraphicsitem_cast
    enum { Type = UserType + 1 };
    int type() const override{ return Type; }

public slots:
    void select(const Chess::BP& src, const Chess::Game& game);
    void deselect();
    void updatePieces();
    void setPiecesMovable(Chess::Player player);
    void setPiecesMovable(bool movable);

private:
    std::array<std::array<SquareState, NUM_COLS>, NUM_ROWS> states_;
    std::array<std::array<ChessPieceItem, NUM_CHESS_PIECES>, NUM_PLAYERS> pieces_;
};

#endif // CHESSBOARDITEM_H
