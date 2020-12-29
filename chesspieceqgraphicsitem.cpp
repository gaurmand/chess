#include <QPainter>
#include <QGraphicsSceneEvent>
#include <iostream>
#include "chesswidget.h"
#include "chesspieceqgraphicsitem.h"
#include "chesspiece.h"

ChessPieceQGraphicsItem::ChessPieceQGraphicsItem(ChessWidget *cw,  ChessPiece* pc)
    : chessWidget(cw), piece(pc)
{
    setZValue(1);
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
    QGraphicsItem::mousePressEvent(event);

    if(chessWidget->getActivePlayer() == piece->getOwner()) {
        if(chessWidget->isPieceSelected() && chessWidget->getSelectedPiece() == piece->getId()){
            //if selected piece clicked -> clear selection
            chessWidget->clearSelectedPiece();
            chessWidget->updateChessBoard();
            std::cout << "Deselected piece: " << piece->toString() << std::endl;
        } else {
            //if unselected piece clicked -> select piece
            chessWidget->setSelectedPiece(piece->getId());
            chessWidget->updateChessBoard();
            std::cout << "Selected piece: " << piece->toString() << std::endl;
        }
    }
}

void ChessPieceQGraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    int x = event->scenePos().x();
    int y = event->scenePos().y();

    x = (x / SQUARE_WIDTH)*SQUARE_WIDTH;
    y = (y / SQUARE_WIDTH)*SQUARE_WIDTH;
//    std::cout << "(" << x  << "," << y << ")" << std::endl;

    setPos(x,y);
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


