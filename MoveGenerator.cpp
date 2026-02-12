#include "MoveGenerator.h"

std::vector<Move> MoveGenerator::generateMoves(const Board& board, int color) 
{
    std::vector<Move> moves;
    generatePawnMoves(board, moves, color);
    generateBishopMoves(board, moves, color, false);
    generateKnightMoves(board, moves, color);
    generateTowerMoves(board, moves, color, false);
    generateQueenMoves(board, moves, color);
    generateKingMoves(board, moves, color);

    //filterMoves(board, moves, color);

    moves.shrink_to_fit();
    return moves;
}

void MoveGenerator::generatePawnMoves(const Board& board, std::vector<Move>& moves, int color) 
{
    if (color == WHITE) {
        generateWhitePawnsMoves(board, moves);
    } else {
        generateBlackPawnMoves(board, moves);
    }
}

void MoveGenerator::generateWhitePawnsMoves(const Board& board, std::vector<Move>& moves) 
{
    uint64_t w_pawns = board.getBitboardFromType(Board::W_PAWN);

    for (int from = 0; from < 64; from++) {
        uint64_t from_mask = Board::getBit(w_pawns, from);
        if (from_mask & w_pawns) {
            int from_rank = from / 8;
            int from_col = from % 8;
            
            Move fwd_1{ from, from + 8, PAWN, NULL_TYPE };
            moves.push_back(fwd_1);

            if (from_rank == 1) {
                Move fwd_2{ from, from + 16, PAWN, NULL_TYPE };
                moves.push_back(fwd_2);
            }
            if (from_col < 7) {
                Move right{ from, from + 9, PAWN, NULL_TYPE };
                moves.push_back(right);
            }
            if (from_col > 0) {
                Move left{ from, from + 7, PAWN, NULL_TYPE };
                moves.push_back(left);
            }
        }
    }
}

void MoveGenerator::generateBlackPawnMoves(const Board& board, std::vector<Move>& moves)
{
    uint64_t b_pawns = board.getBitboardFromType(board.B_PAWN);

    for (int from = 0; from < 64; from++) {
        uint64_t from_mask = Board::getBit(b_pawns, from);
        if (from_mask & b_pawns) {
            int from_rank = from / 8;
            int from_col = from % 8;

            Move fwd_1{ from, from - 8, PAWN, NULL_TYPE };
            moves.push_back(fwd_1);

            if (from_rank == 6) {
                Move fwd_2{ from, from - 16, PAWN, NULL_TYPE };
                moves.push_back(fwd_2);
            }

            if (from_col < 7) {
                Move right{ from, from - 7, PAWN, NULL_TYPE };
                moves.push_back(right);
            }

            if (from_col > 0) {
                Move left{ from, from - 9, PAWN, NULL_TYPE };
                moves.push_back(left);
            }
        }
    }
}

void MoveGenerator::generateBishopMoves(const Board& board, std::vector<Move>& moves, int color, bool forQueen) 
{
    uint64_t bishops;

    if (forQueen) {
        if (color == WHITE) {
            bishops = board.getBitboardFromType(Board::W_QUEEN);
        }
        else if (color == BLACK) {
            bishops = board.getBitboardFromType(Board::B_QUEEN);
        }
    } 
    else 
    {
        if (color == WHITE) {
            bishops = board.getBitboardFromType(Board::W_BISHOP);
        }
        else if (color == BLACK) {
            bishops = board.getBitboardFromType(Board::B_BISHOP);
        }
    }
    

    for (int from = 0; from < 64; from++) {
        uint64_t from_mask = 1ULL << from;
        if (from_mask & bishops) {
            for (int to = from + 9; to < 64; to += 9) {
                if (!Board::isSameDiagonal(from, to)) {
                    break;
                }
                Move m{ from, to, (forQueen ? QUEEN : BISHOP), NULL_TYPE };
                moves.push_back(m);
            }
            for (int to = from - 9; to >= 0; to -= 9) {
                if (!Board::isSameDiagonal(from, to)) {
                    break;
                }
                Move m{ from, to, (forQueen ? QUEEN : BISHOP), NULL_TYPE };
                moves.push_back(m);
            }
            for (int to = from + 7; to < 64; to += 7) {
                if (!Board::isSameDiagonal(from, to)) {
                    break;
                }
                Move m{ from, to, (forQueen ? QUEEN : BISHOP), NULL_TYPE };
                moves.push_back(m);
            }
            for (int to = from - 7; to >= 0; to -= 7) {
                if (!Board::isSameDiagonal(from, to)) {
                    break;
                }
                Move m{ from, to, (forQueen ? QUEEN : BISHOP), NULL_TYPE };
                moves.push_back(m);
            }
        }
    }
}

