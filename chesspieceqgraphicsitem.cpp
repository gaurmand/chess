#include <QPainter>
#include <QGraphicsSceneEvent>
#include <iostream>
#include "chesswidget.h"
#include "chesspieceqgraphicsitem.h"
#include "chesspiece.h"

ChessPieceQGraphicsItem::ChessPieceQGraphicsItem(ChessWidget *cw,  ChessPiece* pc)
    : chessWidget(cw), piece(pc)
{
    setZValue(0);
    setFlag(ItemIsMovable);
    setBoardPosition();
    setPixmap();
}

QRectF ChessPieceQGraphicsItem::boundingRect() const
{
    return QRectF(0, 0, SQUARE_WIDTH, SQUARE_WIDTH);
}

void ChessPieceQGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    painter->drawPixmap(QRect(0, 0, SQUARE_WIDTH, SQUARE_WIDTH), *piecePixmap);
}

void ChessPieceQGraphicsItem::setBoardPosition()
{
    IBP pos = piece->getIBPos();
    setPos(pos.col*SQUARE_WIDTH, pos.row*SQUARE_WIDTH);
}

void ChessPieceQGraphicsItem::setPixmap(){
    piecePixmap = chessWidget->getPiecePixmap(piece->getType(), piece->getOwner());
}

void ChessPieceQGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    setZValue(1);
    chessWidget->chessPieceItemMousePress(piece);

    QGraphicsItem::mousePressEvent(event);
}

void ChessPieceQGraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    setZValue(0);
    chessWidget->chessPieceItemMouseRelease(piece, event->scenePos());
    setBoardPosition();
    update();

    QGraphicsItem::mouseReleaseEvent(event);
}

//void ChessPieceQGraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
//{
//    std::cout << piece->toString() << " mouse move" << std::endl;
//    QGraphicsItem::mouseMoveEvent(event);
//}

//void ChessPieceQGraphicsItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)
//{
//    std::cout << piece->toString() << " mouse dbl click" << std::endl;
//    QGraphicsItem::mouseDoubleClickEvent(event);
//}


