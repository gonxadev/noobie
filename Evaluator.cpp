#include "Evaluator.h"

MoveGenerator moveGen;

Move Evaluator::bestMove;

SearchResult Evaluator::negamax(Board& board, int depth, int alpha, int beta) {
    if (depth == 0 || board.isStalemate()) {
        int eval = evaluate(board);
        return SearchResult(Move{}, (board.isWhiteToMove() ? eval : -eval));
    }

    SearchResult result;
    result.score = -100000; 

    std::vector<Move> movimientos = moveGen.generateMoves(board, (board.isWhiteToMove() ? Board::WHITE : Board::BLACK));

    for (const Move& movimiento : movimientos) {
        board.makeMove(movimiento);
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

    // Control del centro
    const uint64_t center = 0x0000001818000000ULL; // d4, e4, d5, e5
    score += (int)__popcnt64(board.getWhiteBitBoard() & center) * 20;
    score -= (int)__popcnt64(board.getBlackBitBoard() & center) * 20;

    return score;
}

Move Evaluator::getBestMove() {
    return bestMove;
}

// Setter: asigna un nuevo mejor movimiento
void Evaluator::setBestMove(Move move) {
    bestMove = move;
}
