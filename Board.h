#pragma once
#include <stdint.h> // uint64_t
#include <iostream>
#include <stack>
#include <vector>

struct Move {
    int from;
    int to;
    int piece;
    int promotion;
    bool operator==(const Move& other) const {
        return from == other.from && to == other.to && promotion == other.promotion;
    }
};

struct BoardState {
    uint64_t pieces[12];
    bool whiteToMove;
    bool whiteCastleLeft, whiteCastleRight;
    bool blackCastleLeft, blackCastleRight;
    Move lastMove;
    bool stalemate;
    int moves;
};

class Board
{
private:
    uint64_t pieces[12];

    // Castling
    bool blackCastleLeft = true;
    bool blackCastleRight = true;
    bool whiteCastleLeft = true;
    bool whiteCastleRight = true;

    int moves = 0;

    bool whiteToMove = true;

    bool stalemate = false;

    bool checkmate = false;

    Move lastMove ;

    std::stack<BoardState> history;

public:
    // pieces array indexes
    static const short W_PAWN = 0;
    static const short W_BISHOP = 1;
    static const short W_KNIGHT = 2;
    static const short W_TOWER = 3;
    static const short W_QUEEN = 4;
    static const short W_KING = 5;
    static const short B_PAWN = 6;
    static const short B_BISHOP = 7;
    static const short B_KNIGHT = 8;
    static const short B_TOWER = 9;
    static const short B_QUEEN = 10;
    static const short B_KING = 11;

    // color indexes 
    static const short WHITE = 0;
    static const short BLACK = 1;

    // board bounds
    static const uint64_t left = 9259542123273814144ULL;
    static const uint64_t right = 72340172838076673ULL;
    static const uint64_t top = 255ULL << 56;
    static const uint64_t bottom = 255ULL;

    Board();

    Board(bool init);

    Board(uint64_t w_pawn, uint64_t w_bishop, uint64_t w_knight, uint64_t w_tower, uint64_t w_king, uint64_t w_queen,
          uint64_t b_pawn, uint64_t b_bishop, uint64_t b_knight, uint64_t b_tower, uint64_t b_king, uint64_t b_queen);

    void printBitboard(uint64_t bitboard);
    bool makeMove(Move move);              
    void unmakeMove();                     
    void print();

    bool getBlackCastle() const; // Gets a boolean that indicates if castling is possible for black
    bool getLeftBlackCastle() const;
    bool getRightBlackCastle() const;
    bool getWhiteCastle() const; // Gets a boolean that indicates if castling is possible for white
    bool getLeftWhiteCastle() const;
    bool getRightWhiteCastle() const;
    bool isWhiteToMove() const;  // Gets a boolean that indicates if white is making the next move
    void setWhiteToMove(bool value);

    int getMoves();

    const uint64_t(&getPieces() const)[12];

    Move getLastMove() const;

    int getBoardIndexFromMoveGenerator(int indexFromMoveGenerator) const; // Devuelve el índice con el que indexar la posición de una pieza a partir del índice de MoveGenerator

    static bool isSameDiagonal(int from, int to);
    static bool isKnigthMove(int from, int to);
    static bool isSameRank(int from, int to);
    static bool isSameColumn(int from, int to);

    static bool isHorizontalPathClear(Move m, uint64_t occupied);
    static bool isVerticalPathClear(Move m, uint64_t occupied);
    static bool isDiagonalPathClear(Move m, uint64_t occupied);
    static bool isSquareUnderAttack(const Board& board, int square, int attackerColor);


    static uint64_t generatePawnAttacks(uint64_t pawns, int color);
    static uint64_t generateBishopAttacks(uint64_t bishops, uint64_t opposition, uint64_t occupied);
    static uint64_t generateKnightAttacks(uint64_t knight, uint64_t opposition, uint64_t occupied);
    static uint64_t generateTowerAttacks(uint64_t towers, uint64_t opposition, uint64_t occupied);
    static uint64_t generateQueenAttacks(uint64_t queens, uint64_t opposition, uint64_t occupied);
    static uint64_t generateKingAttacks(uint64_t king, uint64_t opposition, uint64_t occupied);

    static bool isKingInCheck(const Board& board, int kingColor);
    static bool isLegal(const Board& board, Move move, int color);

    void filterMoves(const Board& board, std::vector<Move>& moves, int color);

    /*
    * @brief
    * Receives a 64-bit integer and extracts a specific bit, which is
    * returned as a 64-bit integer with that bit set in the same position.
    *
    * @param board
    * The integer from which the bit will be extracted.
    *
    * @param from
    * Position of the bit to extract, counting from the least significant bit.
    *
    * @returns
    * A 64-bit integer with only the specified bit set.
    */
    static uint64_t getBit(uint64_t board, int from);

    /*
    * @brief
    * Sets a bit in an integer to 1 and returns the result.
    *
    * @param board
    * The 64-bit integer to be modified.
    *
    * @param to
    * Index of the bit to set to 1, counting from the least significant bit.
    *
    * @returns
    * The integer with the specified bit set to 1.
    *
    * @details
    * If the specified bit is already 1, the value remains unchanged.
    */
    static uint64_t setBit(uint64_t board, int to);

    /*
    * @brief
    * Receives a 64-bit integer, clears a specific bit and returns it.
    *
    * @param board
    * The 64-bit integer to be modified.
    *
    * @param to
    * Position of the bit to be cleared, counting from the least significant bit.
    *
    * @returns
    * The integer with the specified bit cleared.
    */
    static uint64_t clearBit(uint64_t board, int to);

    int getBitFromBoard(int pieceIndex, int from);
    void setBitOfBoard(int pieceIndex, int to);
    void clearBitOfBoard(int pieceIndex, int to);

    // Gets the position of a type of piece using the color and pieces indexes defined in Board
    uint64_t getBitboardFromType(int pieceType) const;

    // Gets the position of all white pieces in a single bitboard
    uint64_t getWhiteBitBoard() const;

    // Gets the position of all black pieces in a single bitboard
    uint64_t getBlackBitBoard() const;

    // Gets the position of all pieces in a single bitboard
    uint64_t getOccupiedBitBoard() const;

    bool isStalemate() const;

    void setStalemate(bool value);

    Board clone() const;
};