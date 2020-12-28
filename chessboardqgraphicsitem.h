#ifndef CHESSBOARDQGRAPHICSITEM_H
#define CHESSBOARDQGRAPHICSITEM_H

#include <QGraphicsItem>
#include "constants.h"

class ChessWidget;

class ChessBoardQGraphicsItem : public QGraphicsItem
{
public:
    ChessBoardQGraphicsItem(ChessWidget *cw);
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    void drawNormalSquare(int i, int j, QPainter *painter);
    void drawSquareState(int i, int j, QPainter *painter);
    void drawCircle(int radius, QColor color, QPainter *painter);
    void drawBoard(QPainter *painter);

private:
    ChessWidget* chessWidget;
    QColor white;
    QColor black;
    QRect squareRect;
};

#endif // CHESSBOARDQGRAPHICSITEM_H
