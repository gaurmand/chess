#ifndef CHESSBOARDSCENE_H
#define CHESSBOARDSCENE_H

#include "chessboarditem.h"
#include "chesspieceitem.h"
#include "abstractchessgame.h"
#include "ui.h"
#include "chess/common.h"

#include <QGraphicsScene>
#include <QPropertyAnimation>
#include <QPointer>

class ChessBoardScene: public AbstractChessGame, public QGraphicsScene
{
public:
    ChessBoardScene();
    ~ChessBoardScene();

protected:
    void readyForHumanMove() override;
    void finishGame() override;
    void updateVisual() override;

    void enablePieceInteraction();
    void disablePieceInteraction();
    void visualBoardUpdate(const Chess::BP& selected = Chess::BP());
    void visualPiecesUpdate();
    void resetPiecePositions();

    void animatePiece(ChessPieceItem* pieceItem, const Chess::BP& dst, std::function<void()> cb = [](){});

    bool isPieceSelected();
    const Chess::Piece* selectedPiece_ = nullptr;
    ChessPieceItem* selectedPieceItem_ = nullptr;

    void selectPiece(ChessPieceItem* piece);
    void deselectPiece();
    void clearSelection();
    ChessPieceItem* chessPieceItemAt(const Chess::BP& pos);

    ChessBoardItem* board_;

private:
    ui::BPStates computeDefaultBPStates() const;
    ui::BPStates computeSelectedBPStates(const Chess::BP selected) const;

    ui::BPStates defaultStates_;
    std::vector<std::vector<ChessPieceItem*>> pieces_;
};

class InteractiveChessBoardScene: public ChessBoardScene
{
public:
    InteractiveChessBoardScene() {}

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;

    void onChessBoardPress(const Chess::BP& pressPos);
    void onChessPiecePress(ChessPieceItem* pieceItem);
    void onChessPieceRelease(const Chess::BP& releasePos);
    bool isRecentSelection_ = false;
};

#endif // CHESSBOARDSCENE_H
