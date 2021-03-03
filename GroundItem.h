#ifndef GROUNDITEM_H
#define GROUNDITEM_H

#include "Vector2.h"
#include "Item.h"
#include <iostream>

using namespace std;
class GroundItem {
private:
	Vector2 position;
	Item item;
	int count = 0;
public:
	GroundItem(){}
	GroundItem(Item newItem, Vector2 newPosition, int newCount) {
		position = newPosition;
		item = newItem;
		count = newCount;
	}
	~GroundItem() {}
	void setPosition(Vector2 newPosition) { position = newPosition; }
	Vector2 getPosition() { return position; }
	void setItem(Item newItem) { item = newItem; }
	Item getItem() { return item; }
	string getItemName() { return item.getName(); }
	string getTextureName() { return item.getTextureName(); }
	int getCount() { return count; }
	void addToCount(int toAdd) { count += toAdd; }
	void setCount(int newCount) { count = newCount; }
};
#endif