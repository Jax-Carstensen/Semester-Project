#ifndef GAME_H
#define GAME_H

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

#include "Character.h"
#include "Animal.h"
#include "Vector2.h"
#include "Image.h"
#include "KeyManager.h"
#include "Button.h"
#include "Zombie.h"
#include "Grids.h"
#include "Texture.h"
#include "Item.h"
#include "GroundItem.h"
#include "Order.h"
#include "Sound.h"
#include "Zone.h"

using namespace std;

class Game {
private:
    float lastTime = 0;
    sf::Clock clock = sf::Clock();
    const int gridSize = 256;
    const int blocksPerScreen = 20;

    const int pauseMenuButtonsCount = 4;

    const int maxSettlers = 50;
    const int maxAnimals = 100;
    const int maxZombies = 100;
    const int maxTextures = 150;
    const int maxItems = 150;
    const static int maxGroundItems = 250;
    const static int orderIconsCount = 4;
    const static int maxSoundBuffers = 30;
    const static int craftableItemsCount = 100;
    const string settingsSaveLocation = "./settings.txt";
public:
    int currentGroundItemIndex = 0;
    int currentTextureIndex = 0;
    int currentItemIndex = 0;
    int currentSettlerCount = 3;
    int currentMaxRefreshRate = 60;
    int currentOrderIndex = 0;



    Texture* textures = new Texture[maxTextures];
    Item* items = new Item[maxItems];

    Vector2 resolution = GetDesktopResolution();
    Vector2 screenSize = Vector2(resolution.x * 0.75, resolution.y * 0.75);
    Vector2 offset = Vector2();
    Grid world = Grid(gridSize);

    Animal* animals = new Animal[maxAnimals];
    Character* settlers = new Character[maxSettlers];
    Zombie* zombies = new Zombie[maxZombies];
    Vector2 mousePosition;

    float rescaleSize;
    int blockSize;
    KeyManager keys = KeyManager();
    float deltaTime;
    bool mouseClicked = false;
    bool mouseDown = false;
    bool managedMouseDown = false;
    Vector2 clickedPosition;
    sf::RectangleShape clickedRect;
    bool selectedSettler = false;
    int selectedSettlerIndex;
    bool escaped = false;
    bool generatedWorld = false;
    bool craftingOpen = false;
    int craftableItemsIndex = 0;
    string craftableItems[craftableItemsCount];

    sf::Text fpsCounter;
    sf::Text fpsAverage;
    sf::Text placeholderText;
    sf::Font font;

    sf::SoundBuffer buffers[maxSoundBuffers];
    sf::Sound soundObjs[maxSoundBuffers];
    int currentSoundBuffer = 0;

    int currentSoundIndex = 0;

    Order orders[1000];
    GroundItem groundItems[maxGroundItems];

    Zone dumpingZone = Zone(Vector2(4, 4), Vector2(3, 3), "Stockpile Zone");
    sf::RectangleShape zoneBase;
    sf::Text zoneText;

    Button* buildButtons = new Button[12];
    Button* pauseMenuButtons = new Button[pauseMenuButtonsCount];
    string buildButtonNames[12] = {
        "Orders",
        "Craft",  
        "Structure",
        "Production",
        "Furniture",
        "Power",
        "Security",
        "Misc",
        "Floors",
        "Recreation",
        "Ship",
        "Temperature"
    };
    string orderIcons[orderIconsCount] = {
        "cancel",
        "chop",
        "cut",
        "mine"
    };
    Vector2 orderIconPositions[orderIconsCount];
    string tilesToRound[1] = {
        "sand",
        //"dirt"
    };
    string settingsNames[2] = {
        "Tile Blending",
        "Refresh Rate"
    };
    string splashText[3] = {
        "Game not running well? Change some of the options in settings.",
        "Keep your settlers alive! When they die, it's game over.",
        ""
    };
    string currentTooltip;
    /*
    
    Getting average FPS

    */
    int currentFpsSampleIndex = 0;
    int fpsSamples[600];
    int currentAverageFps = 0;

    void addFpsSample(int value) {
        if (currentFpsSampleIndex == 599)
            currentFpsSampleIndex = 0;
        fpsSamples[currentFpsSampleIndex++] = value;
        int totalFps = 0;
        for (int i = 0; i < currentFpsSampleIndex; i++) {
            totalFps += fpsSamples[i];
        }
        currentAverageFps = totalFps / (currentFpsSampleIndex + 1);
    }

    int zoneStartX = 0;
    int zoneStartY = 0;
    int zoneWidth = 0;
    int zoneHeight = 0;

    //Settings
    bool blendTileEdges = true;
    int currentRefreshRateIndex = 0;
    int availibleRefreshrates[7] = {
        60,
        75,
        120,
        144,
        165,
        240,
        360,
    };

