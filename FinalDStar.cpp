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
	double key[2] = {};
	mQ.insert(maze[goalY][goalX], key);
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

double FinalDStar::calculateH(int x, int y) {
	if (HEURISTIC == MANHATTAN) {  
		return max(abs(x - startCoord.x), abs(y - startCoord.y));
	} else if (HEURISTIC == EUCLIDEAN) {
		int a = abs(x - startCoord.x);
		int b = abs(y - startCoord.y);
		return sqrt(a * a + b * b);
	} else {
		printf("ERROR: Undefined heuristic function!\n");
		exit(1);
	}

	return 0.0;
}

void FinalDStar::printMaze() {

	// for (int i = 0; i < rows; i++) {
	// 	for (int j = 0; j < cols; j++) {
	// 		vertex v = maze[i][j];
	// 		printf("(%d, %d), t = %u, h = %.1f, g = %.1f, rhs = %.1f, key = [%.1f, %.1f] \n", 
	// 			i, j, v.type, v.h, v.g, v.rhs, v.key[0], v.key[1]);
	// 	}
	// }

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			vertex v = maze[i][j];
			// printNode(v);
			v.print();
		}
	}

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