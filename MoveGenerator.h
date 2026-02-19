#pragma once
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
private:
    static void generatePawnMoves(const Board& board, std::vector<Move>& moves, int color);
    static void generateWhitePawnsMoves(const Board& board, std::vector<Move>& moves);
    static void generateBlackPawnMoves(const Board& board, std::vector<Move>& moves);
    static void generateBishopMoves(const Board& board, std::vector<Move>& moves, int color, bool forQueen);
    static void generateKnightMoves(const Board& board, std::vector<Move>& moves, int color);
    static void generateTowerMoves(const Board& board, std::vector<Move>& moves, int color, bool forQueen);
    static void generateQueenMoves(const Board& board, std::vector<Move>& moves, int color);
    static void generateKingMoves(const Board& board, std::vector<Move>& moves, int color);
};

