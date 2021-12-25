#include <QPainter>
#include "chesswidget.h"
#include "chessboardqgraphicsitem.h"

static const QColor kWhite = QColor("#f0d9b5");
static const QColor kBlack = QColor("#9b7555");
static const QColor kRed = QColor(227, 29, 29, 200);
static const QRect kSquare = QRect(0, 0, SQUARE_WIDTH, SQUARE_WIDTH);

ChessBoardQGraphicsItem::ChessBoardQGraphicsItem(ChessWidget *cw)
    : chessWidget_(cw)
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

    SGS sgs = chessWidget_->getBGState(i,j);
    switch(sgs) {
        case SGS::SOURCE:
            painter->fillRect(kSquare, QColor(211, 219, 51, 200));
            break;
        case SGS::NORMAL_MOVE:
            drawCircle(20, QColor(164, 164, 164, 128), painter);
            break;
        case SGS::CAPTURE:
            painter->fillRect(kSquare, QColor(164, 164, 164, 128));
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
    if(isInCheck_ && i == checkPos_.row() && j == checkPos_.col()) {
        //paint red square
        painter->fillRect(kSquare, kRed);
    } else if((i+j) % 2 == 0) {
        //paint white square
        painter->fillRect(kSquare, kWhite);
    } else {
        //paint black square
        painter->fillRect(kSquare, kBlack);
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
    const bool displayMoves = chessWidget_->isReadyToDisplayMoves() && chessWidget_->isPieceSelected();

    for(int i=0; i<NUM_ROWS; i++) {
        for(int j=0; j<NUM_COLS; j++) {
            drawNormalSquare(i, j, painter);
            if(displayMoves)
            {
                drawSquareState(i, j, painter);
            }
        }
    }
}

void ChessBoardQGraphicsItem::setCheck(Chess::BP pos){
    isInCheck_ = true;
    checkPos_ = pos;
}

void ChessBoardQGraphicsItem::clearCheck()
{
    isInCheck_ = false;
}


