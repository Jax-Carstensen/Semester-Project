#ifndef SKILL_H
#define SKILL_H

#include <string>

using namespace std;

class Skill {
private:
	int value = 1;
	string name = "";
public:
	Skill() {}
	Skill(string newName, int newValue) {
		value = newValue;
		name = newName;
	}
	~Skill() {}
	void addToValue(int newValue) { value += newValue; }
	string getSkillName() { return name; }
	int getSkillValue() { return value; }
	void setValue(int newValue) { value = newValue; }
};
#endif