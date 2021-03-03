#ifndef IMAGE_H
#define IMAGE_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include "Vector2.h"

using namespace std;

class Image {
public:
    string name;
    string image_name;
    sf::Sprite sprite;
    Image() { name = ""; image_name = ""; }
    ~Image() {}
    void load(sf::Texture& texture) {
        sprite.setTexture(texture);
    }
    Image(string new_name, sf::Texture& base) {
        name = new_name;
        load(base);
    }
    Image(string new_name, sf::Texture& base, int newSize) {
        name = new_name;
        load(base);
        rescale(newSize);
    }
    void setPosition(Vector2 position) {
        sprite.setPosition(sf::Vector2f(position.x, position.y));
    }
    void rescale(float newSize) {
        newSize = newSize / 32.0;
        sprite.setScale(sf::Vector2f(newSize, newSize));
    }
    void draw(sf::RenderWindow& window, Vector2 position) {
        setPosition(position);
        window.draw(sprite);
    }
};
#endif