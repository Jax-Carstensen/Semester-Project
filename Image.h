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
    sf::Sprite spriteTop;
    sf::Sprite rotated;
    sf::Sprite flipped;
    sf::Sprite flippedTop;
    int pixelsWide = 32;
    Image() { name = ""; image_name = ""; }
    ~Image() {}
    void load(sf::Texture& texture) {
        sprite.setTexture(texture);
        rotated.setTexture(texture);
        rotated.rotate(90);
        spriteTop = sf::Sprite(texture, sf::IntRect(0, 0, 32, 20));
        flipped = sf::Sprite(texture, sf::IntRect(32, 0, -32, 32));
        flippedTop = sf::Sprite(texture, sf::IntRect(32, 0, -32, 20));
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
        pixelsWide = newSize;
        newSize = newSize / 32.0;
        sf::Vector2f size = sf::Vector2f(newSize, newSize);
        sprite.setScale(size);
        spriteTop.setScale(size);
        rotated.setScale(size);
        flipped.setScale(size);
        flippedTop.setScale(size);
    }
    void drawTopFlipped(sf::RenderWindow& window, Vector2 position) {
        drawWithinBounds(window, position, Vector2(1920, 1080), flippedTop);
    }
    void draw(sf::RenderWindow& window, Vector2 position) {
        drawWithinBounds(window, position, Vector2(1920, 1080), sprite);
    }
    void drawFlipped(sf::RenderWindow& window, Vector2 position) {
        drawWithinBounds(window, position, Vector2(1920, 1080), flipped);
    }
    void drawWithinBounds(sf::RenderWindow& window, Vector2 position, Vector2 dimensions, sf::Sprite& toDraw) {
        if (position.x < dimensions.x && position.y < dimensions.y) {
            if (position.x + pixelsWide > 0 && position.y + pixelsWide > 0) {
                toDraw.setPosition(sf::Vector2f(position.x, position.y));
                window.draw(toDraw);
            }
        }
    }
    void drawTop(sf::RenderWindow& window, Vector2 position) {
        spriteTop.setPosition(sf::Vector2f(position.x, position.y));
        window.draw(spriteTop);
    }
    void drawRotated(sf::RenderWindow& window, Vector2 position) {
        rotated.setPosition(sf::Vector2f(position.x, position.y));
        window.draw(rotated);
    }
};
#endif