#include "Zombie.h"

using namespace std;

void Zombie::setup(Vector2 newPosition) {
	position = Vector2Float(newPosition.x, newPosition.y);
}

Zombie::Zombie() {
	textureName = "zombie";
	damage = 10;
	maxHealth = 100;
	health = 100;
}
Zombie::Zombie(float mnewMxHealth, float newDamage, string newTextureName) {
	textureName = newTextureName;
	health = mnewMxHealth;
	maxHealth = mnewMxHealth;
	damage = newDamage;
}
void Zombie::initialize(Zombie otherZombie, Vector2 newPosition) {
	position = Vector2Float(newPosition.x, newPosition.y);
	textureName = otherZombie.textureName;
	health = otherZombie.maxHealth;
	maxHealth = otherZombie.maxHealth;
	active = true;
}
bool Zombie::isActive() {
	return active;
}
string Zombie::getTexture() {
	return textureName;
}

Vector2 Zombie::getPosition() {
	return Vector2(position.x, position.y);
}

Vector2 Zombie::getGlobalPosition(float blockSize) {
	return Vector2(position.x * blockSize, position.y * blockSize);
}

Vector2Float Zombie::getFloatPosition() {
	return position;
}

int Zombie::update(float deltaTime, Character* settlers, int settlerLength, int blockSize) {
	int toReturn = -1;
	if (lastAttack < timeBetweenAttacks)
		lastAttack += deltaTime;
	if (!ai.hasBeenSetup)
		ai.setup(position);
	if (targeting && (settlers[targetIndex].dead || !settlers[targetIndex].active))
		targeting = false;
	if (!targeting) {
		for (int i = 0; i < settlerLength; i++) {
			if (getDistance(settlers[i].getPosition(), blockSize) <= chaseRadius) {
				if (settlers[i].dead || !settlers[i].active) {
					continue;
				}
				//Targeting a new survivor
				targeting = true;
				targetIndex = i;
			}
		}
	}
	else {
		ai.setGoal(settlers[targetIndex].getPosition());
		if (getDistance(settlers[targetIndex].getPosition(), blockSize) <= attackRadius) {
			ai.setGoal(Vector2(position.x, position.y));
			if (lastAttack >= timeBetweenAttacks) {
				toReturn = targetIndex;
				lastAttack = 0;
			}
		}
	}
	ai.update(deltaTime);
	position = ai.getPosition();
	return toReturn;
}

float Zombie::getDistance(Vector2 otherPosition, int blockSize) {
	int toAdd = blockSize * 0.5;
	return sqrt(pow((otherPosition.x + toAdd) - (position.x + toAdd), 2) + pow((otherPosition.y + toAdd) - (position.y + toAdd), 2));
}