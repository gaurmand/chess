#include <QPainter>
#include "constants.h"
#include "chessboardqgraphicsitem.h"

ChessBoardQGraphicsItem::ChessBoardQGraphicsItem(ChessWidget *cw)
    : chessWidget(cw), white("#f0d9b5"), black("#9b7555"), squareRect(0, 0, SQUARE_WIDTH, SQUARE_WIDTH)
{

}

QRectF ChessBoardQGraphicsItem::boundingRect() const
{
    return QRectF(0, 0, BOARD_WIDTH, BOARD_HEIGHT);
}

void ChessBoardQGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    drawBoard(painter);

////    paint test axes
//    painter->setPen(Qt::black);
//    QLineF line;

//    line = QLineF(0,0,1000,0);
//    painter->drawLine(line);
//    line = QLineF(0,0,-1000,0);
//    painter->drawLine(line);
//    line = QLineF(0,0,0,1000);
//    painter->drawLine(line);
//    line = QLineF(0,0,0,-1000);
//    painter->drawLine(line);

////    paint testbounds
//    painter->setPen(Qt::black);
//    painter->drawLine(QLineF(0,0,BOARD_WIDTH,0));
//    painter->drawLine(QLineF(BOARD_WIDTH,0,BOARD_WIDTH,BOARD_HEIGHT));
//    painter->drawLine(QLineF(BOARD_WIDTH,BOARD_HEIGHT,0,BOARD_HEIGHT));
//    painter->drawLine(QLineF(0,BOARD_HEIGHT,0,0));
}

void ChessBoardQGraphicsItem::drawSquare(int i, int j, QPainter *painter, QColor color)
{
    painter->save();
    painter->translate(j*SQUARE_WIDTH, i*SQUARE_WIDTH);
    painter->fillRect(squareRect, color);
    painter->restore();
}

void ChessBoardQGraphicsItem::drawBoard(QPainter *painter)
{
    painter->setRenderHint(QPainter::Antialiasing, true);

    for(int i=0; i<NUM_ROWS; i++) {
        for(int j=0; j<NUM_COLS; j++) {
            if((i+j) % 2 == 0) {
                //paint white
                drawSquare(i, j, painter, white);
            } else {
                //paint black
                drawSquare(i, j, painter, black);
            }
        }
    }
}


