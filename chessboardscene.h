#ifndef CHESSBOARDSCENE_H
#define CHESSBOARDSCENE_H

#include <QGraphicsScene>

#include "chessboarditem.h"
#include "chess/game.h"

class ChessBoardScene: public QGraphicsScene
{
    Q_OBJECT
public:
    ChessBoardScene(const Chess::Game& game, QObject* parent = nullptr);

public slots:
    void onReadyForNextMove();
    void onPerformMove();

signals:
    void moveSelected(Chess::Move move);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) override;

private:
    static Chess::BP toChessBoardPosition(QPointF point);

    void onClick(QGraphicsSceneMouseEvent* event);
    void onChessBoardClick(QGraphicsSceneMouseEvent* event);
    void onChessPieceClick(const ChessPieceItem* pieceItem);
    bool attemptMove(Chess::BP src, Chess::BP dst);
    void selectPiece(const Chess::Piece* piece);
    void deselectPiece();

    bool isSelected_ = false;
    const Chess::Piece* selectedPiece_ = nullptr;
    const Chess::Game& game_;
    ChessBoardItem* board_;
};

#endif // CHESSBOARDSCENE_H