void MoveGenerator::generateKnightMoves(const Board& board, std::vector<Move>& moves, int color) 
{
    uint64_t knight;
    if (color == WHITE) {
        knight = board.getBitboardFromType(Board::W_KNIGHT);
    }
    else if (color == BLACK) {
        knight = board.getBitboardFromType(Board::B_KNIGHT);
    }

    for (int from = 0; from < 64; from++) {
        uint64_t from_mask= 1ULL << from;
        if (from_mask & knight) {
            int from_rank = from / 8;
            int from_col = from % 8;

            for (int move : {6, 10, 15, 17, -6, -10, -15, -17}) {
                int to = from + move;
                if (Board::isKnigthMove(from, to)) {
                    Move m{ from, to, KNIGHT, NULL_TYPE };
                    moves.push_back(m);
                }
            }

        }
    }
}

void MoveGenerator::generateTowerMoves(const Board& board, std::vector<Move>& moves, int color, bool forQueen) 
{
    uint64_t tower;

    if (forQueen) {
        if (color == WHITE) {
            tower = board.getBitboardFromType(Board::W_QUEEN);
        }
        else if (color == BLACK) {
            tower = board.getBitboardFromType(Board::B_QUEEN);
        }
    } else {
        if (color == WHITE) {
            tower = board.getBitboardFromType(Board::W_TOWER);
        }
        else if (color == BLACK) {
            tower = board.getBitboardFromType(Board::B_TOWER);
        }
    }
    

    for (int from = 0; from < 64; from++) {
        uint64_t from_mask = 1ULL << from;
        if (from_mask & tower) {
            // Upward moves
            for (int to = from + 8; to < 64; to+=8) {
                Move m{from, to, (forQueen ? QUEEN : TOWER), NULL_TYPE};
                moves.push_back(m);
            }
            // Downward moves
            for (int to = from - 8; to >= 0; to-=8) {
                Move m{ from, to, (forQueen ? QUEEN : TOWER), NULL_TYPE };
                moves.push_back(m);
            }
            // Right moves
            for (int to = from + 1; to < 64; to++) {
                int from_rank = from / 8;
                int to_rank = to / 8;
                if (from_rank == to_rank) {
                    Move m{from, to, (forQueen ? QUEEN : TOWER), NULL_TYPE };
                    moves.push_back(m);
                } else {
                    break;
                }
            }
            // Left moves
            for (int to = from - 1; to >= 0; to--) {
                int from_rank = from / 8;
                int to_rank = to / 8;
                if (from_rank == to_rank) {
                    Move m{ from, to, (forQueen ? QUEEN : TOWER), NULL_TYPE };
                    moves.push_back(m);
                }
                else {
                    break;
                }
            }
        }
    }
}

void MoveGenerator::generateQueenMoves(const Board& board, std::vector<Move>& moves, int color)
{
    generateBishopMoves(board, moves, color, true);
    generateTowerMoves(board, moves, color, true);
}

void MoveGenerator::generateKingMoves(const Board& board, std::vector<Move>& moves, int color)
{
    uint64_t king;
    if (color == WHITE) {
        king = board.getBitboardFromType(Board::W_KING);
    }
    else if (color == BLACK) {
        king = board.getBitboardFromType(Board::B_KING);
    }

    for (int from = 0; from < 64; from++) {
        uint64_t from_mask = 1ULL << from;
        if (from_mask & king) {
            int from_rank = from / 8;
            int from_col = from % 8;

            // Upwards
            if (from_rank <= 6) {
                int to = from + 8;
                Move m{ from, to, KING, NULL_TYPE };
                moves.push_back(m);
            }
            // Downwards
            if (from_rank >= 1) {
                int to = from - 8;
                Move m{ from, to, KING, NULL_TYPE };
                moves.push_back(m);
            }
            // Right
            if (from_col <= 6) {
                int to = from + 1;
                Move m{ from, to, KING, NULL_TYPE };
                moves.push_back(m);
            }
            // Left
            if (from_col >= 1) {
                int to = from - 1;
                Move m{ from, to, KING, NULL_TYPE };
                moves.push_back(m);
            }
            // Right - Up
            if ( from_rank <= 6 && from_col <= 7) {
                int to = from + 9;
                if (Board::isSameDiagonal(from, to)) {
                    Move m{ from, to, KING, NULL_TYPE };
                    moves.push_back(m);
                }
            }
            // Left - Up
            if (from_rank <= 6 && from_col >= 1) {
                int to = from + 7;
                if (Board::isSameDiagonal(from, to)) {
                    Move m{ from, to, KING, NULL_TYPE };
                    moves.push_back(m);
                }
            }
            // Right - Down
            if (from_rank >= 1 && from_col <= 6) {
                int to = from - 7;
                if (Board::isSameDiagonal(from, to)) {
                    Move m{ from, to, KING, NULL_TYPE };
                    moves.push_back(m);
                }
            }
            // Left - Down
            if (from_rank >= 1 && from_col >= 1) {
                int to = from - 9;
                if (Board::isSameDiagonal(from, to)) {
                    Move m{ from, to, KING, NULL_TYPE };
                    moves.push_back(m);
                }
            }
            // Enroque
            if ((color == Board::WHITE && from == 4) ||
                (color == Board::BLACK && from == 60)) {
                Move m1{ from, from + 2, KING, NULL_TYPE };
                Move m2{ from, from - 2, KING, NULL_TYPE };
                moves.push_back(m1);
                moves.push_back(m2);
            }
        }
    }
}