#pragma once
#include "crossingPoint.hpp"

class lastNextObj {
public:
	int last;
	int next;

	lastNextObj(int lastPlace, int nextPlace) {
		last = lastPlace;
		next = nextPlace;

	
	}

	int getLast() {
		return last;
	}

	int getNext() {
		return next;
	}

	int setLast(int a) {
		last = a;
		return last;
	}
	int setNext(int b) {
		last = b;
		return next;
	}


};