#pragma once

#include <SFML/Graphics.hpp>
#include "Image.h";

using namespace std;

class Texture {
public:
	string textureName;
	Image* image;
	sf::Texture texture; 

	void load(string fileName, float rescaleSize) {
		textureName = fileName;
		texture.loadFromFile("./images/" + fileName + ".png");
		image = new Image("grass", texture, rescaleSize);
	}
	Image* getImage() {
		return image;
	}
};