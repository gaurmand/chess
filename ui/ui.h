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
    extern QRectF kBoardSquareRect;
    extern QRectF kBoardRect;

    void updateBoardSize(const QSize& size);

    namespace colour
    {
        extern QColor kBoardWhite;
        extern QColor kBoardBlack;
        extern QColor kBoardCheck;
        extern QColor kBoardSource;
        extern QColor kBoardCapture;
        extern QColor kBoardPrevSrc;
        extern QColor kBoardPrevDst;
        extern QColor kBoardMoveMouseOver;

        extern QColor kSceneBackground;
    }

    enum BPInfo
    {
        NONE = 0x0,
        SOURCE = 0x1,
        NORMAL_MOVE = 0x2,
        CAPTURE = 0x4,
        CHECK = 0x8,
        PREV_SRC = 0x10,
        PREV_DST = 0x20
    };
    Q_DECLARE_FLAGS(BPState, BPInfo)
    Q_DECLARE_OPERATORS_FOR_FLAGS(BPState)

    typedef std::array<std::array<ui::BPState, NUM_COLS>, NUM_ROWS> BPStates;

    QPixmap piecePixmap(const Chess::Player player, const Chess::PieceType type);

    Chess::BP sceneToBP(const QPointF& point);
    QPointF BPToScene(const Chess::BP& pos);
}


#endif // UI_H
