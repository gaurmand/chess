#include "ui.h"

namespace ui
{
    QRect kBoardSquareRect(0, 0, SQUARE_WIDTH, SQUARE_WIDTH);
    QRect kBoardRect(0, 0, BOARD_WIDTH, BOARD_HEIGHT);

    namespace colour
    {
        QColor kBoardWhite(240, 217, 181);
        QColor kBoardBlack(155, 117, 85);
        QColor kBoardCheck(227, 29, 29, 200);
        QColor kBoardSource(211, 219, 51, 200);
        QColor kBoardCapture(164, 164, 164, 128);
    }

    const QPixmap& piecePixmap(Chess::Player player, Chess::PieceType type)
    {
        static const QPixmap whiteKing(":/images/w0");
        static const QPixmap whiteQueen(":/images/w1");
        static const QPixmap whiteRook(":/images/w2");
        static const QPixmap whiteBishop(":/images/w3");
        static const QPixmap whiteKnight(":/images/w4");
        static const QPixmap whitePawn(":/images/w5");

        static const QPixmap blackKing(":/images/b0");
        static const QPixmap blackQueen(":/images/b1");
        static const QPixmap blackRook(":/images/b2");
        static const QPixmap blackBishop(":/images/b3");
        static const QPixmap blackKnight(":/images/b4");
        static const QPixmap blackPawn(":/images/b5");

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

    Chess::BP sceneToBP(QPointF point)
    {
        int x = point.x();
        int y = point.y();

        int i = (y / SQUARE_WIDTH);
        int j = (x / SQUARE_WIDTH);
        return Chess::BP(i, j);
    }
}
