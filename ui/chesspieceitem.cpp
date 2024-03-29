#include "chesspieceitem.h"
#include "ui.h"

#include <QGraphicsSceneEvent>
#include <QApplication>

#include <iostream>

ChessPieceItem::ChessPieceItem(const Chess::Piece* piece): piece_(piece)
{
    setZValue(0);
    setAcceptedMouseButtons(Qt::LeftButton);
    setShapeMode(ShapeMode::BoundingRectShape);
    type_ = piece_->type();
    setPixmap(ui::piecePixmap(piece_->owner(), piece_->type()));
}

void ChessPieceItem::updatePosition()
{
    if (!isVisible())
    {
        return;
    }

    const QPointF expectedPos = ui::BPToScene(piece_->pos());
    if (pos() != expectedPos)
    {
        setPos(expectedPos);
    }
}

void ChessPieceItem::updateVisual()
{
    setVisible(!piece_->isCaptured());
    if (!isVisible())
    {
        return;
    }

    if (type_ != piece_->type())
    {
        type_ = piece_->type();
        setPixmap(ui::piecePixmap(piece_->owner(), piece_->type()));
    }
}

void ChessPieceItem::resizePixmap()
{
    setPixmap(ui::piecePixmap(piece_->owner(), piece_->type()));
}

void ChessPieceItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if (flags() & QGraphicsItem::ItemIsMovable)
    {
        setZValue(1);
        setCursor(Qt::ClosedHandCursor);
    }
    QGraphicsItem::mousePressEvent(event);
}

void ChessPieceItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    if (flags() & QGraphicsItem::ItemIsMovable)
    {
        setZValue(0);
        setCursor(Qt::OpenHandCursor);
    }
    QGraphicsItem::mouseReleaseEvent(event);
}
