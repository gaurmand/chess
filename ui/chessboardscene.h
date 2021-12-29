#ifndef CHESSBOARDSCENE_H
#define CHESSBOARDSCENE_H

#include "chessboarditem.h"
#include "chesspieceitem.h"
#include "chess/game.h"

#include <QGraphicsScene>

class ChessBoardScene: public QGraphicsScene
{
    Q_OBJECT
public:
    ChessBoardScene(const Chess::Game& game, QObject* parent = nullptr);

    void onChessBoardClick(QGraphicsSceneMouseEvent* event);
    void onChessPieceClick(const ChessPieceItem* pieceItem);
    void onChessPieceRelease(QGraphicsSceneMouseEvent* event);

public slots:
    void onReadyForNextMove();
    void onPerformMove(const Chess::Move& move);

signals:
    void moveSelected(Chess::Move move);

private:
    void onClick(QGraphicsSceneMouseEvent* event);

    bool attemptMove(Chess::BP src, Chess::BP dst);
    void selectPiece(const Chess::Piece* piece);
    void deselectPiece();

    void updatePiecePositions();
    void setPiecesMovable(Chess::Player player);
    void setPiecesMovable(bool movable);

    bool isSelected_ = false;
    const Chess::Piece* selectedPiece_ = nullptr;
    bool isRecentSelection_ = false;

    const Chess::Game& game_;
    ChessBoardItem* board_;
    std::array<std::array<ChessPieceItem, NUM_CHESS_PIECES>, NUM_PLAYERS> pieces_;
};

#endif // CHESSBOARDSCENE_H
