#include "FinalDStar.h"

#define DEBUG

void FinalDStar::debug(const char *format, ...) {
	#ifdef DEBUG 
	va_list args;
	va_start(args, format);
	vfprintf(stdout, "FINALDSTAR DEBUG :: ", nullptr);
	vfprintf(stdout, format, args);
	vfprintf(stdout, "\n", nullptr);
	va_end(args);
	fflush(stdout);
	#endif
}

FinalDStar::FinalDStar(unsigned int theHeuristic) {
	// rows = rs; cols = cs;
	HEURISTIC = theHeuristic;

	// maze.resize(rows);
	// for (int i = 0; i < rows; i++) {
	// 	maze[i].resize(cols);
	// }
}

void FinalDStar::setSize(int rs, int cs) {
	rows = rs; cols = cs;

	maze.resize(rows);
	for (int i = 0; i < rows; i++) {
		maze[i].resize(cols);
	}
}

void FinalDStar::init(int startX, int startY, int goalX, int goalY) {
	debug("init(Coordinates)");
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

			if (isStart(maze[i][j])) {
				s_start = &maze[i][j];
			}
			if (isGoal(maze[i][j])) {
				s_goal = &maze[i][j];
			}
		}
	}

	maze[goalY][goalX].rhs = 0;
	insertToQ(maze[goalY][goalX]);
}

void FinalDStar::init(GridWorld &gWorld) {
	debug("init(gWorld)");
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			maze[i][j].type = 1;
		}
	}
	copyDisplayMapToMaze(gWorld, this);
}

void FinalDStar::updateVertex(vertex &v) {
	// debug("UPDATING VERTEX");
	// v.print();
	double newRHS = INF;
	if (notGoal(v)) {
		for (int i = 0; i < DIRECTIONS; i++) {
			int x = v.col + neighbours[i].x;
			int y = v.row + neighbours[i].y;
			// debug("UV - Vertex (%d, %d), type %d", x, y, maze[y][x].type);
			if (notBlocked(maze[y][x])) {
				// debug("vertex(%d, %d) gets here", x, y);
				double rhs = maze[y][x].g + cost(v, maze[y][x]);
				// debug("(%d, %d) rhs = %f", x, y, rhs);
				if (rhs < newRHS) {
					newRHS = rhs;
				}
			}
		}
		v.rhs = newRHS;
	}
	
	// debug("newRHS = %f", newRHS);

	if (mQ.contains(&v)) {
		mQ.remove(&v);
	}

	if (!locallyConsistant(v)) {
		insertToQ(v);
	}
}

bool FinalDStar::computeShortestPath() {


	// while (compareKeys(mQ.getTopKey(), calculateStartKey()) || !locallyConsistant(maze[startCoord.y][startCoord.x])) {
	while (compareKeys(mQ.getTopKey(), calculateStartKey()) || !locallyConsistant(*s_start)) {
		double *tKey = mQ.getTopKey();
		k_old[0] = tKey[0];
		k_old[1] = tKey[1];
		vertex* u = mQ.pop();

		double uKey[2];
		calcKey(*u, uKey);
		
		if (compareKeys(k_old, &uKey[0])) {
			mQ.insert(u, &uKey[0]);
		} else if (u->g > u->rhs) {
			u->g = u->rhs;
			// debug("VERTEX");
			// u->print();
			for (int i = 0; i < DIRECTIONS; i++) {
				int x = u->col + neighbours[i].x;
				int y = u->row + neighbours[i].y;
				if (notBlocked(maze[y][x])) {
					updateVertex(maze[y][x]);	
				} 				
			}
		} else {
			u->g = INF;
			for (int i = 0; i < DIRECTIONS + 1; i++) {
				int x = u->col + allNine[i].x;
				int y = u->row + allNine[i].y;
				if (notBlocked(maze[y][x])) {
					updateVertex(maze[y][x]);	
				}		
			}
		}
	}

	if (s_start->g == INF) {
		return false;
	} else {
		return true;	
	}	
	// printMaze();
	// printPQ();
	// // debug("maze[6][2] t=%d, notBlocked=%d", maze[6][2].type, notBlocked(maze[6][2]));
	// debug("Goal? maze[5][3] %d, notgoal %d", isGoal(maze[5][3]), notGoal(maze[5][3]));
}

