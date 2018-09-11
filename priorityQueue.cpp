#include "priorityQueue.h"

PriorityQueue::PriorityQueue() {
	mHeap = {};
	make_heap(mHeap.begin(), mHeap.end(), keyComparison);
}

void PriorityQueue::clear() {
	mHeap = {};
}

/* ISSUE: If I push all values of the same it alternates between pushing
		  in front and behind the first value added. Not sure why. Need 
		  to fix it. But for now moving on. */
void PriorityQueue::push(vertex *v) {
	mHeap.push_back(v);
	push_heap(mHeap.begin(), mHeap.end(), keyComparison);
}

vertex* PriorityQueue::pop() {
	vertex *v = nullptr;
	if (!mHeap.empty()) {
		v = new vertex();
		v = mHeap.front();
		pop_heap(mHeap.begin(), mHeap.end(), keyComparison);
		mHeap.pop_back();
	}
	return v;	
}

double* PriorityQueue::topKey() {
	if (!mHeap.empty()) {
		return mHeap.front()->key;
	} else {
		double *max = new double(2);
		max[0] = INF; max[1] = INF; 
		return max;
	}
}

void PriorityQueue::insert(vertex* v, double *k) {
	v->key[0] = k[0];
	v->key[1] = k[1];
	push(v);
}

void PriorityQueue::insert(vertex* v) {
	push(v);
}

void PriorityQueue::update(vertex* v, double *k) {
	v->key[0] = k[0];
	v->key[1] = k[1];
}

void PriorityQueue::remove(vertex *v) {
	vector<vertex*>::iterator loc;
	for (vector<vertex*>::iterator it = mHeap.begin(); it != mHeap.end(); ++it) {
		if (*it == v) {
			loc = it;
		}
	}
	mHeap.erase(loc);
}

bool PriorityQueue::contains(vertex *v) {
	if (find(mHeap.begin(), mHeap.end(), v) != mHeap.end()) {
		return true;
	} else {
		return false;
	}
}





// ------------------------------------------------------------
// Helpers 
// ------------------------------------------------------------

void PriorityQueue::printHeap() {
	vector<vertex*> temp(mHeap.size());
	for (int i = 0; i < mHeap.size(); i++) {
		temp[i] = mHeap[i];
	}

	make_heap(temp.begin(), temp.end(), keyComparison);

	printf("My Priority Queue: \n");
	for (int i = 0; i < mHeap.size(); i++) {
		vertex *current = temp.front();
		pop_heap(temp.begin(), temp.end(), keyComparison);
		temp.pop_back();
		current->print();
	}
}

bool keyComparison(vertex* lhs, vertex* rhs) {
	if (lhs->key[0] < rhs->key[0]) {
		// return true;
		return false;
	} else if (lhs->key[0] > rhs->key[0]) {
		// return false;
		return true;
	} else {
		if (lhs->key[1] < rhs->key[1]) {
			// return true;
			return false;
		} else {
			// return false;
			return true;
		}
	}
}

