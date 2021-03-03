#pragma once

#include <iostream>;

using namespace std;
class Item {
private:
	string name;
	int craftingItemsCount = 0;
	string* craftingDependencies;
	int* craftingDependenciesAmount;
	string textureName;
public:
	Item() {}
	Item(string newName, string* newCraftingDependencies, int* newCraftingDependenciesAmount, int newCraftingItemsCount, string newTextureName) {
		name = newName;
		craftingItemsCount = newCraftingItemsCount;
		craftingDependencies = newCraftingDependencies;
		craftingDependenciesAmount = newCraftingDependenciesAmount;
		textureName = newTextureName;
	}
	~Item() {
		//delete craftingDependencies;
		//delete craftingDependenciesAmount;
	}
	void setName(string newName) { name = newName; }
	string getName() { return name; }
	string getTextureName() { return textureName; }
};