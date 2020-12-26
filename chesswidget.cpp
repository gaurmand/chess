#include <QFrame>
#include "chesswidget.h"

ChessWidget::ChessWidget(QWidget *parent)
    : QGraphicsView(parent)
{
    //init pixmaps
    for(int i=0; i<NUM_PLAYERS; i++) {
        for(int j=0; j<NUM_CHESS_PIECE_TYPES; j++) {
            QString path = ":/images/%1%2.png";
            if(Player(i) == WHITE) {
                piecePixmaps[i][j] = new QPixmap();
                piecePixmaps[i][j]->load(path.arg("w").arg(j));
            } else {
                piecePixmaps[i][j] = new QPixmap;
                piecePixmaps[i][j]->load(path.arg("b").arg(j));
            }
        }
    }

    //init chessboard
    chessBoard = new ChessBoardQGraphicsItem(this);

    //init chesspieces
    for(int i=0; i<NUM_PLAYERS; i++) {
        for(int j=0; j<NUM_CHESS_PIECES; j++) {
            ChessPiece* piece = game.getChessPiece(Player(i), PieceID(j));
            pieces[i][j] = new ChessPieceQGraphicsItem(this, piece);
        }
    }

    //init scene
    scene = new QGraphicsScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    scene->setSceneRect(0, 0, BOARD_WIDTH, BOARD_HEIGHT);
    setBackgroundBrush(Qt::darkGray);
    setScene(scene);
    setRenderHint(QPainter::Antialiasing);

    //add chessboard item to scene
    scene->addItem(chessBoard);

    //add chesspiece items to scene
    for(int i=0; i<NUM_PLAYERS; i++) {
        for(int j=0; j<NUM_CHESS_PIECES; j++) {
            scene->addItem(pieces[i][j]);
        }
    }

    //get rid of margin related to fitInView
    setViewportMargins(-2, -2, -2, -2);
    setFrameStyle(QFrame::NoFrame);

    //set min widget size
    setMinimumSize(400, 400);
}

ChessWidget::~ChessWidget() {
    delete chessBoard;

    for(int i=0; i<NUM_PLAYERS; i++) {
        for(int j=0; j<NUM_CHESS_PIECE_TYPES; j++) {
            delete pieces[i][j];
        }
    }
}

void ChessWidget::resizeEvent(QResizeEvent *event) {
    Q_UNUSED(event);

    fitInView(QRect(0,0,BOARD_WIDTH, BOARD_HEIGHT), Qt::AspectRatioMode::KeepAspectRatio);
}

void ChessWidget::setInitialBoardState() {

}

QPixmap* ChessWidget::getPiecePixmap(PieceType type, Player player) {
    return piecePixmaps[player][type];
}
