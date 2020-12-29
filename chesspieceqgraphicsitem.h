#ifndef CHESSPIECEQGRAPHICSITEM_H
#define CHESSPIECEQGRAPHICSITEM_H

#include <QGraphicsItem>
#include  "constants.h"

class ChessWidget;
class ChessPiece;

class ChessPieceQGraphicsItem : public QGraphicsItem
{
public:
    ChessPieceQGraphicsItem(ChessWidget *cw, ChessPiece* piece);
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
//    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) override;
//    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override

    // Enable the use of qgraphicsitem_cast with this item.
    enum { Type = CHESSPIECE_QGRAPHICSITEM_TYPE };
    int type() const override{ return Type; }

    void setBoardPosition();
    void setPixmap();
    ChessPiece* getPiece() {return piece;}

private:
    ChessWidget *chessWidget;
    ChessPiece* piece;
    QPixmap *piecePixmap;
};

#endif // CHESSPIECEQGRAPHICSITEM_H
