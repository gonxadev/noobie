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
    else if (token == "quit")
        quit();
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

    int wtime = 0, btime = 0, winc = 0, binc = 0;
    int movetime = 0, depth = 0, movestogo = 0, nodes = 0;
    bool infinite = false;

    while (iss >> token) {
        if (token == "wtime")          iss >> wtime;
        else if (token == "btime")     iss >> btime;
        else if (token == "winc")      iss >> winc;
        else if (token == "binc")      iss >> binc;
        else if (token == "movetime")  iss >> movetime;
        else if (token == "depth")     iss >> depth;
        else if (token == "movestogo") iss >> movestogo;
        else if (token == "nodes")     iss >> nodes;
        else if (token == "infinite")  infinite = true;
        else if (token == "perft")
            iss >> depth;
		    benchmark::perftDivide(Board(true), depth);
    }
    // calcular tiempo disponible para este movimiento
    int timeForMove; //= calculateTime(wtime, btime, winc, binc, movestogo, movetime, infinite);

    searchAborted = false;

    searchThread = std::thread([this, depth, timeForMove, nodes, infinite]() {
        search(depth == 0 ? 64 : depth, timeForMove, nodes, infinite);
        });
    searchThread.detach();
}

void uci::quit() {
    isRunning = false;
}

void uci::search(int maxDepth, int timeForMove, int maxNodes, bool infinite) {
    auto startTime = std::chrono::steady_clock::now();
    std::string bestMove;

    // iterative deepening
    for (int depth = 1; depth <= maxDepth; depth++) {

        if (searchAborted) break;

        // aquí iría tu alpha-beta/negamax
        SearchResult result = Evaluator::negamax(board, depth, -100000, 100000);

        if (searchAborted) break; // resultado parcial, no es fiable

        bestMove = result.bestMove.toString();

        // info que el GUI muestra en tiempo real
        //auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
        //    std::chrono::steady_clock::now() - startTime).count();

        std::cout << "info"
            << " depth " << depth
            << " score cp " << result.score
            //<< " nodes " << result.nodes
            //<< " time " << elapsed
            //<< " nps " << (elapsed > 0 ? result.nodes * 1000 / elapsed : 0)
            //<< " pv " << result.pv
            << "\n";

        // parar si ya agotamos el tiempo
        //if (!infinite && elapsed >= timeForMove) break;
    }

    std::cout << "bestmove " << (bestMove.empty() ? "0000" : bestMove) << "\n";
}