#include <algorithm>
#include <map>
#include "chessboard.h"
#include "chesspiece.h"

bool ChessBoard::performMove(ChessMove move)
{
    IBP src = BoardPosition::getMoveSrcIBP(move);
    IBP dst = BoardPosition::getMoveDstIBP(move);
    ChessPiece* captured = movePiece(src, dst);
    if(captured != nullptr) {
        captured->setCaptured(true);
    }

    return true;
}

bool ChessBoard::isMoveLegal(ChessMove move, bool checkIfValid)
{
    return true;
}

bool ChessBoard::isMoveValid(ChessMove move)
{
    return true;
}

bool ChessBoard::isMoveCapture(ChessMove move)
{
    IBP dst = BoardPosition::getMoveDstIBP(move);
    return (getPiece(dst) != nullptr);
}

ChessMoves* ChessBoard::getLegalMoves(ChessPiece* piece)
{
    ChessMoves* validMoves;
    switch(piece->getType()) {
        case KING:
        case QUEEN:
        case ROOK:
        case BISHOP:
        case KNIGHT:
            return nullptr;
        case PAWN:
            return validMoves = getValidPawnMoves(piece);
    }

    if(validMoves == nullptr) {
        //if no valid moves, return nullptr
        return nullptr;
    }

    ChessMoves* legalMoves = new ChessMoves;

    //filter out illegal moves (i.e. moves that put our king in check)
    for(ChessMoves::iterator it = legalMoves->begin(); it != legalMoves->end(); ++it) {
        ChessMove move = *it;
        if(isMoveLegal(move, false)) {
            legalMoves->push_back(move);
        }
    }

    //Using copy_if to filter
//    std::copy_if (legalMoves->begin(), legalMoves->end(), std::back_inserter(validMoves), &ChessBoard::isValidMove);

    delete validMoves;

    //return legalMoves unlesss it's empty
    if(!legalMoves->empty()) {
        return legalMoves;
    } else {
        delete legalMoves;
        return nullptr;
    }
}

ChessMoves* ChessBoard::getValidPawnMoves(ChessPiece* piece)
{
    ChessMoves* moves = new ChessMoves;
    Player player = piece->getOwner();
    IBP pos = piece->getIBPos();

    if(player == WHITE){
        IBP top = {pos.row-1, pos.col};
        IBP topLeft = {pos.row-1, pos.col-1};
        IBP topRight = {pos.row-1, pos.col+1};
        IBP twoTop = {pos.row-2, pos.col};

        //ignore if in row 0
        if(pos.row == 0) {
            delete moves;
            return nullptr;
        }

        //if no piece in top pos -> push move
        ChessPiece* topPiece = getPiece(top);
        if(!topPiece) {
            moves->push_back(createMove(pos, top));
        }

        //if pawn not in leftmost column, check top left capture
        if(pos.col-1 >= MIN_COL_INDEX) {
            //if enemy piece in top left pos -> push capture
            ChessPiece* topLeftPiece = getPiece(topLeft);
            if(topLeftPiece && topLeftPiece->getOwner() != player) {
                moves->push_back(createMove(pos, topLeft));
            }
        }

        //if pawn not in rightmost column, check top right capture
        if(pos.col+1 <= MAX_COL_INDEX) {
            //if enemy piece in top right pos -> push capture
            ChessPiece* topRightPiece = getPiece(topRight);
            if(topRightPiece && topRightPiece->getOwner() != player) {
                moves->push_back(createMove(pos, topRight));
            }
        }

        //if pawn in starting row (6), check double advance
        if(pos.row == 6) {
            //if no piece in two top pos -> push move
            ChessPiece* twoTopPiece = getPiece(twoTop);
            if(!twoTopPiece) {
                moves->push_back(createMove(pos, twoTop));
            }
        }
    } else {
        IBP bottom = {pos.row+1, pos.col};
        IBP bottomLeft = {pos.row+1, pos.col-1};
        IBP bottomRight = {pos.row+1, pos.col+1};
        IBP twoBottom = {pos.row+2, pos.col};

        //ignore if in row 7
        if(pos.row == 7) {
            delete moves;
            return nullptr;
        }

        //if no piece in bottom pos -> push move
        ChessPiece* bottomPiece = getPiece(bottom);
        if(!bottomPiece) {
            moves->push_back(createMove(pos, bottom));
        }

        //if pawn not in leftmost column, check bottom left capture
        if(pos.col-1 >= MIN_COL_INDEX) {
            //if enemy piece in bottom left pos -> push capture
            ChessPiece* bottomLeftPiece = getPiece(bottomLeft);
            if(bottomLeftPiece && bottomLeftPiece->getOwner() != player) {
                moves->push_back(createMove(pos, bottomLeft));
            }
        }

        //if pawn not in rightmost column, check bottom right capture
        if(pos.col+1 <= MAX_COL_INDEX) {
            //if enemy piece in bottom right pos -> push capture
            ChessPiece* bottomRightPiece = getPiece(bottomRight);
            if(bottomRightPiece && bottomRightPiece->getOwner() != player) {
                moves->push_back(createMove(pos, bottomRight));
            }
        }

        //if pawn in starting row (1), check double advance
        if(pos.row == 1) {
            //if no piece in two top pos -> push move
            ChessPiece* twoBottomPiece = getPiece(twoBottom);
            if(!twoBottomPiece) {
                moves->push_back(createMove(pos, twoBottom));
            }
        }
    }


    if(moves->size() > 0) {
        return moves;
    } else {
        delete moves;
        return nullptr;
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
