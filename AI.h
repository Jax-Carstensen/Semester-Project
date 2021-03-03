#ifndef AI_H
#define AI_H

#include <iostream>
#include "Vector2.h"

using namespace std;

class AI {
private:
	Vector2 goal;
	bool pathfinding = false;
	float speed = 1;
	Vector2Float position;
public:
	AI() {}
	~AI() {}
	float currentX, currentY;
	bool hasBeenSetup = false;
	void setGoal(Vector2 newGoal) { goal = newGoal; pathfinding = true; }
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
		if (position.x < goal.x)
			position.x += min(speed * deltaTime, goal.x - position.x);
		if (position.y < goal.y)
			position.y += min(speed * deltaTime, goal.y - position.y);
		if (position.x > goal.x) 
			position.x -= min(speed * deltaTime, position.x - goal.x);
		if (position.y > goal.y) 
			position.y -= min(speed * deltaTime, position.y - goal.y);
		if (position.x == goal.x && position.y == goal.y)
			pathfinding = false;
	}
	Vector2Float getPosition() {
		return position;
	}
};
#endif