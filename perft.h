#include "Board.h"
#include "MoveGenerator.h"

class benchmark {
public:
	static uint64_t perft(Board& board, int depth) {
		MoveGenerator moveGen;
        if (depth == 0)
            return 1ULL;

        uint64_t total_nodes = 0;
		int color = board.isWhiteToMove() ? Board::WHITE : Board::BLACK;
        std::vector<Move> moves = moveGen.generateMoves(board, color); // pseudolegales

        for (const Move& move : moves) {

            if (!board.isLegal(board, move, color))
                continue;

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
        std::vector<Move> moves = moveGen.generateMoves(board, color); // pseudolegales
        
        for (const Move& move : moves) {
            
            if (!board.isLegal(board, move, color))
                continue;
            
            board.makeMove(move);
            uint64_t nodes = perft(board, depth - 1);
            
            std::cout << "{ From: " << move.from << " To: " << move.to << " Piece: " << move.piece << " Nodes: " << nodes << " Depth: " << depth << " }" << std::endl;
            
            total_nodes += nodes;
            board.unmakeMove();  
        }
        return total_nodes;
	}
};