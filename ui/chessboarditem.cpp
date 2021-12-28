#include "chessboarditem.h"
#include "ui.h"
#include "chess/move.h"

#include <QPainter>
#include <QGraphicsSceneMouseEvent>

ChessBoardItem::ChessBoardItem(const Chess::Game& game)
{
    setAcceptDrops(true);
    std::for_each(states_.begin(), states_.end(), [](auto& row){ row.fill(SquareState::NONE); });
}

QRectF ChessBoardItem::boundingRect() const
{
    return QRectF(0, 0, BOARD_WIDTH, BOARD_HEIGHT);
}

void ChessBoardItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
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

void ChessBoardItem::drawSquare(int i, int j, QPainter *painter)
{
    const auto drawNormalSquare = [&] () {
        if((i+j) % 2 == 0)
        {
            painter->fillRect(ui::kBoardSquareRect, ui::colour::kBoardWhite);
        }
        else
        {
            painter->fillRect(ui::kBoardSquareRect, ui::colour::kBoardBlack);
        }
    };

    painter->save();
    painter->translate(j*SQUARE_WIDTH, i*SQUARE_WIDTH);

    const SquareState state = states_[i][j];
    switch(state)
    {
        case SquareState::CHECK:
            painter->fillRect(ui::kBoardSquareRect, ui::colour::kBoardCheck);
            break;
        case SquareState::SOURCE:
            painter->fillRect(ui::kBoardSquareRect, ui::colour::kBoardSource);
            break;
        case SquareState::CAPTURE:
            painter->fillRect(ui::kBoardSquareRect, ui::colour::kBoardCapture);
            break;
        case SquareState::NORMAL_MOVE:
        {
            static const QPointF center(SQUARE_WIDTH/2, SQUARE_WIDTH/2);
            static const int radius = 20;

            drawNormalSquare();
            painter->setPen(Qt::NoPen);
            painter->setBrush(ui::colour::kBoardCapture);
            painter->drawEllipse(center, radius,  radius);
            break;
        }
        case SquareState::NONE:
            drawNormalSquare();
            break;
    }

    painter->restore();
}

void ChessBoardItem::drawBoard(QPainter *painter)
{
    painter->setRenderHint(QPainter::Antialiasing, true);
    for(int i=0; i<NUM_ROWS; i++) {
        for(int j=0; j<NUM_COLS; j++) {
            drawSquare(i, j, painter);
        }
    }
}

void ChessBoardItem::setSelectedState(const Chess::BP& src, const Chess::Game& game)
{
    // initialize all states to normal
    std::for_each(states_.begin(), states_.end(), [](auto& row){ row.fill(SquareState::NONE); });

    // move source square
    states_[src.row()][src.col()] = SquareState::SOURCE;

    // king square if in check
    if (isInCheck_)
    {
        states_[checkPos_.row()][checkPos_.col()] = SquareState::CHECK;
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

void ChessBoardItem::setDeselectedState()
{
    std::for_each(states_.begin(), states_.end(), [](auto& row){ row.fill(SquareState::NONE); });
    if (isInCheck_)
    {
        states_[checkPos_.row()][checkPos_.col()] = SquareState::CHECK;
    }
    update();
}

void ChessBoardItem::setCheckState(const Chess::Game& game)
{
    isInCheck_ = game.isInCheck();
    if (isInCheck_)
    {
        checkPos_ = game.kingPosition(game.activePlayer());
    }
    setDeselectedState();
}

void ChessBoardItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsItem::mousePressEvent(event);
    emit mousePress(event);
}
