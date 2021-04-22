#include "Animal.h"
#include "Ai.h"
#include <iostream>

using namespace std;
Animal::Animal() {

}
Animal::Animal(string newName) {
	name = newName;
}
Animal::~Animal() {}
void Animal::initialize(Animal otherAnimal, Vector2 newPosition) {
	name = otherAnimal.name;
	position = Vector2Float(newPosition.x, newPosition.y);
	active = true;
}
string Animal::getName() {
	return name;
}
Vector2 Animal::getPosition() {
	return Vector2(position.x, position.y);
}
bool Animal::isActive() {
	return active;
}
Vector2Float Animal::getFloatPosition() {
	return position;
}
void Animal::setActive(bool newActive) {
	active = newActive;
}
Vector2 Animal::getGlobalPosition(float blockSize) {
	return Vector2(position.x * blockSize, position.y * blockSize);
}
void Animal::update(float& deltaTime) {
	if (health <= 0) {
		health = 0;
		dead = true;
		return;
	}
	if (!ai.hasBeenSetup)
		ai.setup(position);

	if (!ai.getIsPathfinding()) {
		Vector2 newGoalAddon = Vector2(rand() % 12, rand() % 12);
		if (rand() % 2 == 0)
			newGoalAddon.x *= -1;
		if (rand() % 2 == 0)
			newGoalAddon.y *= -1;

		Vector2 newGoal = Vector2(newGoalAddon.x + position.x, newGoalAddon.y + position.y);
		newGoal.x = max(newGoal.x, 0);
		newGoal.y = max(newGoal.y, 0);
		newGoal.x = min(newGoal.x, 255);
		newGoal.y = min(newGoal.y, 255);
		ai.setGoal(newGoal);
		goal = newGoal;
	}
	ai.update(deltaTime);
	position = ai.getPosition();
}
Vector2 Animal::getGoal() {
	return goal;
}