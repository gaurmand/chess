#ifndef CHESSBOARDQGRAPHICSITEM_H
#define CHESSBOARDQGRAPHICSITEM_H

#include <QGraphicsItem>

class ChessWidget;

class ChessBoardQGraphicsItem : public QGraphicsItem
{
public:
    ChessBoardQGraphicsItem(ChessWidget *cw);
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    void drawSquare(int i, int j, QPainter *painter, QColor color);
    void drawBoard(QPainter *painter);

private:
    ChessWidget* chessWidget;
    QColor white;
    QColor black;
    QRect squareRect;
};

#endif // CHESSBOARDQGRAPHICSITEM_H
