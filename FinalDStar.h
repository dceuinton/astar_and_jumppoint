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
	FinalDStar(int rows, int cols);

private:
	int rows, cols;
	vector<vector<FinalDStarCell>> maze;
};

#endif