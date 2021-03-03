#pragma once
#include <SFML/Graphics.hpp>

using namespace std;

class Button {
public:
    string text;
    Vector2 position;
    Vector2 size;
    sf::RectangleShape rect;
    sf::Text textObj;
    sf::Text textShadow;
    float extraWidth = 0;
    float growValue = 250;
    float maxExtraWidth = 50;
    string command;
    sf::Color background;
    bool resized = false;
    int fontSize = 24;
    bool focused = false;
    Button() {}
    Button(string newText, Vector2 newPosition, Vector2 newSize, sf::Font& font, string newCommand, sf::Color backgroundColor, int newFontSize) {
        fontSize = newFontSize;
        rect.setOutlineColor(sf::Color::White);
        rect.setOutlineThickness(2);
        init(newText, newPosition, newSize, font, newCommand, backgroundColor);
    }
    Button(string newText, Vector2 newPosition, Vector2 newSize, sf::Font& font, string newCommand, sf::Color backgroundColor) {
        init(newText, newPosition, newSize, font, newCommand, backgroundColor);
    }
    ~Button() {}
    void setPosition(Vector2 newPosition) {
        rect.setPosition(newPosition.x, newPosition.y);
        position = newPosition;
        updatePosition();
    }
    void setCommand(string newCommand) {
        command = newCommand;
    }
    void updatePosition() {
        int drawY = (position.y + (size.y * 0.5)) - (0.5 * textObj.getCharacterSize());
        textObj.setPosition(sf::Vector2f(0, drawY));
        textShadow.setPosition(sf::Vector2f(fontSize * 0.125, drawY + (fontSize * 0.125)));
    }
    void init(string newText, Vector2 newPosition, Vector2 newSize, sf::Font& font, string newCommand, sf::Color backgroundColor) {
        background = backgroundColor;
        text = newText;
        position = newPosition;
        size = newSize;
        rect.setSize(sf::Vector2f(size.x, size.y));
        rect.setPosition(sf::Vector2f(position.x, position.y));
        rect.setFillColor(background);
        textObj.setFont(font);
        textObj.setCharacterSize(fontSize);
        textObj.setFillColor(sf::Color::White);
        textObj.setStyle(sf::Text::Bold);
        textObj.setString(newText);
        textObj.setPosition(sf::Vector2f(0, 0));
        textShadow.setFont(font);
        textShadow.setString(newText);
        textShadow.setCharacterSize(fontSize);
        textShadow.setFillColor(sf::Color::Black);
        textShadow.setStyle(sf::Text::Bold);
        updatePosition();
        maxExtraWidth = newSize.x * 0.5;
        growValue = maxExtraWidth * 2;
        command = newCommand;
    }
    void draw(sf::RenderWindow& window) {
        if (!resized) {
            resized = true;
            textObj.setPosition(position.x + (size.x * 0.5) - 0.5 * textObj.getGlobalBounds().width, textObj.getPosition().y);
            textShadow.setPosition(position.x + (size.x * 0.5) - 0.5 * textShadow.getGlobalBounds().width + 3, textShadow.getPosition().y);
        }
        window.draw(rect);
        window.draw(textShadow);
        window.draw(textObj);
    }
    void focus() {
        if (focused) return;
        focused = true;
        rect.setFillColor(sf::Color(55, 55, 55, 255 * 0.75));
    }
    void unfocus() {
        if (!focused) return;
        focused = false;
        rect.setFillColor(background);
    }
    /*
    void focus(float& deltaTime) {
        if (extraWidth >= 50) return;
        extraWidth += deltaTime * growValue;
        if (extraWidth > maxExtraWidth) extraWidth = maxExtraWidth;
        rect.setPosition(sf::Vector2f(position.x - extraWidth, position.y));
        rect.setSize(sf::Vector2f(size.x + extraWidth, size.y));
    }
    void unfocus(float& deltaTime) {
        if (extraWidth <= 0) return;
        extraWidth -= deltaTime * growValue;
        if (extraWidth < 0) extraWidth = 0;
        rect.setPosition(sf::Vector2f(position.x - extraWidth, position.y));
        rect.setSize(sf::Vector2f(size.x + extraWidth, size.y));
    }
    */
};