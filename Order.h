#ifndef ORDER_H
#define ORDER_H

#include <iostream>
#include "Vector2.h"

using namespace std;

class Order {
private:
	string orderType;
	string skillName;
	int skillRequirementLevel = 1;
	Vector2 position;    
public:
	Order() {}
	Order(string newOrderType, Vector2 newPosition, string newSkillName, int newSkillRequirementLevel) {
		orderType = newOrderType;
		position = newPosition;
		skillName = newSkillName;
		skillRequirementLevel = newSkillRequirementLevel;
	}
	~Order() {}
	string getSkillName() {
		return skillName;
	}
	int getSkillRequirementLevel() {
		return skillRequirementLevel;
	}
	Vector2 getPosition() {
		return position;
	}
	string getOrderType() {
		return orderType;
	}
	void setPosition(Vector2 newPosition) {
		position = newPosition;
	}
};
#endif