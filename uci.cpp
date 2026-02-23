#include "uci.h"

uci::uci() {
	board = Board(true); 
}

void uci::loop() {
    std::string line;
    while (isRunning && std::getline(std::cin, line)) {
        handleCommand(line);
    }
}

void uci::handleCommand(std::string line) {
    std::istringstream iss(line);
    std::string token;
    iss >> token;

    if (token == "uci")
        uciOk();
    else if (token == "isready")
        isReady();
    else if (token == "go")
        go(iss);
}

void uci::uciOk() {
    std::cout << "id name Noobie 1.0\n";
    std::cout << "id author Gonzalo\n";
    std::cout << "uciok\n";
}

void uci::isReady() {
	// Aquí podrías realizar cualquier inicialización necesaria antes de que el motor esté listo para recibir comandos.
    std::cout << "readyok\n";
}

void uci::go(std::istringstream& iss) {
    std::string token;
    int depth = 0;
    while (iss >> token) {
        if (token == "depth") 
            iss >> depth;
        else if (token == "perft")
            iss >> depth;
		    benchmark::perftDivide(Board(true), depth);
    }
    // Aquí es donde ejecutarías tu función de búsqueda con la profundidad especificada.
    // Por ejemplo:
    // Move bestMove = searchBestMove(depth);
	// std::cout << "bestmove " << bestMove.toString() << "\n";
}