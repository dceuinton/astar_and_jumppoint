#ifndef FINALDSTAR_H
#define FINALDSTAR_H

#include <vector>
#include <stdlib.h>
#include <iostream>
#include <math.h>

#include "gridworld.h"
#include "globalVariables.h"

// class GridWorld; // Forward declaration

class FinalDStar {
public:
	FinalDStar(int rows, int cols, unsigned int theHeuristic);

	void init(int startX, int startY, int goalX, int goalY);



	double workOutH(int x, int y);
	void printMaze();

private:
	int rows, cols;
	vector<vector<vertex>> maze;

	vertex* km;
	vertex* k_old;

	unsigned int HEURISTIC = 0; 
	Coordinates startCoord;
	Coordinates goalCoord;
};

#endif