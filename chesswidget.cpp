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

    //init board graphical state
    for(int pid=0; pid<NUM_CHESS_PIECES; pid++) {
        for(int i=0; i<NUM_ROWS; i++) {
            for(int j=0; j<NUM_COLS; j++) {
                boardGraphicalState[pid][i][j] = BGState::NORMAL;
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

    //start new game
    newGame(false);
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

void ChessWidget::newGame(bool isWAI)
{
    isWhiteAI = isWAI;
    //game.newGame();
    setUnreadyToDisplayMoves();
    clearSelectedPiece();
    nextTurn();
}

bool ChessWidget::isReadyToDisplayMoves()
{
    return readyToDisplayMoves;
}

void ChessWidget::setReadyToDisplayMoves()
{
    readyToDisplayMoves = true;
}

void ChessWidget::setUnreadyToDisplayMoves()
{
    readyToDisplayMoves = false;
}


bool ChessWidget::isPieceSelected()
{
    return pieceSelected;
}

void ChessWidget::setSelectedPiece(PieceID pid)
{
    pieceSelected = true;
    selectedPiece = pid;
}

void ChessWidget::clearSelectedPiece()
{
    pieceSelected = false;
}

PieceID ChessWidget::getSelectedPiece()
{
    return selectedPiece;
}

BGState ChessWidget::getBGState(int i, int j)
{
    if(pieceSelected) {
        return boardGraphicalState[selectedPiece][i][j];
    } else {
        return BGState::NORMAL;
    }
}


void ChessWidget::nextTurn()
{
    game.generateMoves();
    computeBoardGraphicalStates();
    setReadyToDisplayMoves();

//    ChessMove move = selectMove();
//    performMove(move);
}

ChessMove ChessWidget::selectMove()
{
    bool isWhiteTurn = game.getActivePlayer() == WHITE;
    if((!isWhiteTurn && isWhiteAI) || (isWhiteTurn && !isWhiteAI)) {
        return playerSelectMove();
    } else {
        return AISelectMove();
    }
}

ChessMove ChessWidget::playerSelectMove()
{
    return "e1e4";
}

ChessMove ChessWidget::AISelectMove()
{
    return "e1e4";
}

bool ChessWidget::performMove(ChessMove move)
{
    return true;
}

void ChessWidget::computeBoardGraphicalStates()
{
    Player activePlayer = game.getActivePlayer();

    //for each chess piece, compute board graphical state
    for(int pid=0; pid<NUM_CHESS_PIECES; pid++) {
        ChessPiece* piece = game.getChessPiece(activePlayer, PieceID(pid));
        ChessMoves* moves = game.getChessMoves(activePlayer, PieceID(pid));

        //initialize board states for that piece to normal
        for(int i=0; i<NUM_ROWS; i++) {
            for(int j=0; j<NUM_COLS; j++) {
                boardGraphicalState[pid][i][j] = BGState::NORMAL;
            }
        }

        //set piece square as source
        IBP pos = piece->getIBPos();
        boardGraphicalState[pid][pos.row][pos.col] = BGState::SOURCE;

        //for each move, set dst square board state
        if(moves == nullptr)
            continue;

        for(ChessMoves::iterator it = moves->begin(); it != moves->end(); ++it) {
            ChessMove move = *it;
            IBP dst = ChessBoard::getMoveDst(move);
            boardGraphicalState[pid][dst.row][dst.col] = BGState::MOVE;
        }

    }
}

void ChessWidget::updateChessBoard()
{
    chessBoard->update();
}

Player ChessWidget::getActivePlayer()
{
    return game.getActivePlayer();
}
