#ifndef ANIMAL_H
#define ANIMAL_H
#include <iostream>
#include "AI.h"
#include "Vector2.h"

using namespace std;

class Animal {
private:
	AI ai;
	string name;
	string animalType;
	Vector2Float position;
	bool active = false;
	float health = 100;
	float maxHealth = 100;
	bool dead;
	Vector2 goal;
public:
	Animal() {}
	Animal(string newName) {
		name = newName;
	}
	~Animal() {}
	void initialize(Animal otherAnimal, Vector2 newPosition) {
		name = otherAnimal.name;
		position = Vector2Float(newPosition.x, newPosition.y);
		active = true;
	}
	string getName() {
		return name;
	}
	Vector2 getPosition() {
		return Vector2(position.x, position.y);
	}
	bool isActive() {
		return active;
	}
	void setActive(bool newActive) {
		active = newActive;
	}
	Vector2 getGlobalPosition(float blockSize) {
		return Vector2(position.x * blockSize, position.y * blockSize);
	}
	void update(float& deltaTime, Grid* grid) {
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
			ai.setGoal(newGoal, grid);
			goal = newGoal;
		}
		ai.update(deltaTime);
		position = ai.getPosition();
	}
	Vector2 getGoal() {
		return goal;
	}
};
#endif