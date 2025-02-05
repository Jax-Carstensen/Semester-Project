#include <SFML/Graphics.hpp>
#include <iostream>
#include "Character.h"
#include "AI.h"
#include "Skill.h"
#include "Vector2.h"
#include <stdlib.h>
#include <fstream>
#include <string>
#include "Grids.h"

using namespace std;


const int maleNamesCount = 2942;
const int femaleNamesCount = 5000;

void Character::setGoal(Vector2 newGoal) { 
	characterAi.setGoal(newGoal); 
}
void Character::setName(string newName) { name = newName; }
string Character::getName() { return name; }
bool Character::getIsMale() { return isMale; }

void Character::carry(GroundItem toCarry) {
	carrying = toCarry;
	isCarrying = true;
}

GroundItem Character::drop() {
	isCarrying = false;
	return carrying;
}

void Character::rerollStats() {
	isMale = rand() % 2;
	string fileName = "./maleNames.txt";
	int maxNameCount = maleNamesCount;
	if (!isMale) {
		fileName = "./femaleNames.txt";
		maxNameCount = femaleNamesCount;
	}
	int nameIndex = rand() % maxNameCount;
	ifstream file(fileName);
	string newName;
	for (int i = 0; i < nameIndex; i++)
		getline(file, newName);
	setName(newName);
	file.close();
	int points = 80;
	for (int i = 0; i < skillCount; i++) {
		int amount = rand() % (min(points, 10));
		points -= amount;
		skills[i].setValue(amount);
	}
	calculateSpeed();
}

void Character::calculateSpeed() {
	speedBoost = 0.08 * getSkill("Fitness")->getSkillValue() + 1;
	characterAi.setSpeed(speedBoost);
	millisecondsBetweenHits = 750 - (getSkill("Fitness")->getSkillValue() * 25);
}

void Character::setOrderPosition(Vector2 pos) {
	order.setPosition(pos);
	setGoal(pos);
}

void Character::setPosition(Vector2 newPosition) {
	position = Vector2Float(newPosition.x, newPosition.y);
}

void Character::completedOrder() {
	hasOrder = false;
	reachedLocation = false;
}

bool Character::giveOrder(Order newOrder) {
	if (hasOrder) return false;
	int level = 0;
	for (int i = 0; i < 12; i++) {
		if (skills[i].getSkillName() == newOrder.getSkillName()) {
			level = skills[i].getSkillValue();
			break;
		}
	}
	if (level >= newOrder.getSkillRequirementLevel()) {
		hasOrder = true;
		order = newOrder;
		characterAi.setGoal(order.getPosition());
		return true;
	}
	return false;
}

void Character::update(float &deltaTime) {
	hunger -= hungerFillAmount * deltaTime;
	if (hunger <= 0) {
		hunger = 0;
		health -= hungerFillAmount * deltaTime;
	}
	if (health <= 0) {
		health = 0;
		dead = true;
		return;
	}
	if (!characterAi.hasBeenSetup)
		characterAi.setup(position);
	
	if (!characterAi.getIsPathfinding()) {
		if (hasOrder) {
			reachedLocation = true;
		}
		else {
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
			characterAi.setGoal(newGoal);
		}
	}
	if (reachedLocation) {
		//Start fulfilling your order
	}
	characterAi.update(deltaTime);
	position = characterAi.getPosition();
}

Vector2 Character::getGlobalPosition(float newSize) {
	Vector2Float toReturn = Vector2Float(position.x, position.y);
	if (dead && position.x <= 0)
		toReturn.x++;
	if (dead && position.y <= 0)
		toReturn.y++;
	return Vector2(toReturn.x * newSize, toReturn.y * newSize);
}
   
void Character::draw(sf::RenderWindow& window) {

}
Skill Character::getSkillByIndex(int index) {
	return skills[index];
}

Vector2 Character::getPosition() {
	return Vector2(position.x, position.y);
}
Vector2Float Character::getFloatPosition() {
	return position;
}

bool rnd(int chance) {
	int value = rand() % 100;
	if (chance >= value)
		return true;
	return false;
}

Skill* Character::getSkill(string skillName) {
	for (int i = 0; i < skillCount; i++) {
		if (skills[i].getSkillName() == skillName) {
			return &skills[i];
		}
	}
}
void Character::improveSkill(string statName, int amount) { 
	getSkill("statName")->addToValue(amount);
}