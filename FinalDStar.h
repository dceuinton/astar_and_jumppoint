#ifndef FINALDSTAR_H
#define FINALDSTAR_H

#include <vector>
#include <stdlib.h>
#include <iostream>
#include <math.h>

#include "gridworld.h"
#include "globalVariables.h"
#include "priorityQueue.h"

// class GridWorld; // Forward declaration

class FinalDStar {
public:
	FinalDStar(int rows, int cols, unsigned int theHeuristic);

	void init(int startX, int startY, int goalX, int goalY);
	void init(GridWorld &gWorld);
	// calculateKeys
	// updateVertex
	// computeShortestPath
	// main - so to speak	
	
	// My helper functions
	void printMaze();

	friend void copyMazeToDisplayMap(GridWorld &gWorld, FinalDStar *fds);
	friend void copyDisplayMapToMaze(GridWorld &gWorld, FinalDStar *fds);

private:
	int rows, cols;
	vector<vector<vertex>> maze;
	priorityQueue mQ;

	vertex* km;
	vertex* k_old;

	unsigned int HEURISTIC = 0; 
	Coordinates startCoord;
	Coordinates goalCoord;

	double calculateH(int x, int y);
};

#endif