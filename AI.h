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
public:
	AI() {}  
	~AI() {}
	float currentX, currentY;
	bool hasBeenSetup = false;

	//Pre:  A position representing the new goal to move towards
	//Post: Makes the goal to move towards the provided position
	void setGoal(Vector2 newGoal);

	//Pre:  None
	//Post: Returns true if the AI is pathfinding and false if it is not
	bool getIsPathfinding();

	//Pre:  A position to start at
	//Post: Sets the starting position to the provided one
	void setup(Vector2Float newPosition);

	//Pre:  A float to set the AI's speed to
	//Post: Sets the AI's speed to the provided float
	void setSpeed(float newSpeed);

	//Pre:  A float representing the seconds since the last frame
	//Post: Moves the game object based on the provided float
	void update(float deltaTime);
	
	//Pre:  None
	//Post: Returns the current position the AI is at
	Vector2Float getPosition();
};
#endif