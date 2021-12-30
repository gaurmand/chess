#ifndef CHESSPIECEITEM_H
#define CHESSPIECEITEM_H

#include "chess/game.h"

#include <QGraphicsObject>

class ChessPieceItem : public QGraphicsObject
{
    Q_OBJECT
public:
    ChessPieceItem(const Chess::Piece* piece);
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;

    // Enable the use of qgraphicsitem_cast with this item.
    enum { Type = UserType + 2 };
    int type() const override{ return Type; }

    const Chess::Piece* chessPiece() const { return piece_; }

public slots:
    void updatePos();

signals:
    void mouseRelease(QGraphicsSceneMouseEvent* event);
    void mousePress(const ChessPieceItem* pieceItem);
    void mouseMove(const Chess::BP& pos);

private:
    const Chess::Piece* piece_;
};

#endif // CHESSPIECEITEM_H
