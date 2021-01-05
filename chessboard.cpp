#include <algorithm>
#include <map>
#include "chessboard.h"
#include "chesspiece.h"

ChessBoard::ChessBoard()
{
    clearBoard();
}

ChessPiece* ChessBoard::getPiece(IBP pos)
{
    return board[pos.row][pos.col];
}

void ChessBoard::setPiece(ChessPiece* piece, IBP pos)
{
    board[pos.row][pos.col] = piece;
    if(piece) {
        piece->setIBPos(pos);
    }
}

ChessPiece* ChessBoard::movePiece(IBP src, IBP dst)
{
    ChessPiece* dstPiece = board[dst.row][dst.col];
    ChessPiece* srcPiece = board[src.row][src.col];

    if(srcPiece == nullptr) {
        //do nothing if no piece at src position
        return nullptr;
    }

    setPiece(srcPiece, dst);
    setPiece(nullptr, src);
    return dstPiece;
}

void ChessBoard::clearBoard()
{
    for(int i=0; i<NUM_ROWS; i++) {
        for(int j=0; j<NUM_COLS; j++) {
            board[i][j] = nullptr;
        }
    }
}

bool ChessBoard::performMove(ChessMove move, bool enablePromotion)
{
    IBP src = BoardPosition::getMoveSrcIBP(move);
    IBP dst = BoardPosition::getMoveDstIBP(move);

    ChessPiece* captured = movePiece(src, dst);
    if(captured != nullptr) {
        captured->setCaptured(true);
    }

    return true;
}

bool ChessBoard::isMoveCapture(ChessMove move)
{
    IBP dst = BoardPosition::getMoveDstIBP(move);
    return (getPiece(dst) != nullptr);
}

ChessMoves* ChessBoard::getValidMoves(ChessPiece* piece, ChessMoveTypeOpt opts)
{
    ChessMoves* validMoves;
    switch(piece->getType()) {
        case PAWN:
            validMoves = getValidPawnMoves(piece, opts);
            break;
        case KING:
            validMoves = getValidKingMoves(piece, opts);
            break;
        case KNIGHT:
            validMoves = getValidKnightMoves(piece);
            break;
        case ROOK:
            validMoves = getValidRookMoves(piece);
            break;
        case BISHOP:
            validMoves = getValidBishopMoves(piece);
            break;
        case QUEEN:
            validMoves = getValidQueenMoves(piece);
        break;
    }

    if(!validMoves) {
        return nullptr;
    } else if(validMoves->empty()) {
        delete validMoves;
        return nullptr;
    } else {
        return validMoves;
    }
}

ChessMoves* ChessBoard::getValidPawnMoves(ChessPiece* piece, ChessMoveTypeOpt opts)
{
    ChessMoves* moves = new ChessMoves;
    Player player = piece->getOwner();
    IBP pos = piece->getIBPos();

    if(player == WHITE){

        bool topMove = pushNormalMove(moves, piece, {pos.row-1, pos.col}); //push top move

        //if pawn in starting row (6) and top square is empty, push double advance move
        if(pos.row == 6 && topMove) {
            pushNormalMove(moves, piece, {pos.row-2, pos.col});
        }

        pushCapture(moves, piece, {pos.row-1, pos.col-1}); //push top left capture
        pushCapture(moves, piece, {pos.row-1, pos.col+1}); //push top right capture

    } else {

        bool bottom = pushNormalMove(moves, piece, {pos.row+1, pos.col}); //push bottom move

        //if pawn in starting row (1) and bottom square is empty, push double advance move
        if(pos.row == 1 && bottom) {
            pushNormalMove(moves, piece, {pos.row+2, pos.col});
        }


        pushCapture(moves, piece, {pos.row+1, pos.col-1}); //push bottom left capture
        pushCapture(moves, piece, {pos.row+1, pos.col+1}); //push bottom right capture
    }

    //if in last row, make each move a promotion (automatic queen promotion for now)
    if (opts.allowPromotion) {
        if((player == WHITE && pos.row == 1) || (player == BLACK && pos.row == 6)) {
            for(ChessMoves::iterator it = moves->begin(); it != moves->end(); ++it) {
                ChessMove move = *it;
                *it = move + 'q';
            }
        }
    }

    //check for en passant moves
    if(opts.allowEnPassant) {
        bool enPassantTopRight = (pos.row-1) == opts.enPassantPos.row && (pos.col+1) == opts.enPassantPos.col;
        bool enPassantTopLeft = (pos.row-1) == opts.enPassantPos.row && (pos.col-1) == opts.enPassantPos.col;
        bool enPassantBottomRight = (pos.row+1) == opts.enPassantPos.row && (pos.col+1) == opts.enPassantPos.col;
        bool enPassantBottomLeft = (pos.row+1) == opts.enPassantPos.row && (pos.col-1) == opts.enPassantPos.col;

        if(player == WHITE && (enPassantTopRight || enPassantTopLeft)) {
            pushNormalMove(moves, piece, opts.enPassantPos);
        } else if(player == BLACK && (enPassantBottomRight || enPassantBottomLeft)) {
            pushNormalMove(moves, piece, opts.enPassantPos);
        }
    }

    if(moves->size() > 0) {
        return moves;
    } else {
        delete moves;
        return nullptr;
    }
}