void FinalDStar::search(GridWorld &gWorld) {
	init(gWorld);
	s_start = getStartVertex();

	debug("s_start");
	s_start->print();
	debug("s_goal");
	s_goal->print();

	s_last = getStartVertex();
	computeShortestPath();

	// int iteration = 4;

	// for (int i = 0; i < iteration; i++) {
	// 	debug("ITERATION %d", i);

		while (notGoal(*s_start)) {
			int lowest_cost = INF;
			vertex* next = s_start;
			for (int i = 0; i < DIRECTIONS; i++) {
				int x = s_start->col + neighbours[i].x;
				int y = s_start->row + neighbours[i].y;
				if (notBlocked(maze[y][x])) {
					double m_cost = cost(*s_start, maze[y][x]) + maze[y][x].g;
					// debug("Checking (%d, %d) m_cost = %f", x, y, m_cost);
					if (m_cost < lowest_cost) {
						// debug("This is lower (%d, %d) m_cost = %f", x, y, m_cost);
						lowest_cost = m_cost;
						next = &maze[y][x];
						// next->print();
					}
				}
			}
			s_start = next;
			// debug("start");
			// s_start->print();

			for (int i = 0; i < DIRECTIONS; i++) {
				int x = s_start->col + neighbours[i].x;
				int y = s_start->row + neighbours[i].y;
				if (isHidden(maze[y][x])) {
					debug("HIDDEN TRIGGERED (%d, %d)", x, y);
					block(maze[y][x]);
					km = km + cost(*s_last, *s_start);
					s_last = s_start;

					for (int i = 0; i < DIRECTIONS; i++) {
						int x = s_start->col + neighbours[i].x;
						int y = s_start->row + neighbours[i].y;
						if (notBlocked(maze[y][x])) {
							updateVertex(maze[y][x]);
						}
					}
					computeShortestPath();
				}
			}
		}
	// }

	
	


	debug("s_start");
	s_start->print();
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

double* FinalDStar::calculateStartKey() {
	vertex *start = getStartVertex();
	calcKey(*start, mStartKey);
	return mStartKey;
}

vertex* FinalDStar::getVertex(int x, int y) {
	return &maze[y][x];
}

vertex* FinalDStar::getStartVertex() {
	vertex *startVertex;
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			if (isStart(maze[i][j])) {
				startVertex = &maze[i][j];
			}
		}
	}
	return startVertex;
}

vertex* FinalDStar::getGoalVertex() {
	vertex *goalVertex;
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			if (isGoal(maze[i][j])) {
				goalVertex = &maze[i][j];
			}
		}
	}
	return goalVertex;
}

double FinalDStar::calculateK2(vertex &v) {
	return min(v.g, v.rhs);
}

double FinalDStar::calculateK1(vertex &v) {
	double retVal = calculateK2(v);
	// debug("s_start");
	// s_start->print();
	retVal += cost(v, *s_start); 
	retVal += km;
	return retVal;
}

double* FinalDStar::calculateKey(vertex &v) {
	double *keys = new double(2);
	keys[0] = calculateK1(v);
	keys[1] = calculateK2(v);
	return keys;
}

void FinalDStar::calcKey(vertex &v, double *container) {
	container[0] = calculateK1(v);
	container[1] = calculateK2(v);
}

void FinalDStar::setKey(vertex &v) {
	v.key[0] = calculateK1(v);
	v.key[1] = calculateK2(v);
}

bool FinalDStar::compareKeys(double* lhs, double* rhs) {
	if (lhs[0] < rhs[0]) {
		return true;
	} else if (lhs[0] > rhs[0]) {
		return false;
	} else {
		if (lhs[1] < rhs[1]) {
			return true;
		} else {
			return false;
		}
	}
}

bool isGoal(vertex &v) {
	if ((v.type == '7') || (v.type == 55)) {
		return true;
	}
	return false;
}

bool notGoal(vertex &v) {
	return !isGoal(v);
}

bool isStart(vertex &v) {
	if (v.type == '6' || v.type == 54) {
		return true;
	}
	return false;
}

bool notBlocked(vertex &v) {
	if ((v.type == '1') || (v.type == 49)) {
		return false;
	}
	return true;
}

bool isHidden(vertex &v) {
	if ((v.type == '9') || (v.type == 57)) {
		return true;
	} else {
		return false;
	}
}

bool locallyConsistant(vertex &v) {
	if (v.g == v.rhs) {
		return true;
	} else {
		return false;
	}
}

void block(vertex &v) {
	v.type = '1';
	v.h = INF;
	v.g = INF;
	v.rhs = INF;
}

void FinalDStar::insertToQ(vertex &v) {
	setKey(v);
	mQ.insert(&v);
}

double cost(vertex &start, vertex &finish) {
	if        (HEURISTIC == MANHATTAN) {
		return manhattan(start.col, start.row, finish.col, finish.row);
	} else if (HEURISTIC == EUCLIDEAN) {
		return euclidean(start.col, start.row, finish.col, finish.row);
	} else {
		printf("ERROR: Undefined heuristic function!\n");
		exit(1);
	}
}

double manhattan(int x1, int y1, int x2, int y2) {
	return max(abs(x1 - x2), abs(y1 - y2));
}

double euclidean(int x1, int y1, int x2, int y2) {
	int a = abs(x1 - x2);
	int b = abs(y1 - y2);
	return sqrt(a * a + b * b);
}

void FinalDStar::printMaze() {
	printf("FinalDStar Maze: \n");
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
	// printf("copyDisplayMapToMaze\n");

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

