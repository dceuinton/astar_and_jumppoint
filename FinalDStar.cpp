#include "FinalDStar.h"

FinalDStar::FinalDStar(int rs, int cs, unsigned int theHeuristic) {
	rows = rs; cols = cs;
	HEURISTIC = theHeuristic;

	maze.resize(rows);
	for (int i = 0; i < rows; i++) {
		maze[i].resize(cols);
	}
}

void FinalDStar::init(int startX, int startY, int goalX, int goalY) {
	printf("init(Coordinates)\n");
	// Do the priority queue init
	mQ.clear();
	km = 0;

	startCoord.x = startX; startCoord.y = startY;
	goalCoord.x = goalX;   goalCoord.y = goalY;

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			maze[i][j].row = i;
			maze[i][j].col = j;
			maze[i][j].h = calculateH(j, i);
			maze[i][j].g   = INF;
			maze[i][j].rhs = INF;
			maze[i][j].key[0] = 0;
			maze[i][j].key[1] = 0;
		}
	}

	maze[goalY][goalX].rhs = 0;
	insertToQ(maze[goalY][goalX]);
}

void FinalDStar::init(GridWorld &gWorld) {
	printf("init(gWorld)\n");
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			maze[i][j].type = 1;
		}
	}
	copyDisplayMapToMaze(gWorld, this);
}

void FinalDStar::updateVertex(vertex &v) {
	double newRHS = INF;
	if (!isGoal(v)) {
		for (int i = 0; i < DIRECTIONS; i++) {
			int x = v.col + neighbours[i].x;
			int y = v.row + neighbours[i].y;
			if (notBlocked(maze[y][x])) {
				double rhs = maze[y][x].g + cost(v, maze[y][x]);
				if (rhs < newRHS) {
					newRHS = rhs;
				}
			}
		}
	}

	if (mQ.contains(&v)) {
		mQ.remove(&v);
	}

	if (!locallyConsistant(v)) {
		insertToQ(v);
	}
}

double FinalDStar::calculateH(int x, int y) {
	if (HEURISTIC == MANHATTAN) {  
		return manhattan(x, y, startCoord.x, startCoord.y);
	} else if (HEURISTIC == EUCLIDEAN) {
		return euclidean(x, y, startCoord.x, startCoord.y);
	} else {
		printf("ERROR: Undefined heuristic function!\n");
		exit(1);
	}

	return 0.0;
}

double FinalDStar::calculateK2(vertex &v) {
	return min(v.g, v.rhs);
}

double FinalDStar::calculateK1(vertex &v) {
	double retVal = calculateK2(v);
	retVal += v.h; 
	retVal += km;
	return retVal;
}

double* FinalDStar::calculateKey(vertex &v) {
	double *keys = new double(2);
	keys[0] = calculateK1(v);
	keys[1] = calculateK2(v);
	return keys;
}

void FinalDStar::setKey(vertex &v) {
	v.key[0] = calculateK1(v);
	v.key[1] = calculateK2(v);
}

bool FinalDStar::isGoal(vertex &v) {
	if ((v.type == '7') || (v.type == 55)) {
		return true;
	}
	return false;
}

bool FinalDStar::notGoal(vertex &v) {
	return !isGoal(v);
}

bool FinalDStar::notBlocked(vertex &v) {
	if ((v.type == '1') || (v.type == 49)) {
		return false;
	}
	return true;
}

void FinalDStar::insertToQ(vertex &v) {
	setKey(v);
	mQ.insert(&v);
}

double FinalDStar::cost(vertex &start, vertex &finish) {
	if        (HEURISTIC == MANHATTAN) {
		return manhattan(start.col, start.row, finish.col, finish.row);
	} else if (HEURISTIC == EUCLIDEAN) {
		return euclidean(start.col, start.row, finish.col, finish.row);
	} else {
		printf("ERROR: Undefined heuristic function!\n");
		exit(1);
	}
}

bool FinalDStar::locallyConsistant(vertex &v) {
	if (v.g == v.rhs) {
		return true;
	} else {
		return false;
	}
}

double FinalDStar::manhattan(int x1, int y1, int x2, int y2) {
	return max(abs(x1 - x2), abs(y1 - y2));
}

double FinalDStar::euclidean(int x1, int y1, int x2, int y2) {
	int a = abs(x1 - x2);
	int b = abs(y1 - y2);
	return sqrt(a * a + b * b);
}

void FinalDStar::printMaze() {
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			vertex v = maze[i][j];
			v.print();
		}
	}
}

void FinalDStar::printPQ() {
	mQ.printHeap();
}

void copyDisplayMapToMaze(GridWorld &gWorld, FinalDStar* fds) {
	bool initialize = false;
	for (int i = 0; i < fds->rows; i++) {
		for (int j = 0; j < fds->cols; j++) {
			
			if (gWorld.map[i][j].type == '7' || gWorld.map[i][j].type == '6') {
				if (gWorld.map[i][j].type != fds->maze[i][j].type) {
					initialize = true;
				}	
			}

			fds->maze[i][j].type   = gWorld.map[i][j].type;
			fds->maze[i][j].status = gWorld.map[i][j].status;
		}
	}
	if (initialize) {
		vertex start = gWorld.getStartVertex();
		vertex goal  = gWorld.getGoalVertex();
		fds->init(start.col, start.row, goal.col, goal.row);
	}
}

void copyMazeToDisplayMap(GridWorld &gWorld, FinalDStar* fds) {
	for (int i = 0; i < fds->rows; i++) {
		for (int j = 0; j < fds->cols; j++) {
			gWorld.map[i][j].h      = fds->maze[i][j].h;
			gWorld.map[i][j].g      = fds->maze[i][j].g;
			gWorld.map[i][j].rhs    = fds->maze[i][j].rhs;
			gWorld.map[i][j].key[0] = fds->maze[i][j].key[0];
			gWorld.map[i][j].key[1] = fds->maze[i][j].key[1];
			gWorld.map[i][j].type   = fds->maze[i][j].type;
			gWorld.map[i][j].status = fds->maze[i][j].status;
		}
	}	
}