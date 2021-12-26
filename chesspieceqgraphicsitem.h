#ifndef CHESSPIECEQGRAPHICSITEM_H
#define CHESSPIECEQGRAPHICSITEM_H

#define CHESSPIECE_QGRAPHICSITEM_TYPE 65538

#include <QGraphicsItem>
#include "chess/piece.h"

class ChessWidget;

class ChessPieceQGraphicsItem : public QGraphicsItem
{
public:
    ChessPieceQGraphicsItem(ChessWidget *cw, const Chess::Piece& piece);
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
    Chess::Piece getPiece() {return piece_;}

private:
    ChessWidget* chessWidget_;
    const Chess::Piece& piece_;
    QPixmap* piecePixmap_;
};

#endif // CHESSPIECEQGRAPHICSITEM_H
