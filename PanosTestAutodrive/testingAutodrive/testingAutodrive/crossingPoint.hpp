#pragma once
class crossingPoint {
public:
	int north;
	int south;
	int west;
	int east;

	crossingPoint(int n, int s, int w, int e) {
		north = n;
		south = s;
		west = w;
		east = e;
	}

};