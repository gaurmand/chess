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
    void setBoardPosition();
    void setPixmap();
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
//    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) override;
//    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;

private:
    ChessWidget *chessWidget;
    ChessPiece* piece;
    QPixmap *piecePixmap;
};

#endif // CHESSPIECEQGRAPHICSITEM_H
