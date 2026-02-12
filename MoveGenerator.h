#pragma once
#include <stdint.h>
#include <vector>
#include "Board.h"

class MoveGenerator {
public:
    static const uint64_t w_pawns_starting_pos = 255ULL << 8;
    static const uint64_t b_pawns_starting_pos = 255ULL << 48;
    static const int WHITE = 0;
    static const int BLACK = 1;
    static const int NULL_TYPE = -1;
    static const int PAWN = 0;
    static const int BISHOP = 1;
    static const int KNIGHT = 2;
    static const int TOWER = 3;
    static const int QUEEN = 4;
    static const int KING = 5;
public:
    std::vector<Move> generateMoves(const Board& board, int color);
    void filterMoves(const Board& board, std::vector<Move>& moves, int color);
private:
    static bool isSameDiagonal(int from, int to);
    static bool isKnigthMove(int from, int to);
    static bool isSameRank(int from, int to);
    static bool isSameColumn(int from, int to);
    static void generatePawnMoves(const Board& board, std::vector<Move>& moves, int color);
    static void generateWhitePawnsMoves(const Board& board, std::vector<Move>& moves);
    static void generateBlackPawnMoves(const Board& board, std::vector<Move>& moves);
    static void generateBishopMoves(const Board& board, std::vector<Move>& moves, int color, bool forQueen);
    static void generateKnightMoves(const Board& board, std::vector<Move>& moves, int color);
    static void generateTowerMoves(const Board& board, std::vector<Move>& moves, int color, bool forQueen);
    static void generateQueenMoves(const Board& board, std::vector<Move>& moves, int color);
    static void generateKingMoves(const Board& board, std::vector<Move>& moves, int color);
    // Métodos para la validación de movimientos
    static bool isPathClear(Move m, uint64_t occupied);
    static bool isHorizontalPathClear(Move m, uint64_t occupied);
    static bool isVerticalPathClear(Move m, uint64_t occupied);
    static bool isDiagonalClear(Move m, uint64_t occupied);
    static bool isSquareUnderAttack(const Board& board, int square, int attackerColor);
    // Métodos para la crear los ataques
    static uint64_t generatePawnAttacks(uint64_t pawns, int color);
    static uint64_t generateBishopAttacks(uint64_t bishops, uint64_t opposition, uint64_t occupied);
    static uint64_t generateKnightAttacks(uint64_t knight, uint64_t opposition, uint64_t occupied);
    static uint64_t generateTowerAttacks(uint64_t towers, uint64_t opposition, uint64_t occupied);
    static uint64_t generateQueenAttacks(uint64_t queens, uint64_t opposition, uint64_t occupied);
    static uint64_t generateKingAttacks(uint64_t king, uint64_t opposition, uint64_t occupied);
public:
    static bool isKingInCheck(const Board& board, int kingColor);
    static bool isLegal(const Board& board, Move move, int color);
};

