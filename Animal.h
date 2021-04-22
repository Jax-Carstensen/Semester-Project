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
	Animal();
	Animal(string newName);
	~Animal();

	//Pre:  None
	//Post: Returns the current position of the animal in float form
	Vector2Float getFloatPosition();

	//Pre:  The animal to copy stats from, and the position to start at
	//Post: Initializes this animal's stats based on the provided one, and moves to the provided position
	void initialize(Animal otherAnimal, Vector2 newPosition);

	//Pre:  None
	//Post: Returns the name of the animal
	string getName();

	//Pre:  None
	//Post: Returns the current position of the animal
	Vector2 getPosition();

	//Pre:  None
	//Post: Returns true if the animal is alive, and false if it is not
	bool isActive();

	//Pre:  A boolean representing whether or not the animal should be alive
	//Post: Kills the animal if the provided boolean is false, and resurrects it if it is true
	void setActive(bool newActive);

	//Pre:  A float representing the size of tiles (in pixels)
	//Post: Returns the position of the animal relative to the size of tiles in the game
	Vector2 getGlobalPosition(float blockSize);

	//Pre:  A float representing deltaTime
	//Post: Moves the animal based upon the provided deltaTime
	void update(float& deltaTime);

	//Pre:  None
	//Post: Returns the current goal position of the animal
	Vector2 getGoal();
};
#endif