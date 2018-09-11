#ifndef MY_PRIORITY_QUEUE
#define MY_PRIORITY_QUEUE

#include <iostream>
#include <algorithm>
#include <vector>

#include "globalVariables.h"

using namespace std;

class PriorityQueue {

public:
	PriorityQueue();

	void clearQueue();
	void push(vertex *v);
	vertex* pop();
	double* topKey();
	// void insert(vertex &v, double *key);
	// void update(vertex &v, double *key);
	// void remove(vertex &v);

	void printHeap();

private:
	vector<vertex*> mHeap;
};

bool keyComparison(vertex *lhs, vertex *rhs);

#endif