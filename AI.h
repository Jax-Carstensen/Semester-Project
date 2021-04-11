#ifndef AI_H
#define AI_H

#include <iostream>
#include "Vector2.h"
#include "Grids.h"
#include "PathNode.h"
#include <list>

using namespace std;

class AI {
private:
	Vector2 goal;
	bool pathfinding = false;
	float speed = 1;
	Vector2Float position;
	PathNode tiles[256];
	int currentNodeIndex = 0;
	int currentUseNodeIndex = 0;

	const int MOVE_STRAIGHT_COST = 10;
	const int MOVE_DIAGONAL_COST = 14;
public:
	AI() {}  
	~AI() {}
	float currentX, currentY;
	bool hasBeenSetup = false;
	void setGoal(Vector2 newGoal, Grid* grid) { 
		goal = newGoal; 
		pathfinding = true;
		startFindingPath(grid);
	}
	void startFindingPath(Grid* grid) {
		bool found = false;
		PathNode startNode = PathNode(goal.x, goal.y);
		PathNode currentNode = startNode;
		PathNode endNode = PathNode(goal.x, goal.y);
		int iterations = 0;
		while (true) {
			iterations++;
			currentNode = getBestOption(currentNode, endNode, grid);
			addToQueue(currentNode);
			if (currentNode.equals(endNode)) {
				break;
			}
			if (iterations > 200) {
				cout << "Error finding path" << endl;
				break;
			}
		}
		//Look at the neighbors of that node
	}
	PathNode getBestOption(PathNode node, PathNode end, Grid* grid) {
		PathNode toReturn;
		int x = position.x;
		int y = position.y;
		if (node.position.x < end.position.x) {
			toReturn = PathNode(x + 1, y);
			if (toReturn.isValid(256))
				if (grid->getSpace(x + 1, y).walkable)
					return toReturn;
		}
		else if (node.position.x > end.position.x) {
			toReturn = PathNode(x - 1, y);
			if(toReturn.isValid(256))
				if (grid->getSpace(x - 1, y).walkable)
					return toReturn;
		}
		return node;
		/*int newX = 0;
		int newY = 0;
		if (node.position.x < end.position.x) {
			newX++;
		}
		else if (node.position.x > end.position.x) {
			newX--;
		}
		if (node.position.y < end.position.y) {
			newY++;
		}
		else if (node.position.y > end.position.y) {
			newY--;
		}
		return PathNode(node.position.x + newX, node.position.y + newY);*/
		PathNode nodes[4] = {
			PathNode(node.position.x + 1, node.position.y),
			PathNode(node.position.x - 1, node.position.y),
			PathNode(node.position.x, node.position.y + 1),
			PathNode(node.position.x, node.position.y - 1)
		};
		float distances[4] = {
			getDistanceBetweenNodes(nodes[0], end),
			getDistanceBetweenNodes(nodes[1], end),
			getDistanceBetweenNodes(nodes[2], end),
			getDistanceBetweenNodes(nodes[3], end)
		};
		int closestIndex = 0;
		for (int i = 0; i < 4; i++) {
			if (nodes[i].isValid(256)) {
				if (grid->getSpace(nodes[i].position.x, nodes[i].position.y).walkable) {
					if (distances[i] - distances[closestIndex] < 0) {
						closestIndex = i;
					}
				}
			}
			else if (i == closestIndex) {
				closestIndex++;
			}
		}
		return nodes[closestIndex];
	}
	void addToQueue(PathNode node) {
		tiles[currentNodeIndex++] = node;
	}
	int calculateDistanceCost(PathNode a, PathNode b) {
		int xDistance = abs(a.position.x - b.position.x);
		int yDistance = abs(a.position.y - b.position.y);
		int remaining = abs(xDistance - yDistance);
		return MOVE_DIAGONAL_COST * min(xDistance, yDistance) + MOVE_STRAIGHT_COST * remaining;
	}
	float getDistanceBetweenNodes(PathNode node1, PathNode node2) {
		return sqrt(pow(node2.position.x - node1.position.x, 2) + pow(node2.position.y - node1.position.y, 2));
	}
	bool getIsPathfinding() { return pathfinding; }
	void setup(Vector2Float newPosition) {
		position = newPosition;
		hasBeenSetup = true;
	}
	void setSpeed(float newSpeed) {
		speed = newSpeed;
	}
	void update(float deltaTime) {
		if (!pathfinding)
			return;
		if (currentNodeIndex == 0)
			return;
		Vector2 g = goal;
		if (position.x < goal.x)
			position.x += min(speed * deltaTime, g.x - position.x);
		if (position.y < goal.y)
			position.y += min(speed * deltaTime, g.y - position.y);
		if (position.x > goal.x) 
			position.x -= min(speed * deltaTime, position.x - g.x);
		if (position.y > goal.y) 
			position.y -= min(speed * deltaTime, position.y - g.y);
		if (position.x == goal.x && position.y == goal.y) {
			pathfinding = false;
		}
		/*if (position.x == g.x && position.y == g.y) {
			currentUseNodeIndex++;
			if (currentNodeIndex == currentUseNodeIndex) {
				currentNodeIndex = 0;
				pathfinding = false;
				currentUseNodeIndex = 0;
			}
		}*/
	}
	Vector2Float getPosition() {
		return position;
	}
};
#endif