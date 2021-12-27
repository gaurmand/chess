#include <QFrame>
#include <QMouseEvent>
#include <QGraphicsItem>
#include <iostream>
#include "chesswidget.h"

ChessWidget::ChessWidget(QWidget *parent)
    : QGraphicsView(parent)
{
    //init board graphical state
    for(int pid=0; pid<NUM_CHESS_PIECES; pid++) {
        for(int i=0; i<NUM_ROWS; i++) {
            for(int j=0; j<NUM_COLS; j++) {
                boardGraphicalState[pid][i][j] = SGS::NONE;
            }
        }
    }

    //init chessboard
    chessBoard = new ChessBoardQGraphicsItem(this);

    //init chesspieces
    const auto wpieces = game_.pieceRefs(Chess::Player::White);
    for (const Chess::Piece& piece: wpieces)
    {
        const int id = static_cast<int>(piece.id());
        pieces[Chess::Player::White][id] = new ChessPieceQGraphicsItem(this, piece);
    }

    const auto bpieces = game_.pieceRefs(Chess::Player::Black);
    for (const Chess::Piece& piece: bpieces)
    {
        const int id = static_cast<int>(piece.id());
        pieces[Chess::Player::Black][id] = new ChessPieceQGraphicsItem(this, piece);
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
    newGame();

    //init msg box
    gameEndBox.setText("Checkmate");
    gameEndBox.setInformativeText("Do you want to start a new game?");
    gameEndBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    gameEndBox.setDefaultButton(QMessageBox::Yes);
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

void ChessWidget::mousePressEvent(QMouseEvent* event)
{
    QGraphicsItem* item = itemAt(event->pos());
    if(item && item->type() == CHESSBOARD_QGRAPHICSITEM_TYPE) {
        Chess::BP clickedPos = getChessboardPosition(event->pos());
        chessBoardItemMousePress(clickedPos);
    }
    QGraphicsView::mousePressEvent(event);
}

void ChessWidget::chessPieceItemMousePress(const Chess::Piece* piece)
{
    //no piece is selected and any owned piece is clicked
    bool cond1 = !isPieceSelected() && game_.activePlayer() == piece->owner();

    //a piece is currently selected and any other owned piece is clicked
    bool cond2 = isPieceSelected() && game_.activePlayer() == piece->owner() && getSelectedPiece() != piece;

    //a piece is selected and an enemy piece is clicked -> treat as chessboard click
    bool cond3 = isPieceSelected() && game_.activePlayer() != piece->owner();

    if(cond1 || cond2) {
        selectPiece(piece);

        //set flag to prevent next mouse release event from deselecting
        isRecentSelection = true;
    } else if(cond3) {
        chessBoardItemMousePress(piece->pos());
    }
}

void ChessWidget::chessBoardItemMousePress(Chess::BP pos)
{
    if(isPieceSelected()) {
        int pid = selectedPiece_->id();
        SGS sgs = boardGraphicalState[pid][pos.row()][pos.col()];

        switch(sgs) {
            case SGS::NORMAL_MOVE:
            case SGS::CAPTURE:
            {
                Chess::Move move = getPlayerSelectedMove(selectedPiece_, pos);
                completeTurn(move);
                return;
            }
            case SGS::SOURCE:
            case SGS::NONE:
            default:
                deselectPiece();
                return;
        }
    }
}

Chess::Move ChessWidget::getPlayerSelectedMove(const Chess::Piece* piece, Chess::BP dst)
{
    std::vector<Chess::Move> moves = game_.moves(piece->pos().toANBP());

    for (const auto& move: moves)
    {
        if (move.dst() == dst)
        {
            return move;
        }
    }
    return Chess::Move();
}


void ChessWidget::chessPieceItemMouseRelease(const Chess::Piece* piece, QPointF point)
{
    Chess::BP clickedPos = ChessWidget::getChessboardPosition(point);
    if(!ChessWidget::isSamePosition(clickedPos, piece->pos())) {
        //if piece dragged to different square -> treat like chessboard click
        chessBoardItemMousePress(clickedPos);
    }else if(isRecentSelection) {
        //if mouse press event just selected piece -> ignore mouse release
        isRecentSelection = false;
    } else {
        deselectPiece();
    }
}

Chess::BP ChessWidget::getChessboardPosition(QPointF point)
{
    int x = point.x();
    int y = point.y();

    int i = (y / SQUARE_WIDTH);
    int j = (x / SQUARE_WIDTH);
    return Chess::BP(i, j);
}

bool ChessWidget::isClickInChessBoard(QPointF point)
{
    int x = point.x();
    int y = point.y();
    return (x >= 0 && x<=BOARD_WIDTH) && (y>=0 && y<=BOARD_HEIGHT);
}

bool ChessWidget::isSamePosition(Chess::BP p1, Chess::BP p2)
{
    return p1 == p2;
}


void ChessWidget::setInitialBoardState() {

}

void ChessWidget::newGame()
{
    setUnreadyToDisplayMoves();
    deselectPiece();
    setAllPiecesUnmovable();
    game_ = Chess::Game();
    updatePieces();
    startTurn();
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

void ChessWidget::selectPiece(const Chess::Piece* piece)
{
    pieceSelected = true;
    selectedPiece_ = piece;
    chessBoard->update();
    std::cout << "Selected piece: " << *selectedPiece_ << std::endl;
}

void ChessWidget::deselectPiece()
{
    std::cout << "Deselected piece: ";
    if (pieceSelected)
    {
        std::cout << *selectedPiece_;
    }
    std::cout << std::endl;

    pieceSelected = false;
    chessBoard->update();
}


bool ChessWidget::isPieceSelected()
{
    return pieceSelected;
}


const Chess::Piece* ChessWidget::getSelectedPiece()
{
    return selectedPiece_;
}

SGS ChessWidget::getBGState(int i, int j)
{
    if(pieceSelected) {
        return boardGraphicalState[selectedPiece_->id()][i][j];
    } else {
        return SGS::NORMAL_MOVE;
    }
}

void ChessWidget::startTurn()
{
    Chess::Player active = game_.activePlayer();
    PlayerType ptype = playerType[active];
    std::cout << "Start turn: " << active << (ptype == PlayerType::HUMAN ? "(Human)" : "(AI)") << std::endl;

    if(ptype == PlayerType::HUMAN) {
        //HUMAN player -> wait for them to select move
        computeBoardGraphicalStates();
        setReadyToDisplayMoves();
        setPiecesMovable(active);
    } else {
        //AI player -> wait for move from chess engine
    }
}

void ChessWidget::completeTurn(Chess::Move move)
{
    std::cout << "Selected move: " << move << std::endl;

    if(playerType[game_.activePlayer()] == PlayerType::HUMAN)
    {
        playerTurn(move);
    } else
    {
        AITurn(move);
    }

    try {
        game_.performMove(move);
        std::cout << "Performed move: " << move << std::endl;

    }  catch (...)
    {
        std::cout << "Move failed: " << move << std::endl;
    }

    updatePieces();

    if (game_.isComplete())
    {
        switch(game_.result())
        {
            case Chess::ResultType::Checkmate:
            {
                std::string winner = (game_.activePlayer() == Chess::Player::White ? "Black" : "White");
                gameEndBox.setText(QString::fromStdString("Checkmate - " + winner + " wins"));
                break;
            }
            case Chess::ResultType::Draw:
            case Chess::ResultType::Stalemate:
            default:
                gameEndBox.setText(QString::fromStdString("Stalemate - Nobody wins"));
        }

        if(gameEndBox.exec() == QMessageBox::Yes) {
            newGame();
        }
    }
    else
    {
        startTurn();
    }
}

void ChessWidget::playerTurn(Chess::Move move)
{
    deselectPiece();
    setAllPiecesUnmovable();
    return;
}

void ChessWidget::AITurn(Chess::Move move)
{
    return;
}

void ChessWidget::computeBoardGraphicalStates()
{
    std::vector<Chess::Piece> pieces = game_.pieces(game_.activePlayer());
    for (auto& piece: pieces)
    {
        //initialize board states for that piece to normal
        for(int i=0; i<NUM_ROWS; i++) {
            for(int j=0; j<NUM_COLS; j++) {
                boardGraphicalState[piece.id()][i][j] = SGS::NONE;
            }
        }

        //set piece square as source
        Chess::BP pos = piece.pos();
        boardGraphicalState[piece.id()][pos.row()][pos.col()] = SGS::SOURCE;

        //for each move, set dst square board state
        std::vector<Chess::Move> moves = game_.moves(piece.pos());
        for(auto& move: moves)
        {
            Chess::BP dst = move.dst();
            if(move.type() == Chess::MoveType::Capture)
            {
                boardGraphicalState[piece.id()][dst.row()][dst.col()] = SGS::CAPTURE;
            } else
            {
                boardGraphicalState[piece.id()][dst.row()][dst.col()] = SGS::NORMAL_MOVE;
            }
        }

    }

    //clear any previous check
    chessBoard->clearCheck();

    //if active king in check, set king square check state
    if(game_.isInCheck()) {
        Chess::BP kingPos = game_.kingPosition(game_.activePlayer());
        chessBoard->setCheck(kingPos);
    }
}

void ChessWidget::setPiecesMovable(Chess::Player player)
{
    Chess::Player active = player;
    Chess::Player inactive = (player == Chess::Player::White ? Chess::Player::Black : Chess::Player::White);

    for(int pid=0; pid<NUM_CHESS_PIECES; pid++) {
        pieces[active][pid]->setFlag(QGraphicsItem::ItemIsMovable, true);
        pieces[inactive][pid]->setFlag(QGraphicsItem::ItemIsMovable, false);
    }
}

void ChessWidget::setAllPiecesUnmovable()
{
    for(int pid=0; pid<NUM_CHESS_PIECES; pid++) {
        pieces[Chess::Player::White][pid]->setFlag(QGraphicsItem::ItemIsMovable, false);
        pieces[Chess::Player::Black][pid]->setFlag(QGraphicsItem::ItemIsMovable, false);
    }
}

void ChessWidget::updatePieces()
{
    for(int i=0; i<NUM_PLAYERS; i++)
    {
        for(int j=0; j<NUM_CHESS_PIECES; j++)
        {
            pieces[i][j]->updateItem();
        }
    }
}
