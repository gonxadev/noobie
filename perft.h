#include "Board.h"
#include "MoveGenerator.h"

class benchmark {
public:
	static uint64_t perft(Board& board, int depth) {
        if (depth == 0)
            return 1ULL;

        uint64_t total_nodes = 0;
		int color = board.isWhiteToMove() ? Board::WHITE : Board::BLACK;
        std::vector<Move> moves = board.generateLegalMoves(color); // pseudolegales

        for (const Move& move : moves) {
            board.makeMove(move);
			uint64_t nodes = perft(board, depth - 1);

            total_nodes += nodes;
            board.unmakeMove();
        }
        return total_nodes;
	}

    static uint64_t perftDivide(Board& board, int depth) {
        MoveGenerator moveGen;
        if (depth == 0)
            return 1ULL;

        uint64_t total_nodes = 0;
        int color = board.isWhiteToMove() ? Board::WHITE : Board::BLACK;
        std::vector<Move> moves = board.generateLegalMoves(color); // pseudolegales
        
        for (const Move& move : moves) {
            board.makeMove(move);
            uint64_t nodes = perft(board, depth - 1);
            
            std::cout << indexToSquare(move.from) << indexToSquare(move.to) << ": " << nodes << std::endl;
            
            total_nodes += nodes;
            board.unmakeMove();  
        }
        return total_nodes;
	}
    static std::string indexToSquare(int index) {
        int file = index % 8;
        int rank = index / 8;
        std::string result;
        result += static_cast<char>('a' + file);
        result += static_cast<char>('1' + rank);
        return result;
    }
};