ChessMoves* ChessBoard::getValidKingMoves(ChessPiece* piece, ChessMoveTypeOpt opts)
{
    ChessMoves* moves = new ChessMoves;
    IBP pos = piece->getIBPos();

    pushMove(moves, piece, {pos.row, pos.col-1});   //push left move
    pushMove(moves, piece, {pos.row, pos.col+1});   //push right move
    pushMove(moves, piece, {pos.row-1, pos.col});   //push top move
    pushMove(moves, piece, {pos.row-1, pos.col-1}); //push topLeft move
    pushMove(moves, piece, {pos.row-1, pos.col+1}); //push topRight move
    pushMove(moves, piece, {pos.row+1, pos.col});   //push bottom move
    pushMove(moves, piece, {pos.row+1, pos.col-1}); //push bottomLeft move
    pushMove(moves, piece, {pos.row+1, pos.col+1}); //push bottomRight move


    //check short castle
    if(opts.allowShortCastle) {
        ChessPiece* fPiece = getPiece({pos.row, 5});
        ChessPiece* gPiece = getPiece({pos.row, 6});
        if(!fPiece && !gPiece) {
            pushNormalMove(moves, piece, {pos.row, 6});
        }
    }

    //check long castle
    if(opts.allowLongCastle) {
        ChessPiece* bPiece = getPiece({pos.row, 1});
        ChessPiece* cPiece = getPiece({pos.row, 2});
        ChessPiece* dPiece = getPiece({pos.row, 3});

        if(!bPiece && !cPiece && !dPiece) {
            pushNormalMove(moves, piece, {pos.row, 2});
        }
    }

    if(moves->size() > 0) {
        return moves;
    } else {
        delete moves;
        return nullptr;
    }
}

ChessMoves* ChessBoard::getValidKnightMoves(ChessPiece* piece)
{
    ChessMoves* moves = new ChessMoves;
    IBP pos = piece->getIBPos();

    pushMove(moves, piece, {pos.row-2, pos.col+1});   //push top-right hook move
    pushMove(moves, piece, {pos.row-1, pos.col+2});   //push right-top hook move
    pushMove(moves, piece, {pos.row+1, pos.col+2});   //push right-bottom hook move
    pushMove(moves, piece, {pos.row+2, pos.col+1}); //push bottom-right hook move
    pushMove(moves, piece, {pos.row+2, pos.col-1}); //push bottom-left hook move
    pushMove(moves, piece, {pos.row+1, pos.col-2});   //push left-bottom hook move
    pushMove(moves, piece, {pos.row-1, pos.col-2}); //push left-top hook move
    pushMove(moves, piece, {pos.row-2, pos.col-1}); //push top-left hook move

    if(moves->size() > 0) {
        return moves;
    } else {
        delete moves;
        return nullptr;
    }
}

ChessMoves* ChessBoard::getValidRookMoves(ChessPiece* piece)
{
    ChessMoves* moves = new ChessMoves;
    pushStraightMoves(moves, piece);

    if(moves->size() > 0) {
        return moves;
    } else {
        delete moves;
        return nullptr;
    }
}

ChessMoves* ChessBoard::getValidBishopMoves(ChessPiece* piece)
{
    ChessMoves* moves = new ChessMoves;
    pushDiagonalMoves(moves, piece);

    if(moves->size() > 0) {
        return moves;
    } else {
        delete moves;
        return nullptr;
    }
}

ChessMoves* ChessBoard::getValidQueenMoves(ChessPiece* piece)
{
    ChessMoves* moves = new ChessMoves;
    pushStraightMoves(moves, piece);
    pushDiagonalMoves(moves, piece);

    if(moves->size() > 0) {
        return moves;
    } else {
        delete moves;
        return nullptr;
    }
}

