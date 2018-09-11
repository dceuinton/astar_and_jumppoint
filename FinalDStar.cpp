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
	// Do the priority queue init
	km = 0;

	startCoord.x = startX; startCoord.y = startY;
	goalCoord.x = goalX;   goalCoord.y = goalY;

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			maze[i][j].h = 0;
			maze[i][j].g   = INF;
			maze[i][j].rhs = INF;
			maze[i][j].key[0] = 0;
			maze[i][j].key[1] = 0;
		}
	}

	maze[goalX][goalY].rhs = 0;

	// Insert the goal vertex into the priority queue.
}

double FinalDStar::workOutH(int x, int y) {
	if (HEURISTIC == MANHATTAN) {  
		return max(abs(x - startCoord.x), abs(y - startCoord.y));
	} else if (HEURISTIC == EUCLIDEAN) {
		// int a = 
	} else {
		printf("ERROR: Undefined heuristic function!\n");
		exit(1);
	}

	return 0.0;
}

void FinalDStar::printMaze() {

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			vertex v = maze[i][j];
			printf("(%d, %d), h = %.1f, g = %.1f, rhs = %.1f, key = [%.1f, %.1f] \n", 
				i, j, v.h, v.g, v.rhs, v.key[0], v.key[1]);
		}
	}

}