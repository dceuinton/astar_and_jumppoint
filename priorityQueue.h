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

	void clear();
	void push(vertex *v);
	vertex* pop();
	double* topKey();
	double* getTopKey();
	void insert(vertex *v, double *k);
	void insert(vertex* v, double k[2]);
	void insert(vertex *v);
	void update(vertex *v, double *k);
	void remove(vertex *v);

	void printHeap();
	bool contains(vertex *v);

private:
	vector<vertex*> mHeap;

	double mTopKey[2];
};

bool keyComparison(vertex *lhs, vertex *rhs);

#endif