bool ChessBoard::pushMove(ChessMoves* moves,  ChessPiece* srcPiece, IBP dst)
{
    //check if invalid dst
    if(dst.row < MIN_ROW_INDEX || dst.row > MAX_ROW_INDEX) {
        return false;
    }

    if(dst.col < MIN_COL_INDEX || dst.col > MAX_COL_INDEX) {
        return false;
    }

    //check if dst is empty or enemy piece
    ChessPiece* dstPiece = getPiece(dst);
    if(!dstPiece || dstPiece->getOwner() != srcPiece->getOwner()) {
        moves->push_back(createMove(srcPiece->getIBPos(), dst));
        return true;
    }
    return false;
}

bool ChessBoard::pushCapture(ChessMoves* moves, ChessPiece* srcPiece, IBP dst)
{
    //check if invalid dst
    if(dst.row < MIN_ROW_INDEX || dst.row > MAX_ROW_INDEX) {
        return false;
    }

    if(dst.col < MIN_COL_INDEX || dst.col > MAX_COL_INDEX) {
        return false;
    }

    //check if dst is enemy piece
    ChessPiece* dstPiece = getPiece(dst);
    if(dstPiece && dstPiece->getOwner() != srcPiece->getOwner()) {
        moves->push_back(createMove(srcPiece->getIBPos(), dst));
        return true;
    }
    return false;
}

bool ChessBoard::pushNormalMove(ChessMoves* moves, ChessPiece* srcPiece, IBP dst)
{
    //check if invalid dst
    if(dst.row < MIN_ROW_INDEX || dst.row > MAX_ROW_INDEX) {
        return false;
    }

    if(dst.col < MIN_COL_INDEX || dst.col > MAX_COL_INDEX) {
        return false;
    }

    //check if dst is empty
    ChessPiece* dstPiece = getPiece(dst);
    if(!dstPiece) {
        moves->push_back(createMove(srcPiece->getIBPos(), dst));
        return true;
    }
    return false;
}

bool ChessBoard::pushMoveHelper(ChessMoves* moves, ChessPiece* srcPiece, IBP dst)
{
    ChessPiece* dstPiece = getPiece(dst);
    if(dstPiece) {
        if(dstPiece->getOwner() != srcPiece->getOwner()) {
            //if dst is enemy piece, push capture
            moves->push_back(createMove(srcPiece->getIBPos(), dst));
        }
        return true; //end search
    } else {
        //if dst is empty -> push move, continue search
        moves->push_back(createMove(srcPiece->getIBPos(), dst));
        return false;
    }
}

void ChessBoard::pushStraightMoves(ChessMoves* moves, ChessPiece* srcPiece)
{
    IBP src = srcPiece->getIBPos();

    //check down col
    for(int i = src.row+1; i<=MAX_ROW_INDEX; i++){
        if(pushMoveHelper(moves, srcPiece, {i, src.col}))
            break;
    }

    //check up col
    for(int i = src.row-1; i>=MIN_ROW_INDEX; i--){
        if(pushMoveHelper(moves, srcPiece, {i, src.col}))
            break;
    }

    //check right row
    for(int j = src.col+1; j<=MAX_COL_INDEX; j++){
        if(pushMoveHelper(moves, srcPiece, {src.row, j}))
            break;
    }

    //check left row
    for(int j = src.col-1; j>=MIN_COL_INDEX; j--){
        if(pushMoveHelper(moves, srcPiece, {src.row, j}))
            break;
    }
}

void ChessBoard::pushDiagonalMoves(ChessMoves* moves, ChessPiece* srcPiece)
{
    IBP src = srcPiece->getIBPos();

    //check bottom right diagonal
    for(int i = src.row+1, j = src.col+1; i<=MAX_ROW_INDEX && j<=MAX_COL_INDEX; i++, j++){
        if(pushMoveHelper(moves, srcPiece, {i, j}))
            break;
    }

    //check top left diagonal
    for(int i = src.row-1, j = src.col-1; i>=MIN_ROW_INDEX && j>=MIN_COL_INDEX; i--, j--){
        if(pushMoveHelper(moves, srcPiece, {i, j}))
            break;
    }

    //check top right diagonal
    for(int i = src.row-1, j = src.col+1; i>=MIN_ROW_INDEX && j<=MAX_COL_INDEX; i--, j++){
        if(pushMoveHelper(moves, srcPiece, {i, j}))
            break;
    }

    //check bottom left diagonal
    for(int i = src.row+1, j = src.col-1; i<=MAX_ROW_INDEX && j>=MIN_COL_INDEX; i++, j--){
        if(pushMoveHelper(moves, srcPiece, {i, j}))
            break;
    }
}

ChessMove ChessBoard::createMove(IBP src, IBP dst, bool isPromotion, PieceType promotiontType)
{
    ChessMove res = "";
    ABP srcABP = BoardPosition::tranlateIBPoABP(src);
    ABP dstABP = BoardPosition::tranlateIBPoABP(dst);
    res = srcABP + dstABP;

    return res;
}