    Button settingsButtons[3] = {
        Button("Change", Vector2(screenSize.x * 0.25 - (screenSize.x * 0.125 * 0.5), screenSize.y * 0.1), Vector2(screenSize.x * 0.125, screenSize.y * 0.075), font, "change-tile-blending", sf::Color(30, 30, 30, 255 * 0.85), 28),
        Button("Change", Vector2(screenSize.x * 0.25 - (screenSize.x * 0.125 * 0.5), screenSize.y * 0.3), Vector2(screenSize.x * 0.125, screenSize.y * 0.075), font, "change-refresh-rate", sf::Color(30, 30, 30, 255 * 0.85), 28),
        Button("Apply", Vector2(screenSize.x * 0.5 - (screenSize.x * 0.125 * 0.5), screenSize.y * 0.85), Vector2(screenSize.x * 0.125, screenSize.y * 0.075), font, "apply", sf::Color(30, 30, 30, 255 * 0.85), 28),
    };



    string order = "";
    Order orderQueue[1000];
    bool choosingSettlers = true;
    bool shuttingDown = false;
    bool creatingZone = false;
    bool preparingCloseWindow = false;
    bool ordersOpen = false;
    bool settingsOpen = false;
    bool changeFramerateLimit = false;
    sf::Text settler1Name, settler2Name, settler3Name, settlerTitleText, largeSettlerTitleText;
    int settlerDisplayIndex = -1;
    sf::RectangleShape* settler1Box;
    sf::RectangleShape* settler2Box;
    sf::RectangleShape* settler3Box;
    sf::Text* settlerStatText = new sf::Text[12];
    sf::Text* settlerStatValue = new sf::Text[12]; 
    sf::RectangleShape* settlerStatBars = new sf::RectangleShape[12];
    sf::RectangleShape iconBackground;
    sf::Text loadingText;
    Button rerollStatsButton = Button("Reroll Stats", Vector2(screenSize.x * 0.8, screenSize.y * 0.05), Vector2(screenSize.x * 0.15, screenSize.y * 0.075), font, "reroll", sf::Color(30, 30, 30));
    Button startGame = Button("Start Game", Vector2(screenSize.x * 0.0125, screenSize.y * 0.9), Vector2(screenSize.x * 0.125, screenSize.y * 0.075), font, "reroll", sf::Color(0, 255, 0));

    Game();
    ~Game() {}

    void addTexture(string textureName);
    int rndNum(int maxNum);
    void drawLoadingScreen(sf::RenderWindow& window);
    bool positionWithinBounds(int position);
    void roundTileEdges(int x, int y, string tileName);
    void saveSettings();
    void loadSettings();
    void manageSetting(string settingName, string settingValue);
    void addItem(string itemName, string* craftingDependencies, int* craftingDependenciesAmount, int craftingItemsCount, string textureName);
    Item& getItemByName(string itemName);
    string getRandomTooltip();
    void manageZone();
    string* getItemDrops(string structureName);
    void manageGridItem(int x, int y);
    void giveOrders();
    void queueOrder(Order order);
    void dropResources(int x, int y, string oldTextureName);
    void addGroundItem(GroundItem item);
    void buildWall(string wallType);
    void playSound(string fileName);
    void addSound(string fileName);
    void addCraftableItem(string itemName);
    void addItem();
    void drawZone(sf::RenderWindow& window, Zone zone, sf::Color color);
    Vector2 getFreeDumpZonePosition(GroundItem item);

    Vector2 GetDesktopResolution();
    //Returns a Vector2 containing the width and height of the user's monitor in pixels

    Vector2 addVectors(Vector2 vector1, Vector2 vector2);
    //Returns a new Vector2 by adding the 2 provided ones

    bool rnd(float chance);
    //Given a percent (out of 100) does the calculations and returns true if that chance happens to work

    Image& getImageByName(string name);
    //Returns the image linked to a given file name

    void generateWorld();
    //Generates the entire tilemap including structures, trees, zombies, and ground

    void manageClick(string function);
    //Called when the user clicks and manages all collision between buttons and survivors

    void loadResources();
    //Loads all images and fonts that the game uses

    void resolutionCalculations();
    //Sets the size of all objects in the game

    void uiSetup();
    //Initializes all UI that is sized based upon the screen dimensions

    void updateStatsDisplay();
    //Updates the values and size of all survivor stats bars

    void drawSettlerScene(sf::RenderWindow& window, sf::RectangleShape coverSettlerNames);
    //Draws the survivor choice scene to the screen

    void start();
    //Called to start the game loop and initialize all important variables

    void windowClosed();
    //Called when we want the window to cloes

    void draw(sf::RenderWindow& window);
    //Takes a RenderWindow and draws all objects to the screen

    void update(sf::RenderWindow& window);
    //Takes a RenderWindow and handles all of the physics, movement, and calculations

};
#endif