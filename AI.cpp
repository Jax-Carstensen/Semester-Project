#include "AI.h"

void AI::setGoal(Vector2 newGoal) {
	goal = newGoal;
	pathfinding = true;
}
bool AI::getIsPathfinding() { return pathfinding; }
void AI::setup(Vector2Float newPosition) {
	position = newPosition;
	hasBeenSetup = true;
}
void AI::setSpeed(float newSpeed) {
	speed = newSpeed;
}
void AI::update(float deltaTime) {
	if (!pathfinding)
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
}
Vector2Float AI::getPosition() {
	return position;
}