#include "Game.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include "wtypes.h"
#include <fstream>
#include <string>
#include <array>
#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>
#include <time.h>
#include <filesystem>
#include <fstream>
#include <chrono>
#include <thread>

#include "Character.h"
#include "Animal.h"
#include "Vector2.h"
#include "Image.h"
#include "KeyManager.h"
#include "Button.h"
#include "Zombie.h"
#include "Grids.h"
#include "Zone.h"
#include "StockpileItem.h"

using namespace std;


//Returns a Vector2 representing the monitor's resolution as (width, height)
Vector2 Game::GetDesktopResolution()
{
    RECT desktop;
    const HWND hDesktop = GetDesktopWindow();
    GetWindowRect(hDesktop, &desktop);
    // desktop.right is the monitor's width, and desktop.bottom is the monitor's height
    Vector2 v = Vector2(desktop.right, desktop.bottom);
    return v;
}

Game::Game() {
    GridItem grid = GridItem();
    int size = sizeof(grid);
    float sizeInMegabytes = (float)(size * gridSize * gridSize * 0.000001);
    sizeInMegabytes = (float)((int)(sizeInMegabytes * 100 + .5)) / 100;
    cout << "Total map size of a " << gridSize << "x" << gridSize << " grid: " << sizeInMegabytes << "MB" << endl;
    start();
}

void Game::addTexture(string textureName) {
    textures[currentTextureIndex] = Texture();
    textures[currentTextureIndex++].load(textureName, rescaleSize);
}

