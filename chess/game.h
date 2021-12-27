#ifndef GAME_H
#define GAME_H

#include "common.h"
#include "position.h"
#include "move.h"

#include <map>

namespace Chess
{

class Game: public Position
{
public:
    Game(): Position() { update(); }
    Game(const FENState& state): Position(state) { update(); }
    void setFENState(const FENState& state);

    void performMove(const Move& move, bool checkLegal = true);
    void performMove(const BP& src, const BP& dst,
                     bool checkLegal = false);

    Move move(const BP& src, const BP& dst) const;
    bool isLegalMove(const BP& src, const BP& dst) const;
    MoveType moveType(const BP& src, const BP& dst) const;
    std::vector<Move> moves() const;
    std::vector<Move> moves(const BP& pos) const;

    bool isInCheck() const { return isInCheck_; }
    bool isComplete() const { return result_ != ResultType::Incomplete; }
    ResultType result() const { return result_; }

private:
    using Board::performMove;
    using State::setActivePlayer;
    using State::switchActivePlayer;

    const Move* findMove(const ANBP& src, const ANBP& dst) const;

    void executeMove(const BP& src, const BP& dst, MoveType type);
    bool executeDoubleAdvance(const BP& dst);
    void executeKingOrRookMove(const BP& src, const Piece* srcPiece);
    void executeEnPassant();
    void executePromotion(Piece* srcPiece);
    void executeLongCastle();
    void executeShortCastle();

    void update();
    void computeCandidateMoves(std::map<ANBP, std::vector<Move>>& map);
    void computeLegalMoves();
    bool isCheck();

    void computeKingMoves(Piece& piece, std::vector<Move>& moves);
    void computeQueenMoves(Piece& piece, std::vector<Move>& moves);
    void computeRookMoves(Piece& piece, std::vector<Move>& moves);
    void computeBishopMoves(Piece& piece, std::vector<Move>& moves);
    void computeKnightMoves(Piece& piece, std::vector<Move>& moves);
    void computePawnMoves(Piece& piece, std::vector<Move>& moves);
    bool pushValidMove(const BP& src,
                       const BP& dst,
                       std::vector<Move>& moves,
                       bool allowNormalMoves = true,
                       bool allowCaptures = true);
    void pushMoveHelper(const BP& src,
                        std::vector<Move>& moves,
                        bool checkStraights,
                        bool checkDiagonals);

    std::map<ANBP, std::vector<Move>> legalMoves_;
    std::map<ANBP, std::vector<Move>> checkMoves_;
    ResultType result_ = ResultType::Incomplete;
    bool isInCheck_ = false;
    int numLegalMoves_ = 0;
};

}; // namespace Chess

#endif // GAME_H
