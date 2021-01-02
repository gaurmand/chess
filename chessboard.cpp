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
        case PAWN:
            validMoves = getValidPawnMoves(piece);
            break;
        case KING:
            validMoves = getValidKingMoves(piece);
            break;
        case KNIGHT:
            validMoves = getValidKnightMoves(piece);
            break;
        case QUEEN:
        case ROOK:
        case BISHOP:
            return nullptr;
    }

    if(validMoves == nullptr) {
        //if no valid moves, return nullptr
        return nullptr;
    }

    ChessMoves* legalMoves = new ChessMoves;

    //filter out illegal moves (i.e. moves that put our king in check)
    for(ChessMoves::iterator it = validMoves->begin(); it != validMoves->end(); ++it) {
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

        //ignore if in row 0
        if(pos.row == 0) {
            delete moves;
            return nullptr;
        }

        //if pawn in starting row (6), push double advance move
        if(pos.row == 6) {
            pushNormalMove(moves, piece, {pos.row-2, pos.col});
        }

        pushNormalMove(moves, piece, {pos.row-1, pos.col}); //push top move
        pushCapture(moves, piece, {pos.row-1, pos.col-1}); //push top left capture
        pushCapture(moves, piece, {pos.row-1, pos.col+1}); //push top right capture
    } else {

        //ignore if in row 7
        if(pos.row == 7) {
            delete moves;
            return nullptr;
        }

        //if pawn in starting row (1), push double advance move
        if(pos.row == 1) {
            pushNormalMove(moves, piece, {pos.row+2, pos.col});
        }

        pushNormalMove(moves, piece, {pos.row+1, pos.col}); //push bottom move
        pushCapture(moves, piece, {pos.row+1, pos.col-1}); //push bottom left capture
        pushCapture(moves, piece, {pos.row+1, pos.col+1}); //push bottom right capture
    }


    if(moves->size() > 0) {
        return moves;
    } else {
        delete moves;
        return nullptr;
    }
}

ChessMoves* ChessBoard::getValidKingMoves(ChessPiece* piece)
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
    }
    return true;
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
    }
    return true;
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
    }
    return true;
}

ChessMove ChessBoard::createMove(IBP src, IBP dst, bool isPromotion, PieceType promotiontType)
{
    ChessMove res = "";
    ABP srcABP = BoardPosition::tranlateIBPoABP(src);
    ABP dstABP = BoardPosition::tranlateIBPoABP(dst);
    res = srcABP + dstABP;

    return res;
}
