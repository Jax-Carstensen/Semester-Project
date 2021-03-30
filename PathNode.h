#ifndef PATHNODE_H
#define PATHNODE_H

#include "Vector2.h";

using namespace std;

class PathNode {
public:
	Vector2 position;
	PathNode(int x, int y) {
		position = Vector2(x, y);
	}
	PathNode() {
		position = Vector2();
	}
	~PathNode() {}

	int gCost = 99999999;
	int hCost;
	int fCost;


};

#endif