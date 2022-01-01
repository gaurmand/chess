#ifndef CHESSPIECEITEM_H
#define CHESSPIECEITEM_H

#include "chess/game.h"

#include <QGraphicsPixmapItem>

class ChessPieceItem : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
    Q_PROPERTY(QPointF pos READ pos WRITE setPos FINAL)
public:
    ChessPieceItem(const Chess::Piece* piece);

    // Enable the use of qgraphicsitem_cast with this item.
    enum { Type = UserType + 2 };
    int type() const override{ return Type; }

    const Chess::Piece* chessPiece() const { return piece_; }

public slots:
    void updatePosition();
    void updateVisual();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

private:
    const Chess::Piece* piece_;
    Chess::PieceType type_;
};

#endif // CHESSPIECEITEM_H
