#ifndef PATHNODE_H
#define PATHNODE_H

#include "Vector2.h";
#include "Grids.h"

using namespace std;

class PathNode {
public:
	Vector2 position;
	Vector2 cameFromPosition = Vector2(-1, -1);

	bool open = false;
	bool closed = false;
	bool isBarrier = false;
	bool isStart = false;
	bool isEnd = false;
	bool isPath = false;
	Vector2 neighbors[4];
	int totalNeighbors = 0;
	void updateNeighbors(Grid* grid) {
		Vector2 positions[4] = {
			Vector2(position.x - 1, position.y),
			Vector2(position.x + 1, position.y),
			Vector2(position.x, position.y + 1),
			Vector2(position.x, position.y - 1),
		};
		for (int i = 0; i < 4; i++) {
			if (positions[i].x >= 0 && positions[i].x < 256 && positions[i].y >= 0 && positions[i].y < 256) {
				if (grid->getSpace(positions[i].x, positions[i].y).walkable) {
					neighbors[totalNeighbors++] = positions[i];
				}
			}
		}
	}

	PathNode(int x, int y) {
		position = Vector2(x, y);
	}
	PathNode(GridItem g) {
		position = g.getPosition();
		isBarrier = !g.walkable;
	}
	PathNode() {
		position = Vector2();
	}
	~PathNode() {}

	int gCost = 999999;
	int hCost = 999999;
	int fCost = 999999;

	void calculateFCost() {
		fCost = gCost + hCost;
	}
	bool equals(PathNode otherNode) {
		return otherNode.position.x == position.x && otherNode.position.y == position.y;
	}
	bool isValid(int size) {
		return position.x >= 0 && position.x < size && position.y >= 0 && position.y < size;
	}
	float getDistance(PathNode other) {
		return abs(other.position.x - position.x) + abs(other.position.y - position.y);
	}


};

#endif