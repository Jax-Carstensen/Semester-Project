#ifndef ZOMBIE_H
#define ZOMBIE_H

#include "AI.h"
#include "Vector2.h"
#include "Character.h"

using namespace std;
class Zombie {
private:
	Vector2Float position;
	AI ai;
	bool active = false;
	float health = 100;
	float damage = 0;
	float maxHealth = 100;
	string textureName;
	int targetIndex;
	bool targeting = false;
	//Tiles not pixels
	float chaseRadius = 4;
	float attackRadius = 0.8;
	float lastAttack = 99999;
	float stopChasingRadius = 8;
	float timeBetweenAttacks = 1;
public:
	Zombie();
	Zombie(float mnewMxHealth, float newDamage, string newTextureName);

	//Pre:  A zombie to copy from, and a position to start at
	//Post: Copies the other zombie's values to this one, and moves it to the provided position
	void initialize(Zombie otherZombie, Vector2 newPosition);

	//Pre:  None
	//Post: Returns true if the zombie is active, and false if it is not
	bool isActive();

	//Pre:  None
	//Post: Returns the name of the zombie's current texture
	string getTexture();

	//Pre:  A Vector2 representing the position the zombie is to be created at
	//Post: Sets the zombie up at the provided position
	void setup(Vector2 newPosition);

	//Pre:  None
	//Post: Returns the zombie's position
	Vector2 getPosition();

	//Pre:  A float representing the default size of the tiles
	//Post: Returns the position of the zombie multiplied by the provided default tile size
	Vector2 getGlobalPosition(float blockSize);

	//Pre:  None
	//Post: Returns the current position of the zombie in float form
	Vector2Float getFloatPosition();

	//Pre:  A float representing the time since the last frame, an array of characters, the length of the array, and the default tile size
	//Post: Zombie targets nearby settlers and chases them
	int update(float deltaTime, Character* settlers, int settlerLength, int blockSize);
	
	//Pre:  A Vector2 representing the position to measure the distance to, and the size of the default tiles
	//Post: Returns the distance to the provided position
	float getDistance(Vector2 otherPosition, int blockSize);
};
#endif