Vector2 Game::addVectors(Vector2 vector1, Vector2 vector2) {
    return Vector2(vector1.x + vector2.x, vector1.y + vector2.y);
}
bool Game::rnd(float chance) {
    float value = rand() % 100;
    if (chance >= value)
        return true;
    return false;
}
int Game::rndNum(int maxNum) {
    return rand() % maxNum;
}
/*int Game::binaryTextureSearch(int start, int end, string value) {
    if (start <= end) {
        int mid = (start + (end - start) / 2);
        if (textures[mid].textureName == value)
            return mid;
        if (textures[mid].textureName > value)
            return binaryTextureSearch(start, mid - 1, value);
        return binaryTextureSearch(mid + 1, end, value);

    }
}*/
void Game::addItem(string itemName, string* craftingDependencies, int* craftingDependenciesAmount, int craftingItemsCount, string textureName) {
    items[currentItemIndex++] = Item(itemName, craftingDependencies, craftingDependenciesAmount, craftingItemsCount, textureName);
}
Item& Game::getItemByName(string itemName) {
    for (int i = 0; i < currentItemIndex; i++) {
        if (items[i].getName() == itemName) {
            return items[i];
        }
    }
}
Image& Game::getImageByName(string name) {
    for (int i = 0; i < currentTextureIndex; i++) {
        if (textures[i].textureName == name) {
            if(textures[i].getImage() != NULL)
                return *textures[i].getImage();
        }
    }
}
bool Game::positionWithinBounds(int position) {
    return position >= 0 && position < gridSize;
}
string* Game::getItemDrops(string structureName) {
    string* items = new string[4];
    return items;
}
void Game::drawZone(sf::RenderWindow& window, Zone zone, sf::Color color) {
    zoneBase.setFillColor(color);
    zoneBase.setPosition(zone.getPosition().x * blockSize + offset.x, zone.getPosition().y * blockSize + offset.y);
    zoneBase.setSize(sf::Vector2f(zone.getDimensions().x * blockSize, zone.getDimensions().y * blockSize));
    window.draw(zoneBase);
    zoneText.setString(zone.getZoneName());
    sf::Vector2f center = sf::Vector2f(zone.getPosition().x * blockSize + offset.x + (zone.getDimensions().x * blockSize * 0.5), zone.getPosition().y * blockSize + offset.y + (zone.getDimensions().y * blockSize * 0.5));
    sf::Vector2f zoneTextPosition = sf::Vector2f(center.x - zoneText.getLocalBounds().width * 0.5, center.y - zoneText.getLocalBounds().height * 0.5);
    zoneText.setPosition(zoneTextPosition);
    window.draw(zoneText);
}
void Game::generateWorld() {
    srand(time(NULL));
    for (int x = 0; x < gridSize; x++) {
        for (int y = 0; y < gridSize; y++) {
            /*if (rnd(18))
                world.setSpace(x, y, new GridItem("dirt", Vector2(x, y)));
            else*/
                world.setSpace(x, y, new GridItem("grass", Vector2(x, y)));
            //75% chance that something will be placed here
            if (rand() % 100 <= 75) {
                
                //10% chance to spawn basic foliage
                if (rnd(10)) {
                    int value = rndNum(100);
                    if (world.getSpace(x, y).canBuild()) {
                        if (value <= 33)
                            world.getSpace(x, y).buildItem("bush");
                        else if (value <= 67)
                            world.getSpace(x, y).buildItem("orange-flower");
                        else if (value <= 100)
                            world.getSpace(x, y).buildItem("venus-flytrap");
                    }
                }
                //35% chance to build a tree
                else if (rnd(35)) {
                    if (world.getSpace(x, y).canBuild()) {
                        if (rnd(50))
                            world.getSpace(x, y).addTree("basic");
                        else
                            world.getSpace(x, y).addTree("pine");
                    }
                }
                //4% chance to generate boulder
                else if (rnd(4)) {
                    if (world.getSpace(x, y).canBuild()) {                 
                        int value = rndNum(100);
                        if (rnd(78))
                            if (value <= 33)
                                world.getSpace(x, y).addBoulder("boulder");
                            else if (value <= 67)
                                world.getSpace(x, y).addBoulder("boulder-2");
                            else
                                world.getSpace(x, y).addBoulder("boulder-3");
                        else
                            world.getSpace(x, y).addBoulder("iron-boulder");
                    }
                //0.0001% chance to generate a pond
                }else if (rnd(0.00001)) {
                    world.getSpace(x, y).deconstruct();
                    world.getSpace(x, y).setTexture("water");
                    int size = ((int)(rndNum(13) / 2) + 2);
                    if (size % 2 != 0) size += 1;
                    int side = size / 2;
                    int currentHeight = 0;
                    //bool isVertical = rnd(50);
                    int height = 0;
                    for (int newX = x - side; newX <= x + side; newX++) {
                        if (newX < x) height += 2;
                        else if (newX >= x) height -= 2;
                        for (int newY = y - height / 2; newY <= y + height / 2; newY++) {
                            if (positionWithinBounds(newX) && positionWithinBounds(newY)) {
                                world.getSpace(newX, newY).setTexture("water");
                                world.getSpace(newX, newY).deconstruct();
                            }
                        }
                    }
                }
            }
        }
    }
    int x, y;
    for (x = 3; x < 7; x++) {
        for (y = 3; y < 7; y++) {
            world.getSpace(x, y).deconstruct();
            if (world.getSpace(x, y).getTextureName() == "water")
                world.getSpace(x, y).setTexture("grass");
        }
    }
    for (x = 0; x < gridSize; x++) {
        for (y = 0; y < gridSize; y++) {
            if (world.getSpace(x, y).getTextureName() == "water") {
                if (x > 0 && world.getSpace(x - 1, y).getTextureName() != "water") {
                    world.getSpace(x - 1, y).setTexture("sand");
                    world.getSpace(x - 1, y).deconstruct();
                }
                if (y > 0 && world.getSpace(x, y - 1).getTextureName() != "water") {
                    world.getSpace(x, y - 1).setTexture("sand");
                    world.getSpace(x, y - 1).deconstruct();
                }
                if (x < gridSize - 1 && world.getSpace(x + 1, y).getTextureName() != "water") {
                    world.getSpace(x + 1, y).setTexture("sand");
                    world.getSpace(x + 1, y).deconstruct();
                }
                if (y < gridSize - 1 && world.getSpace(x, y + 1).getTextureName() != "water") {
                    world.getSpace(x, y + 1).setTexture("sand");
                    world.getSpace(x, y + 1).deconstruct();
                }
            }
        }
    }
    for (x = 0; x < gridSize; x++) {
        for (y = 0; y < gridSize; y++) {
            if (x < gridSize - 1 && x > 0) {
                if (world.getSpace(x, y).getTextureName() != "water") {
                    if (world.getSpace(x - 1, y).getTextureName() == "sand" && world.getSpace(x + 1, y).getTextureName() == "sand") {
                        world.getSpace(x, y).setTexture("sand");
                        world.getSpace(x, y).deconstruct();
                    }
                }
            }
            if (y < gridSize - 1 && y > 0) {
                if (world.getSpace(x, y).getTextureName() != "water") {
                    if (world.getSpace(x, y - 1).getTextureName() == "sand" && world.getSpace(x, y + 1).getTextureName() == "sand") {
                        world.getSpace(x, y).setTexture("sand");
                        world.getSpace(x, y).deconstruct();
                    }
                }
            }
        }
    }
    generatedWorld = true;
}
void Game::addCraftableItem(string itemName) {
    cout << "Adding" << itemName << endl;
    if(craftableItemsIndex < craftableItemsCount)
        craftableItems[craftableItemsIndex++] = itemName;
}
void Game::manageClick(string function) {
    managedMouseDown = true;
    if (function == "back to game")
        escaped = false;
    else if (function == "quit")
        preparingCloseWindow = true;
    else if (function == "Orders")
        ordersOpen = true;
    else if (function == "Craft")
        craftingOpen = true;
    else if (function == "settings")
        settingsOpen = true;
    else if (function == "apply") {
        settingsOpen = false;
        saveSettings();
    }
    else if (function == "cancel") {
        settingsOpen = false;
        escaped = false;
        creatingZone = false;
        preparingCloseWindow = false;
        ordersOpen = false;
    }
    else if (function == "change-refresh-rate") {
        currentRefreshRateIndex++;
        if (currentRefreshRateIndex >= sizeof(availibleRefreshrates) / sizeof(availibleRefreshrates[0]))
            currentRefreshRateIndex = 0;
        changeFramerateLimit = true;
    }
    else if (function == "change-volume-level") {
        currentVolumeLevelIndex++;
        if (currentVolumeLevelIndex >= sizeof(volumeLevels) / sizeof(volumeLevels[0]))
            currentVolumeLevelIndex = 0;
        changedVolume = true;
    }
    else if (function == "chop")
        order = "chop";
    else if (function == "mine")
        order = "mine";
    else if (function == "cut")
        order = "cut";
    else
        cout << "Function: " << function << " has not been handled! Please change this in the manageClick function" << endl;
}
void Game::manageSetting(string settingName, string settingValue) {
    if (settingName == "refresh-rate-index") {
        currentRefreshRateIndex = stoi(settingValue);
    }
    else if (settingName == "volume-index") {
        currentVolumeLevelIndex = stoi(settingValue);
        changedVolume = true;
    }
}
void Game::saveSettings() {
    cout << "Saving..." << endl;
    ofstream outputFile;
    outputFile.open(settingsSaveLocation, ofstream::out, ofstream::trunc);
    outputFile << "refresh-rate-index:" + to_string(currentRefreshRateIndex) << endl;
    outputFile << "volume-index:" + to_string(currentVolumeLevelIndex) << endl;
    outputFile.close();
}
void Game::loadSettings() {
    ifstream inputFile;
    inputFile.open(settingsSaveLocation);
    if (inputFile.good()) {
        string str;
        while (getline(inputFile, str)) {
            if (str.find(':') != string::npos) {
                manageSetting(str.substr(0, str.find(":")), str.substr(str.find(":") + 1, str.length()));
            }
        }
    }
    else {
        ofstream createFile;
        createFile.open(settingsSaveLocation);
        createFile << "refresh-rate-index:0" << endl;
        createFile.close();
    }
    inputFile.close();
}
void Game::loadResources() {
    int i, j, min;
    string temp;
    for (i = 0; i < textureLength - 1; i++) {
        min = i;
        for (j = i + 1; j < textureLength; j++)
            if (textureNames[j] < textureNames[min])
                min = j;
        temp = textureNames[i];
        textureNames[i] = textureNames[min];
        textureNames[min] = temp;
    }
    for (int j = 0; j < textureLength; j++) {
        addTexture(textureNames[j]);
    }
    addItems();
}
void Game::addItems() {
    //Adds all items to the game
    addItem("log", NULL, NULL, 0, "log");
    addItem("stone-shard", NULL, NULL, 0, "stone-shard");
    addItem("iron-shard", NULL, NULL, 0, "iron-shard");
    
    //Takes 8 lines to add each craftable item... Will need to be changed somehow
    string* a = new string[2];
    int* b = new int[2];
    a[0] = "log";
    a[1] = "stone-shard";
    b[0] = 20;
    b[1] = 2;
    addItem("chest", a, b, 2, "chest");
    addCraftableItem("chest");

    //Adds all items that can be stored in your stockpile
    addStockpileItem("log");
    addStockpileItem("stone-shard");
    addStockpileItem("iron-shard");
}
void Game::addStockpileItem(string itemName) {
    stockpile[currentStockpileIndex++] = StockpileItem(itemName, 0);
}
void Game::resolutionCalculations() {
    resolution = GetDesktopResolution();
    screenSize = Vector2(resolution.x * 0.75, resolution.y * 0.75);
    rescaleSize = screenSize.x / blocksPerScreen;
    blockSize = rescaleSize;
}
void buildWall(string wallType) {

}
void Game::uiSetup() {
    int _width = screenSize.x * 0.1;
    int _height = screenSize.y * 0.045;
    int yPos = screenSize.y - _height * 6 - 2;
    for (int y = 0; y < 6; y++) {
        for (int x = 0; x < 2; x++) {
            int index = (y * 2) + x;
            buildButtons[index] = Button(buildButtonNames[index], Vector2(x * _width + 4, yPos), Vector2(_width, _height), font, buildButtonNames[index], sf::Color(20, 20, 20, 255 * 0.75), 16);
        }
        yPos += _height;
    }
    int pauseMenuButtonWidth = screenSize.x * 0.185;
    iconBackground.setSize(sf::Vector2f(blockSize, blockSize));
    iconBackground.setFillColor(sf::Color(0, 0, 0, 255 * 0.75));
    pauseMenuButtons[0] = Button("Back To Game", Vector2(screenSize.x * 0.5 - pauseMenuButtonWidth * 0.5, screenSize.y * 0.35), Vector2(pauseMenuButtonWidth, screenSize.y * 0.085), font, "back to game", sf::Color(20, 20, 20, 255 * 0.85), 28);     
    pauseMenuButtons[1] = Button("Save Game", Vector2(screenSize.x * 0.5 - pauseMenuButtonWidth * 0.5, screenSize.y * 0.45), Vector2(pauseMenuButtonWidth, screenSize.y * 0.085), font, "save", sf::Color(20, 20, 20, 255 * 0.85), 28);
    pauseMenuButtons[2] = Button("Settings", Vector2(screenSize.x * 0.5 - pauseMenuButtonWidth * 0.5, screenSize.y * 0.55), Vector2(pauseMenuButtonWidth, screenSize.y * 0.085), font, "settings", sf::Color(20, 20, 20, 255 * 0.85), 28);
    pauseMenuButtons[3] = Button("Quit Game", Vector2(screenSize.x * 0.5 - pauseMenuButtonWidth * 0.5, screenSize.y * 0.65), Vector2(pauseMenuButtonWidth, screenSize.y * 0.085), font, "quit", sf::Color(20, 20, 20, 255 * 0.85), 28);
    font.loadFromFile("./fonts/arial.ttf");
    fpsCounter.setFont(font);
    fpsCounter.setString("FPS: ");
    fpsCounter.setCharacterSize(24);
    fpsCounter.setFillColor(sf::Color::White);
    fpsCounter.setStyle(sf::Text::Bold);
    fpsCounter.setOutlineThickness(2);
    fpsCounter.setOutlineColor(sf::Color::Black);
    fpsAverage.setFont(font);
    fpsAverage.setString("FPS: ");
    fpsAverage.setCharacterSize(24);
    fpsAverage.setFillColor(sf::Color::White);
    fpsAverage.setStyle(sf::Text::Bold);
    fpsAverage.setOutlineThickness(2);
    fpsAverage.setOutlineColor(sf::Color::Black);
    fpsAverage.setPosition(sf::Vector2f(0, 35));

    placeholderText.setFont(font);
    placeholderText.setCharacterSize(24);
    placeholderText.setFillColor(sf::Color::White);
    placeholderText.setStyle(sf::Text::Bold);
    placeholderText.setOutlineThickness(2);
    placeholderText.setOutlineColor(sf::Color::Black);

    clickedRect.setSize(sf::Vector2f(0, 0));
    clickedRect.setFillColor(sf::Color(255, 0, 0, 255 * 0.2));

    zoneText.setFont(font);
    zoneText.setCharacterSize(24);
    zoneText.setFillColor(sf::Color::White);
    zoneText.setStyle(sf::Text::Bold);
    zoneText.setOutlineThickness(2);
    zoneText.setOutlineColor(sf::Color::Black);

    yPos = screenSize.y * 0.15;
    for (int i = 0; i < 12; i++) {
        settlerStatText[i].setFont(font);
        settlerStatText[i].setCharacterSize(24);
        settlerStatText[i].setFillColor(sf::Color::White);
        settlerStatText[i].setStyle(sf::Text::Bold);
        settlerStatText[i].setPosition(sf::Vector2f(screenSize.x * 0.25, yPos));
        settlerStatText[i].setString("ALABAMA");

        settlerStatValue[i].setFont(font);
        settlerStatValue[i].setCharacterSize(24);
        settlerStatValue[i].setFillColor(sf::Color::White);
        settlerStatValue[i].setStyle(sf::Text::Bold);
        settlerStatValue[i].setString("1");
        settlerStatValue[i].setPosition(sf::Vector2f(screenSize.x * 0.4, yPos + (settlerStatValue[i].getGlobalBounds().height * 0.25)));
        settlerStatValue[i].setOutlineColor(sf::Color::Black);
        settlerStatValue[i].setOutlineThickness(1);

        settlerStatBars[i].setPosition(sf::Vector2f(screenSize.x * 0.395, yPos));
        settlerStatBars[i].setFillColor(sf::Color(255, 30, 30));
        settlerStatBars[i].setOutlineColor(sf::Color(0, 0, 0));
        settlerStatBars[i].setOutlineThickness(2);
        yPos += screenSize.y * 0.05;
    }

    for (int j = 0; j < orderIconsCount; j++) {
        orderIconPositions[j] = Vector2(j * blockSize * 1.1 + blockSize * 0.1, screenSize.y - blockSize * 1.1);
    }

    for (int k = 0; k < sizeof(settingsButtons) / sizeof(settingsButtons[0]) - 1; k++) {
        settingsButtons[k].setPosition(Vector2(screenSize.x * 0.35, screenSize.y * 0.12 * k + screenSize.y * 0.05));
    }

    settlerTitleText.setFont(font);
    settlerTitleText.setString("Your Settlers:");
    settlerTitleText.setCharacterSize(24);
    settlerTitleText.setFillColor(sf::Color::White);
    settlerTitleText.setStyle(sf::Text::Bold);

    largeSettlerTitleText.setFont(font);
    largeSettlerTitleText.setString("SETTLER NAME");
    largeSettlerTitleText.setCharacterSize(48);
    largeSettlerTitleText.setFillColor(sf::Color::White);
    largeSettlerTitleText.setStyle(sf::Text::Bold);
    largeSettlerTitleText.setPosition(sf::Vector2f(screenSize.x * 0.25, 0));
    largeSettlerTitleText.setOutlineColor(sf::Color::Black);
    largeSettlerTitleText.setOutlineThickness(2);

    loadingText.setFont(font);
    loadingText.setString("loading...");
    loadingText.setCharacterSize(32);
    loadingText.setFillColor(sf::Color::White);
    loadingText.setStyle(sf::Text::Bold);

    itemCount.setFont(font);
    itemCount.setString("2x");
    itemCount.setCharacterSize(16);
    itemCount.setFillColor(sf::Color::White);
    itemCount.setStyle(sf::Text::Bold);

    settler1Name.setFont(font);
    settler1Name.setString("The quick brown fox jumped over the lazy dogs");
    settler1Name.setCharacterSize(32);
    settler1Name.setFillColor(sf::Color::White);
    settler1Name.setStyle(sf::Text::Bold);
    settler1Name.setPosition(sf::Vector2f(blockSize * 1.2, screenSize.y * 0.1 + blockSize * 0.5 - settler1Name.getLocalBounds().height * 0.5));

    settler2Name.setFont(font);
    settler2Name.setString("The quick brown fox jumped over the lazy dogs");
    settler2Name.setCharacterSize(32);
    settler2Name.setFillColor(sf::Color::White);
    settler2Name.setStyle(sf::Text::Bold);
    settler2Name.setPosition(sf::Vector2f(blockSize * 1.2, screenSize.y * 0.3 + blockSize * 0.5 - settler2Name.getLocalBounds().height * 0.5));

    settler3Name.setFont(font);
    settler3Name.setString("The quick brown fox jumped over the lazy dogs");
    settler3Name.setCharacterSize(32);
    settler3Name.setFillColor(sf::Color::White);
    settler3Name.setStyle(sf::Text::Bold);
    settler3Name.setPosition(sf::Vector2f(blockSize * 1.2, screenSize.y * 0.5 + blockSize * 0.5 - settler3Name.getLocalBounds().height * 0.5));
}
void Game::updateStatsDisplay() {
    settler1Name.setString(settlers[0].getName());
    settler2Name.setString(settlers[1].getName());
    settler3Name.setString(settlers[2].getName());
    for (int j = 0; j < 12; j++) {
        largeSettlerTitleText.setString(settlers[settlerDisplayIndex].getName());
        settlerStatText[j].setString(settlers[settlerDisplayIndex].getSkillByIndex(j).getSkillName() + ":");
        settlerStatValue[j].setString(to_string(settlers[settlerDisplayIndex].getSkillByIndex(j).getSkillValue()));
        settlerStatBars[j].setSize(sf::Vector2f(settlers[settlerDisplayIndex].getSkillByIndex(j).getSkillValue() * screenSize.y * 0.025, blockSize * 0.5));
    }
}
void Game::drawSettlerScene(sf::RenderWindow& window, sf::RectangleShape coverSettlerNames) {
    window.clear(sf::Color(60, 60, 60));
    int currentY = screenSize.y * 0.1;
    window.draw(coverSettlerNames);
    settlerTitleText.setString("Your Settlers:");
    settlerTitleText.setPosition(sf::Vector2f(20, screenSize.y * 0.025));
    sf::RectangleShape mouseRect = sf::RectangleShape(sf::Vector2f(1, 1));
    mouseRect.setPosition(sf::Vector2f(mousePosition.x, mousePosition.y));
    settler1Box->setFillColor(sf::Color(30, 30, 30));
    settler2Box->setFillColor(sf::Color(30, 30, 30));
    settler3Box->setFillColor(sf::Color(30, 30, 30));
    if (mouseClicked && settlerDisplayIndex != -1) {
        if (mouseRect.getGlobalBounds().intersects(rerollStatsButton.rect.getGlobalBounds())) {
            settlers[settlerDisplayIndex].rerollStats();
            updateStatsDisplay();
        }
    }
    if (mouseClicked) {
        if (mouseRect.getGlobalBounds().intersects(startGame.rect.getGlobalBounds())) {
            choosingSettlers = false;
            return;
        }
    }
    if (mouseRect.getGlobalBounds().intersects(settler1Box->getGlobalBounds())) {
        settler1Box->setFillColor(sf::Color(50, 50, 50));
        if (mouseClicked) {
            settlerDisplayIndex = 0;
            updateStatsDisplay();
        }
    }
    if (mouseRect.getGlobalBounds().intersects(settler2Box->getGlobalBounds())) {
        settler2Box->setFillColor(sf::Color(50, 50, 50));
        if (mouseClicked) {
            settlerDisplayIndex = 1;
            updateStatsDisplay();
        }
    }
    if (mouseRect.getGlobalBounds().intersects(settler3Box->getGlobalBounds())) {
        settler3Box->setFillColor(sf::Color(50, 50, 50));
        if (mouseClicked) {
            settlerDisplayIndex = 2;
            updateStatsDisplay();
        }
    }
    if (settlerDisplayIndex != -1) {
        rerollStatsButton.draw(window);
        window.draw(largeSettlerTitleText);
        for (int i = 0; i < 12; i++) {
            window.draw(settlerStatBars[i]);
            window.draw(settlerStatText[i]);
            window.draw(settlerStatValue[i]);
        }
    }
    window.draw(settlerTitleText);
    window.draw(*settler1Box);
    window.draw(*settler2Box);
    window.draw(*settler3Box);
    for (int i = 0; i < currentSettlerCount; i++) {
        if (settlers[i].getIsMale())
            getImageByName("settler").draw(window, Vector2(0, currentY));
        else
            getImageByName("settlerFemale").draw(window, Vector2(0, currentY));

        currentY += screenSize.y * 0.2;
    }
    window.draw(settler1Name);
    window.draw(settler2Name);
    window.draw(settler3Name);
    startGame.draw(window);
    //window.draw(fpsCounter);
}
void Game::start() {
    resolutionCalculations();
    loadResources();
    uiSetup();
    loadSettings();
    srand(time(NULL));
    settlers[0].rerollStats();
    settlers[0].setPosition(Vector2(0, 0));
    settler1Name.setString(settlers[0].getName());
    settlers[1].rerollStats();
    settlers[0].setPosition(Vector2(1, 0));
    settler2Name.setString(settlers[1].getName());
    settlers[2].rerollStats();
    settlers[0].setPosition(Vector2(2, 0));
    settler3Name.setString(settlers[2].getName());
    sf::RenderWindow window(sf::VideoMode(screenSize.x, screenSize.y), "RPG");
    sf::RectangleShape coverSettlerNames(sf::Vector2f(screenSize.x * 0.15, screenSize.y));
    settler1Box = new sf::RectangleShape(sf::Vector2f(screenSize.x * 0.15, blockSize + screenSize.y * 0.02));
    settler2Box = new sf::RectangleShape(sf::Vector2f(screenSize.x * 0.15, blockSize + screenSize.y * 0.02));
    settler3Box = new sf::RectangleShape(sf::Vector2f(screenSize.x * 0.15, blockSize + screenSize.y * 0.02));
    settler1Box->setPosition(sf::Vector2f(0, screenSize.y * 0.09));
    settler2Box->setPosition(sf::Vector2f(0, screenSize.y * 0.29));
    settler3Box->setPosition(sf::Vector2f(0, screenSize.y * 0.49));

    coverSettlerNames.setFillColor(sf::Color(30, 30, 30));
    window.setFramerateLimit(availibleRefreshrates[currentRefreshRateIndex]);
    thread generatingThread([this] { this->generateWorld(); });
    cout << "Setup time: " << clock.restart().asMilliseconds() << "ms" << endl;
    while (window.isOpen()) {
        mousePosition = Vector2(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
        if(choosingSettlers || generatedWorld)
            update(window);
        window.clear();
        if (!shuttingDown) {
            if (choosingSettlers)
                drawSettlerScene(window, coverSettlerNames);
            else {
                if (!generatedWorld)
                    drawLoadingScreen(window);
                else {
                    if (!joined) {
                        generatingThread.join();
                        joined = true;
                    }
                    draw(window);
                }
            }
            window.display();
        }
    }
}
string Game::getRandomTooltip() {
    return splashText[rand() % (sizeof(splashText) / sizeof(splashText[0]))];
}
void Game::drawLoadingScreen(sf::RenderWindow& window) {
    if (currentTooltip == "")
        currentTooltip = getRandomTooltip();
    loadingText.setPosition(screenSize.x * 0.05, screenSize.y * 0.925);
    loadingText.setString("Loading. . .");
    window.draw(loadingText);
    loadingText.setString(currentTooltip);
    loadingText.setPosition(0, screenSize.x * 0.5);
    window.draw(loadingText);
}
void Game::windowClosed() {
    shuttingDown = true;
    cout << "Cleaning up memory..." << endl;
    delete settler1Box;
    delete settler2Box;
    delete settler3Box;
    delete world.spaces;
    delete[] pauseMenuButtons;
    delete[] buildButtons;
}
void Game::manageGridItem(int x, int y) {
    if (world.getSpace(x, y).getIsFull() && world.getSpace(x, y).isDestroyed()) {
        cout << "Destroyed " << world.getSpace(x, y).getBuildingTextureName() << endl;
        world.getSpace(x, y).deconstruct();
    }
}
void Game::giveOrders() {
    int* orderIndexes = new int[currentSettlerCount];
    //Iterates over every settler
    for (int i = 0; i < currentSettlerCount; i++) {
        //Skip over that settler if they are already working on completing an order
        if (settlers[i].getHasOrder()) continue;
        //Iterate over all of the orders starting at 0
        for (int j = 0; j < currentOrderIndex; j++) {
            //If that order can be given to the settler
            if (settlers[i].giveOrder(orders[j], &world)) {
                //Start at the order after the current one. Move every order that follows one place to the left
                for (int k = j + 1; k < currentOrderIndex; k++) {
                    orders[k - 1] = orders[k];
                }
                currentOrderIndex--;
                break;
            }
        }
    }
}
void Game::queueOrder(Order order) {
    orders[currentOrderIndex++] = order;
}
void Game::dropResources(int x, int y, string oldTextureName) {
    string dropName = "";
    int amount = 1;
    if (oldTextureName == "pine-tree-bottom" || oldTextureName == "tree-bottom") {
        dropName = "log";
        amount = rndNum(8) + 2;
    }
    else if (oldTextureName == "boulder" || oldTextureName == "boulder-2" || oldTextureName == "boulder-3") {
        dropName = "stone-shard";
        amount = rndNum(8) + 2;
    }
    else if (oldTextureName == "iron-boulder") {
        dropName = "iron-shard";
        amount = rndNum(8) + 2;
    }
    addGroundItem(GroundItem(getItemByName(dropName), Vector2(x, y), amount));


    //Order(string newOrderType, Vector2 newPosition, string newSkillName, int newSkillRequirementLevel) {
    queueOrder(Order("haul", Vector2(x, y), "Fitness", 0));
}
void Game::addGroundItem(GroundItem item) {
    bool found = false;
    for (int i = 0; i < currentGroundItemIndex; i++) {
        if (groundItems[i].getPosition().x == item.getPosition().x && groundItems[i].getPosition().y == item.getPosition().y) {
            if (groundItems[i].getItemName() == item.getItemName()) {
                groundItems[i].addToCount(item.getCount());
                found = true;
                break;
            }
        }
    }
    if (!found) {
        groundItems[currentGroundItemIndex] = item;
        groundItems[currentGroundItemIndex++].setVisible(true);
    }
}
/*void Game::draw(sf::RenderWindow& window) {
    int xStart = floor(offset.x * -1 / blockSize);
    int yStart = floor(offset.y * -1 / blockSize);
    for (int x = xStart; x < xStart + blocksPerScreen + 1; x++) {
        for (int y = yStart; y < (int)(yStart + (screenSize.y / blockSize) + 2); y++) {
            if ((x * blockSize + offset.x > screenSize.x || y * blockSize + offset.y > screenSize.y) || (x * blockSize + offset.x < -blockSize || y * blockSize + offset.y < -blockSize)) {
                continue;
            }
            getImageByName(world.getSpace(x, y).getTextureName()).draw(window, addVectors(world.getSpace(x, y).getGlobalPosition(blockSize), offset));
            if (blendTileEdges)
                for (int i = 0; i < world.getSpace(x, y).getCurrentTextureDetailsCount(); i++)
                    getImageByName(world.getSpace(x, y).getTextureDetailByIndex(i)).draw(window, addVectors(world.getSpace(x, y).getGlobalPosition(blockSize), offset));
            if (world.getSpace(x, y).getIsFull()) {
                getImageByName(world.getSpace(x, y).getBuildingTextureName()).draw(window, addVectors(world.getSpace(x, y).getGlobalPosition(blockSize), offset));
                if (world.getSpace(x, y).getBuildingTextureName() == "pine-tree-bottom")
                    getImageByName("pine-tree-top").draw(window, addVectors(world.getSpace(x, y - 1).getGlobalPosition(blockSize), offset));
            }
            if (y + 1 < gridSize)
                if ((x * blockSize + offset.x > screenSize.x || (y + 1) * blockSize + offset.y > screenSize.y) || (x * blockSize + offset.x < -blockSize || (y + 1) * blockSize + offset.y < -blockSize))
                    if (world.getSpace(x, y + 1).getIsFull() && world.getSpace(x, y + 1).getBuildingTextureName() == "pine-tree-bottom")
                        getImageByName("pine-tree-top").draw(window, addVectors(world.getSpace(x, y).getGlobalPosition(blockSize), offset));
            manageGridItem(x, y);
            if (world.getSpace(x, y).isBeingDestroyed())
                getImageByName("x").draw(window, addVectors(world.getSpace(x, y).getGlobalPosition(blockSize), offset));
        }
    }
    for (int j = 0; j < currentGroundItemIndex; j++) {
        if(groundItems[j].getIsVisible())
            getImageByName(groundItems[j].getTextureName()).draw(window, addVectors(Vector2(groundItems[j].getPosition().x * blockSize, groundItems[j].getPosition().y * blockSize), offset));
        if(positionWithinBounds(groundItems[j].getPosition().y + 1))
            if(world.getSpace(groundItems[j].getPosition().x, groundItems[j].getPosition().y + 1).getIsFull())
                if (world.getSpace(groundItems[j].getPosition().x, groundItems[j].getPosition().y + 1).getBuildingTextureName() == "pine-tree-bottom")
                    getImageByName("pine-tree-top").draw(window, addVectors(Vector2(groundItems[j].getPosition().x * blockSize, groundItems[j].getPosition().y * blockSize), offset));
    }
    drawZone(window, dumpingZone, sf::Color(0, 0, 255, 255 * 0.175));
    for (int i = 0; i < currentSettlerCount; i++) {
        if (settlers[i].getIsMale())
            getImageByName("settler").draw(window, addVectors(settlers[i].getGlobalPosition(blockSize), offset));
        else
            getImageByName("settlerFemale").draw(window, addVectors(settlers[i].getGlobalPosition(blockSize), offset));
    }
    if (!escaped) {
        if (inventoryOpen) {
            placeholderText.setString("Your Items");
            placeholderText.setPosition(screenSize.x * 0.8, screenSize.y * 0.12);
            window.draw(placeholderText);
        }
    }
    if (escaped) {
        if (settingsOpen) {
            for (int i = 0; i < sizeof(settingsButtons) / sizeof(settingsButtons[0]); i++) {
                settingsButtons[i].draw(window);
            }
            for (int j = 0; j < sizeof(settingsNames) / sizeof(settingsNames[0]); j++) {
                string value = "true";
                if (settingsNames[j] == "Tile Blending") {
                    if (!blendTileEdges) value = "false";
                }
                else if (settingsNames[j] == "Refresh Rate") {
                    value = to_string(availibleRefreshrates[currentRefreshRateIndex]);
                }
                else {
                    cout << "Setting: " << settingsNames[j] << " has not been defined!" << endl;
                }
                //settingsButtons[k].setPosition(Vector2(screenSize.x * 0.35, screenSize.y * 0.12 * k + screenSize.y * 0.05));
                placeholderText.setString(settingsNames[j] + ": " + value);
                placeholderText.setPosition(screenSize.x * 0.5, screenSize.y * 0.12 * j + screenSize.y * 0.05);
                window.draw(placeholderText);
            }
        }
        else {
            for (int i = 0; i < pauseMenuButtonsCount; i++) {
                pauseMenuButtons[i].draw(window);
            }
            window.draw(fpsCounter);
            window.draw(fpsAverage);
            return;
        }
    }
    else if(ordersOpen) {
        for (int i = 0; i < orderIconsCount; i++) {
            iconBackground.setPosition(sf::Vector2f(orderIconPositions[i].x, orderIconPositions[i].y));
            window.draw(iconBackground);
            getImageByName(orderIcons[i] + "-icon").draw(window, orderIconPositions[i]);
        }
    }
    else if (craftingOpen) {
        for (int i = 0; i < 8; i++) {
            iconBackground.setPosition(sf::Vector2f(screenSize.x * 0.015, screenSize.y * 0.1 * i + screenSize.y * 0.1));
            window.draw(iconBackground);
            if (i < craftableItemsIndex) {
                cout << "Drawing " << craftableItems[i] << endl;
                getImageByName(craftableItems[i]).draw(window, Vector2(screenSize.x * 0.015, screenSize.y * 0.1 * i + screenSize.y * 0.1));
            }
        }
    }
    else {
        for (int j = 0; j < 12; j++) {
            buildButtons[j].draw(window);
        }
    }
    if (selectedSettler)
        window.draw(settlerTitleText);
    if (mouseDown && order != "") {
        //clickedRect.setPosition(clickedRect.getPosition().x + offset.x, clickedRect.getPosition().y + offset.y);
        window.draw(clickedRect);
    }
    window.draw(fpsCounter);
    window.draw(fpsAverage);
}*/
void Game::draw(sf::RenderWindow& window) {
    int xStart = floor(offset.x * -1 / blockSize);
    int yStart = floor(offset.y * -1 / blockSize);
    int grassIndex = 0;
    for (int x = xStart; x < xStart + blocksPerScreen + 1; x++) {
        for (int y = yStart; y < (int)(yStart + (screenSize.y / blockSize) + 2); y++) {
            if ((x * blockSize + offset.x > screenSize.x || y * blockSize + offset.y > screenSize.y) || (x * blockSize + offset.x < -blockSize || y * blockSize + offset.y < -blockSize)) {
                continue;
            }
            getImageByName(world.getSpace(x, y).getTextureName()).draw(window, addVectors(world.getSpace(x, y).getGlobalPosition(blockSize), offset));
            if (world.getSpace(x, y).getIsFull()) {
                getImageByName(world.getSpace(x, y).getBuildingTextureName()).draw(window, addVectors(world.getSpace(x, y).getGlobalPosition(blockSize), offset));
                if (world.getSpace(x, y).getBuildingTextureName() == "pine-tree-bottom")
                    getImageByName("pine-tree-top").draw(window, addVectors(world.getSpace(x, y - 1).getGlobalPosition(blockSize), offset));
            }
            if (y + 1 < gridSize)
                if ((x * blockSize + offset.x > screenSize.x || (y + 1) * blockSize + offset.y > screenSize.y) || (x * blockSize + offset.x < -blockSize || (y + 1) * blockSize + offset.y < -blockSize))
                    if (world.getSpace(x, y + 1).getIsFull() && world.getSpace(x, y + 1).getBuildingTextureName() == "pine-tree-bottom")
                        getImageByName("pine-tree-top").draw(window, addVectors(world.getSpace(x, y).getGlobalPosition(blockSize), offset));
            manageGridItem(x, y);
            if (world.getSpace(x, y).isBeingDestroyed())
                getImageByName("x").draw(window, addVectors(world.getSpace(x, y).getGlobalPosition(blockSize), offset));
        }
    }
    drawZone(window, dumpingZone, sf::Color(0, 0, 255, 255 * 0.175));
    for (int j = 0; j < currentGroundItemIndex; j++) {
        if (groundItems[j].getIsVisible()) {
            Vector2 itemPosition = addVectors(Vector2(groundItems[j].getPosition().x * blockSize, groundItems[j].getPosition().y * blockSize), offset);
            getImageByName(groundItems[j].getTextureName()).draw(window, itemPosition);
            itemCount.setString(to_string(groundItems[j].getCount()) + "x");
            sf::Vector2f position(itemPosition.x + blockSize * 0.5 - itemCount.getGlobalBounds().width * 0.5, itemPosition.y);
            itemCount.setPosition(position);
            window.draw(itemCount);
        }
        if (positionWithinBounds(groundItems[j].getPosition().y + 1))
            if (world.getSpace(groundItems[j].getPosition().x, groundItems[j].getPosition().y + 1).getIsFull())
                if (world.getSpace(groundItems[j].getPosition().x, groundItems[j].getPosition().y + 1).getBuildingTextureName() == "pine-tree-bottom")
                    getImageByName("pine-tree-top").draw(window, addVectors(Vector2(groundItems[j].getPosition().x * blockSize, groundItems[j].getPosition().y * blockSize), offset));
    }
    for (int i = 0; i < currentSettlerCount; i++) {
        if (settlers[i].getIsMale())
            getImageByName("settler").draw(window, addVectors(settlers[i].getGlobalPosition(blockSize), offset));
        else
            getImageByName("settlerFemale").draw(window, addVectors(settlers[i].getGlobalPosition(blockSize), offset));
    }
    if (!escaped) {
        if (inventoryOpen) {
            placeholderText.setString("Your Items");
            placeholderText.setPosition(screenSize.x * 0.8, screenSize.y * 0.12);
            window.draw(placeholderText);
            int currentY = screenSize.y * 0.18;
            sf::RectangleShape bg;
            bg.setFillColor(sf::Color(0, 0, 0, 255 * 0.5));
            bg.setSize(sf::Vector2f(screenSize.x * 0.2, screenSize.y * 0.04));
            for (int i = 0; i < currentStockpileIndex; i++) {
                if (stockpile[i].value >= 0) {
                    bg.setPosition(sf::Vector2f(screenSize.x * 0.78, currentY - screenSize.y * 0.01));
                    window.draw(bg);
                    placeholderText.setString(to_string(stockpile[i].value) + "x");
                    placeholderText.setPosition(screenSize.x * 0.85, currentY - screenSize.y * 0.005);
                    window.draw(placeholderText);
                    getImageByName(stockpile[i].name).draw(window, Vector2(screenSize.x * 0.8, currentY - screenSize.y * 0.035));
                    currentY += screenSize.y * 0.0525;
                }
            }
        }
    }
    if (escaped) {
        if (settingsOpen) {
            for (int i = 0; i < sizeof(settingsButtons) / sizeof(settingsButtons[0]); i++) {
                settingsButtons[i].draw(window);
            }
            for (int j = 0; j < sizeof(settingsNames) / sizeof(settingsNames[0]); j++) {
                string value = "true";
                if (settingsNames[j] == "Refresh Rate") {
                    value = to_string(availibleRefreshrates[currentRefreshRateIndex]);
                }
                else if (settingsNames[j] == "Sound Volume") {
                    value = to_string(volumeLevels[currentVolumeLevelIndex]);
                }
                else {
                    cout << "Setting: " << settingsNames[j] << " has not been defined!" << endl;
                }
                placeholderText.setString(settingsNames[j] + ": " + value);
                placeholderText.setPosition(screenSize.x * 0.5, screenSize.y * 0.12 * j + screenSize.y * 0.05);
                window.draw(placeholderText);
            }
        }
        else {
            for (int i = 0; i < pauseMenuButtonsCount; i++) {
                pauseMenuButtons[i].draw(window);
            }
            window.draw(fpsCounter);
            window.draw(fpsAverage);
            return;
        }
    }
    else if (ordersOpen) {
        for (int i = 0; i < orderIconsCount; i++) {
            iconBackground.setPosition(sf::Vector2f(orderIconPositions[i].x, orderIconPositions[i].y));
            window.draw(iconBackground);
            getImageByName(orderIcons[i] + "-icon").draw(window, orderIconPositions[i]);
        }
    }
    else if (craftingOpen) {
        for (int i = 0; i < 8; i++) {
            iconBackground.setPosition(sf::Vector2f(screenSize.x * 0.015, screenSize.y * 0.1 * i + screenSize.y * 0.1));
            window.draw(iconBackground);
            if (i < craftableItemsIndex) {
                cout << "Drawing " << craftableItems[i] << endl;
                getImageByName(craftableItems[i]).draw(window, Vector2(screenSize.x * 0.015, screenSize.y * 0.1 * i + screenSize.y * 0.1));
            }
        }
    }
    else {
        for (int j = 0; j < 12; j++) {
            buildButtons[j].draw(window);
        }
    }
    if (selectedSettler)
        window.draw(settlerTitleText);
    if (mouseDown && order != "") {
        //clickedRect.setPosition(clickedRect.getPosition().x + offset.x, clickedRect.getPosition().y + offset.y);
        window.draw(clickedRect);
    }
    window.draw(fpsCounter);
    window.draw(fpsAverage);
}
bool Game::groundItemAtPosition(Vector2 position) {
    for (int i = 0; i < maxGroundItems; i++) {
        if (groundItems[i].getPosition().x == position.x) {
            if (groundItems[i].getPosition().y == position.y) {
                return true;
            }
        }
    }
    return false;
}
Vector2 Game::getFreeDumpZonePosition(GroundItem item) {
    int startX = dumpingZone.getPosition().x;
    int startY = dumpingZone.getPosition().y;
    bool found = false;
    int x, y;
    for (x = startX; x < startX + dumpingZone.getDimensions().x; x++) {
        for (y = startY; y < startY + dumpingZone.getDimensions().y; y++) {
            if (groundItemExists(Vector2(x, y)) && getGroundItem(Vector2(x, y)).getItemName() == item.getItemName()) {
                found = true;
                break;
            }
            if (!groundItemAtPosition(Vector2(x, y))) {
                found = true;
                break;
            }
        }
        if (found) break;
    }
    return Vector2(x, y);
}
void Game::addToStockpile(GroundItem item) {
    for (int i = 0; i < maxStockpileItems; i++) {
        if (stockpile[i].name == item.getItemName()) {
            stockpile[i].value += item.getCount();
            break;
        }
    }
}
void Game::update(sf::RenderWindow& window) {
    float currentTime = clock.restart().asSeconds();
    deltaTime = currentTime;
    float fps = 1.f / currentTime;
    lastTime = currentTime;
    if (generatedWorld && !choosingSettlers)
        addFpsSample(fps);
    fpsCounter.setString("FPS: " + to_string((int)fps));
    fpsAverage.setString("AVG: " + to_string(currentAverageFps));
    giveOrders();
    mouseClicked = false;
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed || preparingCloseWindow) {
            windowClosed();
            window.close();
            preparingCloseWindow = false;
            return;
        }
        else if (event.type == sf::Event::KeyPressed)
            keys.manageKey(event.key.code, true);
        else if (event.type == sf::Event::KeyReleased)
            keys.manageKey(event.key.code, false);
        else if (event.type == sf::Event::MouseButtonReleased) {
            mouseClicked = true;
            mouseDown = false;
            if (creatingZone) {
                manageZone();
                creatingZone = false;
            }
        }
        else if (event.type == sf::Event::MouseButtonPressed) {
            mouseDown = true;
            managedMouseDown = false;
            creatingZone = false;
        }
    }
    if (changeFramerateLimit) {
        window.setFramerateLimit(availibleRefreshrates[currentRefreshRateIndex]);
        changeFramerateLimit = false;
    }
    if (changedVolume) {
        for (int i = 0; i < maxSoundBuffers; i++) {
            soundObjs[i].setVolume(volumeLevels[currentVolumeLevelIndex]);
        }
        changedVolume = false;
    }
    if (choosingSettlers)
        return;
    if (!ordersOpen) order = "";
    sf::RectangleShape mouseRect = sf::RectangleShape(sf::Vector2f(1, 1));
    sf::RectangleShape localMouseRect = sf::RectangleShape(sf::Vector2f(1, 1));
    localMouseRect.setPosition(sf::Vector2f(mousePosition.x, mousePosition.y));
    mouseRect.setPosition(sf::Vector2f(mousePosition.x + -offset.x, mousePosition.y + -offset.y));
    if (keys.escape)
        escaped = true;
    if (keys.i) {
        keys.manageKey(8, false);
        inventoryOpen = !inventoryOpen;
    }
    if (escaped) {
        if (settingsOpen) {
            for (int i = 0; i < sizeof(settingsButtons) / sizeof(settingsButtons[0]); i++) {
                if (localMouseRect.getGlobalBounds().intersects(settingsButtons[i].rect.getGlobalBounds())) {
                    if (mouseDown && !managedMouseDown)
                        manageClick(settingsButtons[i].command);
                    settingsButtons[i].focus();
                }
                else
                    settingsButtons[i].unfocus();
            }
        }
        else {
            for (int i = 0; i < pauseMenuButtonsCount; i++) {
                if (localMouseRect.getGlobalBounds().intersects(pauseMenuButtons[i].rect.getGlobalBounds())) {
                    if (mouseDown && !managedMouseDown)
                        manageClick(pauseMenuButtons[i].command);
                    pauseMenuButtons[i].focus();
                }
                else
                    pauseMenuButtons[i].unfocus();
            }
        }
        return;
    }

    //Do all mouse collisions such as resources, players, and such before we assume the player is creating a box
    if (mouseDown && !managedMouseDown) {
        sf::RectangleShape settlerRect = sf::RectangleShape(sf::Vector2f(blockSize, blockSize));
        for (int j = 0; j < currentSettlerCount; j++) {
            settlerRect.setPosition(sf::Vector2f(settlers[j].getGlobalPosition(blockSize).x, settlers[j].getGlobalPosition(blockSize).y));
            if (mouseRect.getGlobalBounds().intersects(settlerRect.getGlobalBounds())) {
                managedMouseDown = true;
                selectedSettler = true;
                selectedSettlerIndex = j;
                settlerTitleText.setCharacterSize(48);
                settlerTitleText.setString(settlers[j].getName());
                settlerTitleText.setOutlineColor(sf::Color::Black);
                settlerTitleText.setOutlineThickness(3);
                settlerTitleText.setPosition(sf::Vector2f(screenSize.x * 0.5 - settlerTitleText.getGlobalBounds().width * 0.5, blockSize * 0.01));
                break;
            }
        }
    }
    bool canHighlight = true;
    if (!ordersOpen) {
        for (int j = 0; j < 12; j++) {
            if (localMouseRect.getGlobalBounds().intersects(buildButtons[j].rect.getGlobalBounds())) {
                if (mouseDown && !managedMouseDown) {
                    managedMouseDown = true;
                    manageClick(buildButtons[j].command);
                }
                if (canHighlight) {
                    buildButtons[j].focus();
                    canHighlight = false;
                    continue;
                }
                buildButtons[j].unfocus();
            }
            else
                buildButtons[j].unfocus();
        }
    }
    else {
        sf::RectangleShape collideTest;
        collideTest.setSize(sf::Vector2f(blockSize, blockSize));
        for (int j = 0; j < orderIconsCount; j++) {
            if (mouseDown && !managedMouseDown) {
                collideTest.setPosition(sf::Vector2f(orderIconPositions[j].x, orderIconPositions[j].y));
                if (localMouseRect.getGlobalBounds().intersects(collideTest.getGlobalBounds())) {
                    manageClick(orderIcons[j]);
                }
            }
        }
    }
    if (order != "") {
        if (mouseDown && !managedMouseDown) {
            managedMouseDown = true;
            zoneStartX = (mousePosition.x - offset.x) / blockSize;
            zoneStartY = (mousePosition.y - offset.y) / blockSize;
            clickedRect.setPosition(sf::Vector2f(zoneStartX * blockSize + offset.x, zoneStartY * blockSize + offset.y));
            creatingZone = true;
            selectedSettler = false;
        }
        if (creatingZone) {
            int newX = blockSize * ((int)(mousePosition.x - clickedRect.getPosition().x + blockSize) / blockSize);
            int newY = blockSize * ((int)(mousePosition.y - clickedRect.getPosition().y + blockSize) / blockSize);
            zoneWidth = newX / blockSize;
            zoneHeight = newY / blockSize;
            clickedRect.setSize(sf::Vector2f(newX, newY));
        }    
    }
    if (order == "") {
        float movementAmount = screenSize.x * 0.15 * deltaTime * 1.5;
        if (keys.w)
            offset.y += movementAmount * deltaTime * 300;
        if (keys.s)
            offset.y += -movementAmount * deltaTime * 300;
        if (keys.a)
            offset.x += movementAmount * deltaTime * 300;
        if (keys.d)
            offset.x += -movementAmount * deltaTime * 300;
        offset.x = min(offset.x, 0);
        offset.y = min(offset.y, 0);
        offset.x = max(offset.x, -1 * blockSize * (gridSize - blocksPerScreen));
        offset.y = max(offset.y, -1 * blockSize * (gridSize - blocksPerScreen));
    }
    for (int i = 0; i < currentSettlerCount; i++) {
        settlers[i].update(deltaTime, &world);
        if (settlers[i].getHasOrder() &&  settlers[i].getIsAtOrderLocation()) {
            if (settlers[i].getOrder().getOrderType() == "deconstruct") {
                Vector2 position = settlers[i].getOrder().getPosition();
                long int now = chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now().time_since_epoch()).count();
                if (settlers[i].canHit(now)) {
                    if (world.getSpace(position.x, position.y).hasTree)
                        playSound("wood-chop");
                    else if (world.getSpace(position.x, position.y).hasBoulder)
                        playSound("stone-hit");
                    settlers[i].setLastHit(now);
                    world.getSpace(position.x, position.y).takeDamage(25);
                    if (world.getSpace(position.x, position.y).canBuild()) {
                        settlers[i].completedOrder();
                        dropResources(position.x, position.y, world.getSpace(position.x, position.y).getBuildingTextureName());
                    }
                }
            }
            else if (settlers[i].getOrder().getOrderType() == "haul") {
                if (settlers[i].getIsCarrying()) {
                    Vector2 toGo = getFreeDumpZonePosition(settlers[i].getItem());
                    Vector2 pos = settlers[i].getOrder().getPosition();
                    if (pos.x == toGo.x && pos.y == toGo.y) {
                        GroundItem newItem = settlers[i].drop();
                        newItem.setPosition(*settlers[i].getPosition());
                        addGroundItem(newItem);
                        addToStockpile(newItem);
                        settlers[i].completedOrder();
                    }
                    else {
                        settlers[i].setOrderPosition(toGo, &world);
                    }
                }
                else {
                    groundItemsToRemove[groundItemsToRemoveCount++] = settlers[i].getOrder().getPosition();
                    settlers[i].carry(getGroundItem(settlers[i].getOrder().getPosition()));
                    settlers[i].completedOrder();
                    settlers[i].giveOrder(Order("haul", getFreeDumpZonePosition(settlers[i].getItem()), "Fitness", 0), &world);
                }
            }
        }
    }
    removeGroundItems();
}
void Game::removeGroundItems() {
    for (int i = 0; i < groundItemsToRemoveCount; i++) {
        for (int j = 0; j < currentGroundItemIndex; j++) {
            if (groundItems[j].getPosition().x == groundItemsToRemove[i].x && groundItems[j].getPosition().y == groundItemsToRemove[i].y) {
                for (int k = j + 1; k < currentGroundItemIndex; k++) {
                    groundItems[k - 1] = groundItems[k];
                }
                currentGroundItemIndex--;
                break;
            }
        }
    }
    groundItemsToRemoveCount = 0;
}
void Game::removeGroundItem(Vector2 position) {
    for (int i = 0; i < currentGroundItemIndex; i++) {
        if (groundItems[i].getPosition().x == position.x && groundItems[i].getPosition().y == position.y) {
            groundItems[i].setVisible(false);
            currentGroundItemIndex--;
            cout << "Found" << endl;
            return;
        }
    }
}
GroundItem Game::getGroundItem(Vector2 itemPosition) {
    for (int i = 0; i < currentGroundItemIndex; i++) {
        if (groundItems[i].getPosition().x == itemPosition.x && groundItems[i].getPosition().y == itemPosition.y) {
            return groundItems[i];
        }
    }
}
bool Game::groundItemExists(Vector2 itemPosition) {
    for (int i = 0; i < currentGroundItemIndex; i++) {
        if (groundItems[i].getPosition().x == itemPosition.x && groundItems[i].getPosition().y == itemPosition.y) {
            return true;
        }
    }
    return false;
}
void Game::playSound(string fileName) {
    buffers[currentSoundBuffer].loadFromFile("./sounds/" + fileName + ".wav");
    soundObjs[currentSoundBuffer].setBuffer(buffers[currentSoundBuffer]);
    soundObjs[currentSoundBuffer].play();
    currentSoundBuffer++;
    if (currentSoundBuffer == maxSoundBuffers)
        currentSoundBuffer = 0;
}
void Game::manageZone() {
    for (int x = zoneStartX; x < zoneWidth + zoneStartX; x++) {
        for (int y = zoneStartY; y < zoneHeight + zoneStartY; y++) {
            if (!positionWithinBounds(x) || !positionWithinBounds(y)) continue;
            if (order == "chop") {
                if (world.getSpace(x, y).hasTree) {
                    world.getSpace(x, y).setBeingDestroyed();
                    queueOrder(Order("deconstruct", Vector2(x, y), "Construction", 1));
                }
            }
            else if (order == "mine") {
                if (world.getSpace(x, y).hasBoulder) {
                    world.getSpace(x, y).setBeingDestroyed();
                    queueOrder(Order("deconstruct", Vector2(x, y), "Construction", 1));
                }
            }
        }
    }
    clickedRect.setSize(sf::Vector2f(0, 0));
    order = "";
}