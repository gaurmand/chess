#include "ui.h"

#include <QIcon>

namespace ui
{
    QRectF kBoardSquareRect(0, 0, SQUARE_WIDTH, SQUARE_WIDTH);
    QRectF kBoardRect(0, 0, BOARD_WIDTH, BOARD_HEIGHT);

    void updateBoardSize(const QSize& size)
    {
        const int min = std::min(size.width(), size.height());
        const double newWidth = min / 8.0;
        kBoardSquareRect = QRectF(0, 0, newWidth, newWidth);
        kBoardRect = QRectF(0, 0, 8.0 * newWidth, 8.0 * newWidth);
    }

    namespace colour
    {
        QColor kBoardWhite(240, 217, 181);
        QColor kBoardBlack(155, 117, 85);
        QColor kBoardCheck(227, 29, 29, 200);
        QColor kBoardSource(211, 219, 51, 200);
        QColor kBoardCapture(164, 164, 164, 100);
        QColor kBoardPrevSrc(184, 154, 52);
        QColor kBoardPrevDst(215, 181, 68);
        QColor kBoardMoveMouseOver(239, 239, 239, 225);

        QColor kSceneBackground(40, 40, 40);
    }

    const QIcon& pieceIcon(const Chess::Player player, const Chess::PieceType type)
    {
        static const QIcon whiteKing(":/images/wking.svg");
        static const QIcon whiteQueen(":/images/wqueen.svg");
        static const QIcon whiteRook(":/images/wrook.svg");
        static const QIcon whiteBishop(":/images/wbishop.svg");
        static const QIcon whiteKnight(":/images/wknight.svg");
        static const QIcon whitePawn(":/images/wpawn.svg");

        static const QIcon blackKing(":/images/bking.svg");
        static const QIcon blackQueen(":/images/bqueen.svg");
        static const QIcon blackRook(":/images/brook.svg");
        static const QIcon blackBishop(":/images/bbishop.svg");
        static const QIcon blackKnight(":/images/bknight.svg");
        static const QIcon blackPawn(":/images/bpawn.svg");

        switch (type)
        {
            case Chess::PieceType::King:
                return player == Chess::Player::White ? whiteKing : blackKing;
            case Chess::PieceType::Queen:
                return player == Chess::Player::White ? whiteQueen : blackQueen;
            case Chess::PieceType::Rook:
                return player == Chess::Player::White ? whiteRook : blackRook;
            case Chess::PieceType::Bishop:
                return player == Chess::Player::White ? whiteBishop : blackBishop;
            case Chess::PieceType::Knight:
                return player == Chess::Player::White ? whiteKnight : blackKnight;
            case Chess::PieceType::Pawn:
                return player == Chess::Player::White ? whitePawn : blackPawn;
            default:
                throw 1;
        }
    }

    QPixmap piecePixmap(const Chess::Player player, const Chess::PieceType type)
    {
        return pieceIcon(player, type).pixmap(kBoardSquareRect.toAlignedRect().size());
    }

    Chess::BP sceneToBP(const QPointF& point)
    {
        const int row = point.y() / kBoardSquareRect.width();
        const int col = point.x() / kBoardSquareRect.width();
        return Chess::BP(row, col);
    }

    QPointF BPToScene(const Chess::BP& pos)
    {
        const double x = pos.col() * kBoardSquareRect.width();
        const double y = pos.row() * kBoardSquareRect.width();
        return QPointF(x, y);
    }
}
