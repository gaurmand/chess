#ifndef UI_H
#define UI_H

#include "chess/common.h"
#include "chess/boardpos.h"

#include <QRect>
#include <QColor>
#include <QPixmap>

#define SQUARE_WIDTH 90
#define BOARD_WIDTH SQUARE_WIDTH*NUM_ROWS
#define BOARD_HEIGHT SQUARE_WIDTH*NUM_COLS

namespace ui
{
    extern QRect kBoardSquareRect;
    extern QRect kBoardRect;

    namespace colour
    {
        extern QColor kBoardWhite;
        extern QColor kBoardBlack;
        extern QColor kBoardCheck;
        extern QColor kBoardSource;
        extern QColor kBoardCapture;
    }

    const QPixmap& piecePixmap(Chess::Player player, Chess::PieceType type);

    Chess::BP sceneToBP(QPointF point);
}

#endif // UI_H
