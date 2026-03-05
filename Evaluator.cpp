#include "Evaluator.h"

MoveGenerator moveGen;

Move Evaluator::bestMove;

// Opening position tables knights.
const static int KNIGHT_PST[64] = {
        -50,-40,-30,-30,-30,-30,-40,-50,
        -40,-20,  0,  0,  0,  0,-20,-40,
        -30,  0, 10, 15, 15, 10,  0,-30,
        -30,  5, 15, 20, 20, 15,  5,-30,
        -30,  0, 15, 20, 20, 15,  0,-30,
        -30,  5, 10, 15, 15, 10,  5,-30,
        -40,-20,  0,  5,  5,  0,-20,-40,
        -50,-40,-30,-30,-30,-30,-40,-50,
};

// Opening position tables for bishops. For black this table needs to be mirrored vertically with mirror, since the perspective is different.
const static int BISHOP_PST_OPENING[64] = {
    -20,-10,-10,-10,-10,-10,-10,-20,
    -10,  0,  0,  0,  0,  0,  0,-10,
    -10,  0,  5, 10, 10,  5,  0,-10,
    -10,  5,  5, 10, 10,  5,  5,-10,
    -10,  0, 10, 10, 10, 10,  0,-10,
    -10, 10, 10, 10, 10, 10, 10,-10,
    -10,  5,  0,  0,  0,  0,  5,-10,
    -20,-10,-10,-10,-10,-10,-10,-20,
};

static uint64_t mirror(uint64_t bb) {
    bb = ((bb >> 8) & 0x00FF00FF00FF00FFULL) | ((bb & 0x00FF00FF00FF00FFULL) << 8);
    bb = ((bb >> 16) & 0x0000FFFF0000FFFFULL) | ((bb & 0x0000FFFF0000FFFFULL) << 16);
    bb = ((bb >> 32)) | ((bb) << 32);
    return bb;
}

static int bitScanForward(uint64_t bb) {
    unsigned long index;
    _BitScanForward64(&index, bb);
    return (int)index;
}

static int mirrorSquare(int sq) {
    return sq ^ 56; // invierte la fila
}

SearchResult Evaluator::negamax(Board& board, int depth, int alpha, int beta) {
    if (depth == 0 || board.isStalemate()) {
        int eval = evaluate(board);
        return SearchResult(Move{}, (board.isWhiteToMove() ? eval : -eval));
    }

    SearchResult result;
    result.score = -100000; 
	int colorToMove = (board.isWhiteToMove() ? Board::WHITE : Board::BLACK);

    std::vector<Move> movimientos = moveGen.generateMoves(board, colorToMove);

    for (const Move& movimiento : movimientos) {
        if (Board::isLegal(board, movimiento, colorToMove) && board.makeMove(movimiento)) {
            SearchResult hijo = negamax(board, depth - 1, -beta, -alpha);
            int valor = -hijo.score;
            board.unmakeMove();

            if (valor > result.score) {
                result.score = valor;
                result.bestMove = movimiento;
            }

            if (result.score > alpha) {
                alpha = result.score;
            }

            if (alpha >= beta) {
                break;
            }
        }
    }

    return result;
}

int Evaluator::evaluate(Board& board) {
    int score = 0;

    // Sumar piezas blancas
    score += (int)__popcnt64(board.getBitboardFromType(Board::W_PAWN)) * PAWN_VALUE;
    score += (int)__popcnt64(board.getBitboardFromType(Board::W_KNIGHT)) * KNIGHT_VALUE;
    score += (int)__popcnt64(board.getBitboardFromType(Board::W_BISHOP)) * BISHOP_VALUE;
    score += (int)__popcnt64(board.getBitboardFromType(Board::W_TOWER)) * ROOK_VALUE;
    score += (int)__popcnt64(board.getBitboardFromType(Board::W_QUEEN)) * QUEEN_VALUE;

    // Restar piezas negras 
    score -= (int)__popcnt64(board.getBitboardFromType(Board::B_PAWN)) * PAWN_VALUE;
    score -= (int)__popcnt64(board.getBitboardFromType(Board::B_KNIGHT)) * KNIGHT_VALUE;
    score -= (int)__popcnt64(board.getBitboardFromType(Board::B_BISHOP)) * BISHOP_VALUE;
    score -= (int)__popcnt64(board.getBitboardFromType(Board::B_TOWER)) * ROOK_VALUE;
    score -= (int)__popcnt64(board.getBitboardFromType(Board::B_QUEEN)) * QUEEN_VALUE;

    if (board.getMoveCount()/2 < 10) { //Openig phase
        // Control del centro
        const uint64_t center = 0x0000001818000000ULL; // d4, e4, d5, e5
        score += (int)__popcnt64(board.getWhiteBitBoard() & center) * 20;
        score -= (int)__popcnt64(board.getBlackBitBoard() & center) * 20;

        // Control del centro por los caballos
        uint64_t w_knights = board.getBitboardFromType(Board::W_KNIGHT);
        while (w_knights) {
            int sq = bitScanForward(w_knights);
            score += KNIGHT_PST[sq];
            w_knights &= w_knights - 1; // quitar bit menos significativo
        }

        uint64_t b_knights = board.getBitboardFromType(Board::B_KNIGHT);
        while (b_knights) {
            int sq = bitScanForward(b_knights);
            score -= KNIGHT_PST[sq];
            b_knights &= b_knights - 1; 
        }

		// Desarrollo de los alfiles
        uint64_t w_bishops = board.getBitboardFromType(Board::W_BISHOP);
        while (w_bishops) {
            int sq = bitScanForward(w_bishops);
            score += BISHOP_PST_OPENING[sq];
			w_bishops &= w_bishops - 1; 
        }

		uint64_t b_bishops = board.getBitboardFromType(Board::B_BISHOP);
        while(b_bishops){
            int sq = mirrorSquare(bitScanForward(b_bishops));
			score -= BISHOP_PST_OPENING[sq];
			b_bishops &= b_bishops - 1;
        }
        
    }

    return score;
}

Move Evaluator::getBestMove() {
    return bestMove;
}

// Setter: asigna un nuevo mejor movimiento
void Evaluator::setBestMove(Move move) {
    bestMove = move;
}
