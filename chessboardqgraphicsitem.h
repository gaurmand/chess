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

private:
    ChessWidget* chessWidget;
};

#endif // CHESSBOARDQGRAPHICSITEM_H
