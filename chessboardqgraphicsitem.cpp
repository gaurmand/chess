#include <QPainter>
#include "constants.h"
#include "chesswidget.h"
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
}

void drawAxes(QPainter *painter) {
    painter->setPen(Qt::black);
    painter->drawLine(QLineF(0,0,1000,0));
    painter->drawLine(QLineF(0,0,-1000,0));
    painter->drawLine(QLineF(0,0,0,1000));
    painter->drawLine(QLineF(0,0,0,-1000));
}

void drawBounds(QPainter *painter) {
    painter->setPen(Qt::black);
    painter->drawLine(QLineF(0,0,BOARD_WIDTH,0));
    painter->drawLine(QLineF(BOARD_WIDTH,0,BOARD_WIDTH,BOARD_HEIGHT));
    painter->drawLine(QLineF(BOARD_WIDTH,BOARD_HEIGHT,0,BOARD_HEIGHT));
    painter->drawLine(QLineF(0,BOARD_HEIGHT,0,0));
}

void ChessBoardQGraphicsItem::drawSquareState(int i, int j, QPainter *painter)
{
    painter->save();
    painter->translate(j*SQUARE_WIDTH, i*SQUARE_WIDTH);

    BGS bgs = chessWidget->getBGState(i,j);
    switch(bgs) {
        case BGS::SOURCE:
            painter->fillRect(squareRect, QColor(224, 224, 117));
            break;
        case BGS::NORMAL_MOVE:
            drawCircle(20, QColor(164, 164, 164, 100), painter);
            break;
        default:
            break;
    }

    painter->restore();

}

void ChessBoardQGraphicsItem::drawNormalSquare(int i, int j, QPainter *painter)
{
    painter->save();
    painter->translate(j*SQUARE_WIDTH, i*SQUARE_WIDTH);
    if((i+j) % 2 == 0) {
        //paint white square
        painter->fillRect(squareRect, white);

    } else {
        //paint black square
        painter->fillRect(squareRect, black);
    }
    painter->restore();
}


void ChessBoardQGraphicsItem::drawCircle(int radius, QColor color, QPainter *painter)
{
    painter->setPen(Qt::NoPen);
    painter->setBrush(color);

    QPointF center = {SQUARE_WIDTH/2, SQUARE_WIDTH/2};
    painter->drawEllipse(center, radius,  radius);
}


void ChessBoardQGraphicsItem::drawBoard(QPainter *painter)
{
    painter->setRenderHint(QPainter::Antialiasing, true);
    bool displayMoves = chessWidget->isReadyToDisplayMoves() && chessWidget->isPieceSelected();

    for(int i=0; i<NUM_ROWS; i++) {
        for(int j=0; j<NUM_COLS; j++) {
            drawNormalSquare(i, j, painter);
            if(displayMoves) {
                drawSquareState(i, j, painter);
            }
        }
    }


}


