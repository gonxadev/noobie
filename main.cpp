#include "uci.h"
int main(int argc, char** argv) {
    /*
    Board board(true);
    int depth = 4;
    uint64_t nodes = benchmark::perftDivide(board, depth);
	std::cout << "Total nodes at depth " << depth << ": " << nodes << std::endl;
    */
	uci uciEngine;
	uciEngine.loop();
    return 0;
}