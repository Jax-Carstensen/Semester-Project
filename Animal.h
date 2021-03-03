#ifndef ANIMAL_H
#define ANIMAL_H
#include <iostream>
#include "AI.h"

using namespace std;

class Animal {
private:
	AI ai;
	string name;
	string animalType;
public:
	Animal() {}
	~Animal() {}
};
#endif