#ifndef CHESSPIECEITEM_H
#define CHESSPIECEITEM_H

#include "chess/game.h"

#include <QGraphicsObject>

class ChessPieceItem : public QGraphicsObject
{
    Q_OBJECT
public:
    ChessPieceItem();
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;

    void setChessPiece(const Chess::Piece* piece) { piece_ = piece; }
    const Chess::Piece* chessPiece() const {return piece_; }

    // Enable the use of qgraphicsitem_cast with this item.
    enum { Type = UserType + 2 };
    int type() const override{ return Type; }

public slots:
    void updatePos();

signals:
    void mouseRelease(QGraphicsSceneMouseEvent* event);
    void mousePress(const ChessPieceItem* pieceItem);
    void mouseMove(const Chess::BP& pos);

private:
    const Chess::Piece* piece_ = nullptr;
};

#endif // CHESSPIECEITEM_H
