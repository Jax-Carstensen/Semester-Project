#ifndef ZOMBIE_H
#define ZOMBIE_H

#include "AI.h"
#include "Vector2.h"

using namespace std;
class Zombie {
private:
	Vector2 position;
	AI ai;
public:
	Zombie(Vector2 newPosition) {
		position = newPosition;
	}
	Zombie() {
		position = Vector2();
	}
	void setup(Vector2 newPosition);
};
#endif