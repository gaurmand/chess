#ifndef CHESSBOARDQGRAPHICSITEM_H
#define CHESSBOARDQGRAPHICSITEM_H

#define CHESSBOARD_QGRAPHICSITEM_TYPE 65537

#include <QGraphicsItem>
#include "chess/game.h"

enum class SquareState{NONE, SOURCE, NORMAL_MOVE, CAPTURE, CHECK};

class ChessWidget;

class ChessBoardQGraphicsItem : public QGraphicsItem
{
public:
    ChessBoardQGraphicsItem(ChessWidget *cw);
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    // Enable the use of qgraphicsitem_cast with this item.
    enum { Type = CHESSBOARD_QGRAPHICSITEM_TYPE };
    int type() const override{ return Type; }

    void drawSquare(int i, int j, QPainter *painter);
    void drawBoard(QPainter *painter);

public slots:
    void select(const Chess::BP& src, const Chess::Game& game);
    void deselect();

private:
    ChessWidget* chessWidget_;

    std::array<std::array<SquareState, NUM_COLS>, NUM_ROWS> states_;
};

#endif // CHESSBOARDQGRAPHICSITEM_H
