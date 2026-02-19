#include <iostream>
#include "./Board.h"
#include "./perft.h"

int main(int argc, char** argv) {

    Board board(true);
    int depth = 5;
    uint64_t nodes = benchmark::perft(board, depth);
	std::cout << "Total nodes at depth " << depth << ": " << nodes << std::endl;
    return 0;
}