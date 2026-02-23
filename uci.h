#pragma once
#include <atomic>
#include <string>
#include <sstream>
#include "Board.h"
#include "perft.h"

class uci {
public:
    uci();
    std::atomic<bool> isRunning{ true };
    void loop();
private:
	Board board;
    void handleCommand(std::string line);
    void uciOk();
    void isReady();
    void go(std::istringstream& iss);
	void quit();
};