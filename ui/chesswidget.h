#ifndef CHESSWIDGET_H
#define CHESSWIDGET_H

#include "chess/game.h"
#include "ui.h"

#include <QGraphicsView>
#include <QMessageBox>

enum class PlayerType{HUMAN, AI};

class ChessWidget : public QGraphicsView
{
    Q_OBJECT
public:
    ChessWidget(Chess::Game& game, QWidget *parent = nullptr);
    ~ChessWidget() = default;
    void resizeEvent(QResizeEvent *event) override;

    void newGame();
    void startTurn();
    void completeTurn(Chess::Move move);

signals:
    void readyToReceiveUserMove();
    void movePerformed();

private:
    Chess::Game& game_;
    PlayerType playerType[NUM_PLAYERS] = {PlayerType::HUMAN, PlayerType::HUMAN};
    QMessageBox gameEndBox;
};

#endif // CHESSWIDGET_H
