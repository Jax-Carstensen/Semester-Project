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
	float health = 100;
	float maxHealth = 100;
	float infection = 0;
	float maxInfection = 100;
	float hunger = 100;
	float maxHunger = 100;
	const float hungerFillAmount = 0.1;
public:
	bool dead = false;
	bool managedDeath = false;
	bool active = true;
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
	//Pre:  A value to subtract from health
	//Post: Subtracts provided value from character's health
	void takeDamage(float amount) { health -= amount; }

	//Pre:  A value to add to infection
	//Post: Adds the provided value to the character's current infection level
	void infect(float amount) { infection += amount; }

	//Pre:  None
	//Post: Returns the item the character is currently carrying (if any)
	GroundItem getItem() { return carrying; }

	//Pre:  None
	//Post: Returns true if character is carrying an item, and false if it is not
	bool getIsCarrying() { return isCarrying; }

	//Pre:  A long int representing the current date in time
	//Post: Sets the character's last hit to the provided date
	void setLastHit(long int newLastHit) { lastHit = newLastHit; }

	//Pre:  None
	//Post: Returns the date in time of the last time this character hit a tree
	long int getLastHit() { return lastHit; }

	//Pre:  A long int representing the current date in time
	//Post: Returns true if it has been long enough since the last time the character hit a tree to do it again, and false if it is not
	bool canHit(long int time) { return time - lastHit >= millisecondsBetweenHits; }

	//Pre:  None
	//Post: Returns the number of milliseconds this character has as a cooldown between hitting objects
	int getMillisecondsBetweenHits() { return millisecondsBetweenHits; }

	//Pre:  None
	//Post: Returns true if the character is fulfilling an order, and false if it is not
	bool getHasOrder() { return hasOrder; }

	//Pre:  None
	//Post: Returns the order this character is currntly fulfilling (if any)
	Order getOrder() { return order; }

	//Pre:  None
	//Post: Returns true if this character is standing at the position their order requires them to be at, and false if they aren't
	bool getIsAtOrderLocation() { return order.getPosition().x == position.x && order.getPosition().y == position.y; }

	//Pre:  None
	//Post: Returns the character's current health
	float getHealth() { return health; }

	//Pre:  None
	//Post: Returns the character's maximum health
	float getMaxHealth() { return maxHealth; }

	//Pre:  None
	//Post: Returns the current level of infection the character has
	float getInfection() { return infection; }

	//Pre:  None
	//Post: Returns the maximum amount of infection the character can have
	float getMaxInfection() { return maxInfection; }

	//Pre:  None
	//Post: Returns the character's current hunger
	float getHunger() { return hunger; }
	
	//Pre:  None
	//Post: Returns the character's maximum amount of hunger
	float getMaxHunger() { return maxHunger; }

	//Pre:  None
	//Post: Called when the character's death is managed to ensure it is only done once
	void manageDeath() { managedDeath = true; }

	//Pre:  Float representing the amount of hunger to replenish
	//Post: Increases character's hunger by the provided amount (to a maximum of their max hunger)
	void eat(float amount) { hunger = max(maxHunger, hunger + amount); }

	//Pre:  An item to carry
	//Post: Makes the character carry the provided item
	void carry(GroundItem toCarry);

	//Pre:  None
	//Post: Calculates the character's speed based on their athleticism
	void calculateSpeed();

	//Pre:  A position for the character to head towards
	//Post: Makes the character head for the provided position
	void setOrderPosition(Vector2 pos);

	//Pre:  None
	//Post: Returns the item the character is holding, and removes it from their inventory
	GroundItem drop();

	//Pre:  None
	//Post: Called when the character completes an order, and will make it wander
	void completedOrder();

	//Pre:  Order for settler to fulfill
	//Post: Ensures the settler will begin fulfilling the provided order
	bool giveOrder(Order newOrder);

	//Pre:  The index of the skill needed to access
	//Post: Returns the value of the character's skill at the provided index
	Skill getSkillByIndex(int index);

	//Pre:  None
	//Post: Returns true if the character is male, and false if it is not
	bool getIsMale();

	//Pre:  None
	//Post: Randomly generates the gender, skill values, and name of the character
	void rerollStats();

	//Pre:  Position to move towards
	//Post: Character will try to make its way towards the provided position
	void setGoal(Vector2 newGoal);

	//Pre:  The skill name to search for
	//Post: Returns the value of the skill tied to the provided name
	Skill* getSkill(string skillName);

	//Pre:  A string representing the name of the skill, and an integer representing the amount to increase it by
	//Post: Increases the value of the provided skill by the provided value
	void improveSkill(string statName, int amount);

	//Pre:  A string to set the character's name to
	//Post: Sets the character's name to the provided string
	void setName(string newName);
	
	//Pre:  A float representing the size of tiles (in pixels)
	//Post: Returns the position of the character relative to the size of tiles in the game
	Vector2 getGlobalPosition(float newSize);

	//Pre:  A float representing delta time
	//Post: Moves the character, and manages its variables based on deltaTime
	void update(float &deltaTime);

	//Pre:  The window to draw to
	//Post: Draws the character to the window
	void draw(sf::RenderWindow& window);

	//Pre:  None
	//Post: Returns the character's current position
	Vector2 getPosition();

	//Pre:  A position to go to
	//Post: Sets the character's position to the provided one
	void setPosition(Vector2 newPosition);

	//Pre:  None
	//Post: Returns the character's name
	string getName();

	//Pre:  None
	//Post: Returns the current position in float form
	Vector2Float getFloatPosition();
};
#endif