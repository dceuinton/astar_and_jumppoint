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
	// updateVertex
	void updateVertex(vertex &v);
	bool computeShortestPath();
	void search(GridWorld &gWorld);
	
	// My helper functions
	vertex* getVertex(int x, int y);
	vertex* getStartVertex();
	vertex* getGoalVertex();
	void printMaze();
	void printPQ();
	void debug(const char *format, ...);

	friend void copyMazeToDisplayMap(GridWorld &gWorld, FinalDStar *fds);
	friend void copyDisplayMapToMaze(GridWorld &gWorld, FinalDStar *fds);

private:
	int rows, cols;
	vector<vector<vertex>> maze;
	PriorityQueue mQ;

	double km;
	double k_old[2];
	double mStartKey[2];

	unsigned int HEURISTIC = 0; 
	Coordinates startCoord;
	Coordinates goalCoord;
	vertex* s_last;
	vertex* s_start;
	vertex* s_goal;

	double calculateH(int x, int y);
	double calculateK2(vertex &v);
	double calculateK1(vertex &v);
	double* calculateKey(vertex &v);
	void calcKey(vertex &v, double *container);
	double* calculateStartKey();
	void setKey(vertex &v);
	bool isGoal(vertex &v);
	bool notGoal(vertex &v);
	bool isStart(vertex &v);
	bool notBlocked(vertex &v);
	double cost(vertex &start, vertex &finish);
	bool locallyConsistant(vertex &v);
	void insertToQ(vertex &v);
	bool compareKeys(double* lhs, double* rhs);
	bool isHidden(vertex &v);
	void block(vertex &v);

	double manhattan(int x1, int y1, int x2, int y2);
	double euclidean(int x1, int y1, int x2, int y2);
};

#endif