#ifndef CHESSWIDGET_H
#define CHESSWIDGET_H

#include "chess/game.h"
#include "ui.h"

#include <QGraphicsView>
#include <QMessageBox>

class ChessWidget : public QGraphicsView
{
    Q_OBJECT
public:
    ChessWidget(QWidget *parent = nullptr);
    ~ChessWidget() = default;

public slots:
    void showGameCompleteDialog(const Chess::ResultType result, const Chess::Player lastPlayer);

signals:
    void requestNewGame();

private:
    QMessageBox gameEndBox;
};

#endif // CHESSWIDGET_H
