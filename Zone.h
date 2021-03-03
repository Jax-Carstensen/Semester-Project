#ifndef ZONE_H
#define ZONE_H

#include <iostream>
#include "Vector2.h"

class Zone {
private:
	Vector2 dimensions;
	Vector2 position;
	string zoneName = "";
public:
	Zone() {
		dimensions = Vector2(3, 3);
		position = Vector2(0, 0);
	}
	Zone(Vector2 newDimensions, Vector2 newPosition, string newZoneName) {
		position = newPosition;
		dimensions = newDimensions;
		zoneName = newZoneName;
	}
	~Zone() {}
	void setPosition(Vector2 newPosition) {
		position = newPosition;
	}
	void setDimensions(Vector2 newDimensions) {
		dimensions = newDimensions;
	}
	void setZoneName(string newZoneName) {
		zoneName = newZoneName;
	}
	string getZoneName() { return zoneName; }
	Vector2 getPosition() { return position; }
	Vector2 getDimensions() { return dimensions; }
};

#endif