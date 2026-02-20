#pragma once
#include <cassert>
#include "Board.h";
#include "perft.h";
class Tests {
	public:
		static void runAllTests();
		static void perftTest();
		static void perftTest(int depth);
};