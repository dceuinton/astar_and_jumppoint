#include "FinalDStar.h"

FinalDStar::FinalDStar(int rs, int cs) {
	rows = rs; cols = cs;

	maze.resize(rows);
	for (int i = 0; i < rows; i++) {
		maze[i].resize(cols);
	}
}