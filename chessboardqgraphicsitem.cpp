#include <QPainter>
#include "chesswidget.h"
#include "chessboardqgraphicsitem.h"

#include "chess/move.h"


static const QColor kWhite = QColor("#f0d9b5");
static const QColor kBlack = QColor("#9b7555");
static const QColor kRed = QColor(227, 29, 29, 200);
static const QColor kGreen = QColor(211, 219, 51, 200);
static const QColor kGray = QColor(164, 164, 164, 128);
static const QRect kSquare = QRect(0, 0, SQUARE_WIDTH, SQUARE_WIDTH);

ChessBoardQGraphicsItem::ChessBoardQGraphicsItem(ChessWidget *cw)
    : chessWidget_(cw)
{
    std::for_each(states_.begin(), states_.end(), [](auto& row){ row.fill(SquareState::NONE); });
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

void ChessBoardQGraphicsItem::drawSquare(int i, int j, QPainter *painter)
{
    const auto drawNormalSquare = [&] () {
        if((i+j) % 2 == 0)
        {
            painter->fillRect(kSquare, kWhite);
        }
        else
        {
            painter->fillRect(kSquare, kBlack);
        }
    };

    painter->save();
    painter->translate(j*SQUARE_WIDTH, i*SQUARE_WIDTH);

    const SquareState state = states_[i][j];
    switch(state)
    {
        case SquareState::CHECK:
            painter->fillRect(kSquare, kRed);
            break;
        case SquareState::SOURCE:
            painter->fillRect(kSquare, kGreen);
            break;
        case SquareState::CAPTURE:
            painter->fillRect(kSquare, kGray);
            break;
        case SquareState::NORMAL_MOVE:
        {
            static const QPointF center(SQUARE_WIDTH/2, SQUARE_WIDTH/2);
            static const int radius = 20;

            drawNormalSquare();
            painter->setPen(Qt::NoPen);
            painter->setBrush(kGray);
            painter->drawEllipse(center, radius,  radius);
            break;
        }
        case SquareState::NONE:
            drawNormalSquare();
            break;
    }

    painter->restore();
}

void ChessBoardQGraphicsItem::drawBoard(QPainter *painter)
{
    painter->setRenderHint(QPainter::Antialiasing, true);
    for(int i=0; i<NUM_ROWS; i++) {
        for(int j=0; j<NUM_COLS; j++) {
            drawSquare(i, j, painter);
        }
    }
}

void ChessBoardQGraphicsItem::select(const Chess::BP& src, const Chess::Game& game)
{
    // initialize all states to normal
    std::for_each(states_.begin(), states_.end(), [](auto& row){ row.fill(SquareState::NONE); });

    // move source square
    states_[src.row()][src.col()] = SquareState::SOURCE;

    // king square if in check
    if (game.isInCheck())
    {
        const Chess::BP check = game.kingPosition(game.activePlayer());
        states_[check.row()][check.col()] = SquareState::CHECK;
    }

    // move squares (normal and captures)
    const auto moves = game.moves(src);
    for (const auto& move: moves)
    {
        const Chess::BP dst = move.dst();
        if (move.type() == Chess::MoveType::Capture)
        {
            states_[dst.row()][dst.col()] = SquareState::CAPTURE;
        }
        else
        {
            states_[dst.row()][dst.col()] = SquareState::NORMAL_MOVE;
        }
    }

    update();
}

void ChessBoardQGraphicsItem::deselect()
{
    std::for_each(states_.begin(), states_.end(), [](auto& row){ row.fill(SquareState::NONE); });
    update();
}
