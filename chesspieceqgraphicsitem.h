#ifndef CHESSPIECEQGRAPHICSITEM_H
#define CHESSPIECEQGRAPHICSITEM_H

#include <QGraphicsItem>

class ChessWidget;

class ChessPieceQGraphicsItem : public QGraphicsItem
{
public:
    ChessPieceQGraphicsItem(ChessWidget *cw);
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    ChessWidget *chessWidget;
};

#endif // CHESSPIECEQGRAPHICSITEM_H
