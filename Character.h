#ifndef CHARACTER_H
#define CHARACTER_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include "Skill.h"
#include "AI.h"
#include "Vector2.h"
#include "Order.h"
#include "GroundItem.h"

using namespace std;
class Character {
private:
	string name;
	const int skillCount = 12;
	Skill skills[12];
	AI characterAi = AI();
	bool isMale;
	bool hasOrder = false;
	sf::Text displayName;
	Vector2Float position;
	Order order;
	bool reachedLocation = false;
	float speedBoost = 1;
	long int lastHit = 0;
	int millisecondsBetweenHits = 750;
	GroundItem carrying;
	bool isCarrying = false;
public:
	Character() {
		skills[0] =  Skill("Shooting",					1);//Ability to shoot ranged weapons
		skills[1] =  Skill("Melee",						1);//Ability to use melee weapons
		skills[2] =  Skill("Construction",				1);//Ability to build structures
		skills[3] =  Skill("Mining",					1);//Ability to mine ores and stone
		skills[4] =  Skill("Cooking",					1);//Ability to cook meals
		skills[5] =  Skill("Plants",					1);//Ability to grow and harvest plants
		skills[6] =  Skill("Animals",					1);//Ability to tame animals
		skills[7] =  Skill("Crafting",					1);//Ability to craft different items
		skills[8] =  Skill("Infection Resistance",		1);//Ability to resist the zombie virus
		skills[9] =  Skill("Medical",					1);//Ability to heal other characters
		skills[10] = Skill("Fitness",					1);//Movment speed and chopping speed
		skills[11] = Skill("Intellectual",				1);//Researching
	}
	~Character() {}
	void carry(GroundItem toCarry) {
		carrying = toCarry;
	}
	void drop() {
		isCarrying = false;
	}
	void setLastHit(long int newLastHit) { lastHit = newLastHit; }
	long int getLastHit() { return lastHit; }
	bool canHit(long int time) { return time - lastHit >= millisecondsBetweenHits; }
	int getMillisecondsBetweenHits() { return millisecondsBetweenHits; }
	void calculateSpeed();
	bool getHasOrder() { return hasOrder; }
	Order getOrder() { return order; }
	bool getIsAtOrderLocation() { return order.getPosition().x == position.x && order.getPosition().y == position.y; }
	void completedOrder();
	bool giveOrder(Order newOrder);
	Skill getSkillByIndex(int index);
	bool getIsMale();
	void rerollStats();
	void setGoal(Vector2 newGoal);
	Skill* getSkill(string skillName);
	void improveSkill(string statName, int amount);
	void setName(string newName);
	Vector2 getGlobalPosition(float newSize);
	void update(float &deltaTime);
	void draw(sf::RenderWindow& window);
	Vector2* getPosition();
	void setPosition(Vector2 newPosition);
	string getName();
};
#endif