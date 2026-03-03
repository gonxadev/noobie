#pragma once
#include <atomic>
#include <string>
#include <sstream>
#include <thread>
#include "Board.h"
#include "perft.h"
#include "Evaluator.h"

class uci {
public:
    uci();
    std::atomic<bool> isRunning{ true };
    std::atomic<bool> searchAborted{ false };
    std::thread searchThread;
    void loop();
private:
    Board board;
    void handleCommand(std::string line);
    void uciOk();
    void isReady();
    void go(std::istringstream& iss);
    void quit();
	void search(int maxDepth, int timeForMove, int maxNodes, bool infinite);
};