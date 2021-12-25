#ifndef CHESSBOARDQGRAPHICSITEM_H
#define CHESSBOARDQGRAPHICSITEM_H

#define CHESSBOARD_QGRAPHICSITEM_TYPE 65537

#include <QGraphicsItem>
#include "chess/boardpos.h"

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

    void drawNormalSquare(int i, int j, QPainter *painter);
    void drawSquareState(int i, int j, QPainter *painter);
    void drawCircle(int radius, QColor color, QPainter *painter);
    void drawBoard(QPainter *painter);
    void setCheck(Chess::BP pos);
    void clearCheck();

private:
    ChessWidget* chessWidget_;
    bool isInCheck_;
    Chess::BP checkPos_;
};

#endif // CHESSBOARDQGRAPHICSITEM_H
