#ifndef GRIDITEM_H
#define GRIDITEM_H
#include <iostream>
#include <string>
#include "Vector2.h"

using namespace std;
class GridItem {
private:
    bool collidable = false;
    string textureName = "";
    string buildingTextureName;
    Vector2 position = Vector2();
    int maxHealth = 0;
    int health = 0;
    bool beingDestroyed = false;
public:
    bool occupied = false;
    bool hasTree = false;
    bool hasBoulder = false;
    bool walkable = true;
    //bool beingDestroyed = false;
    GridItem(string newTextureName, Vector2 newPosition) {
        textureName = newTextureName;
        position = newPosition;
    }
    GridItem() {}
    ~GridItem() {}
    bool getCollidable() { return collidable; }
    void buildItem(string itemName, int itemHealth) {
        health = itemHealth;
        maxHealth = itemHealth;
        if (itemName == "boulder") hasBoulder = true;
        buildingTextureName = itemName;
        occupied = true;
        if (itemName != "plant")
            collidable = true;
    }
    bool canWalk() {
        return walkable;
    }
    bool isDestroyed() {
        return health <= 0;
    }
    void takeDamage(int value) {
        health -= value;
        if (health <= 0) {
            health = 0;
            deconstruct();
        }
    }
    void buildItem(string itemName) {
        health = 100;
        maxHealth = 100;
        if (itemName == "boulder") hasBoulder = true;
        buildingTextureName = itemName;
        occupied = true;
        if (itemName != "plant") {
            collidable = true;
            walkable = false;
        }
    }
    bool canBuild() {
        return !occupied;
    }
    void setTexture(string newTextureName) {
        textureName = newTextureName;
        if (textureName == "water")
            walkable = false;
    }
    void addTree(string treeType) {
        hasTree = true;
        if (treeType == "basic")
            buildItem("tree-bottom");
        else if (treeType == "pine")
            buildItem("pine-tree-bottom");
    }
    void addBoulder(string boulderName) {
        hasBoulder = true;
        buildItem(boulderName);
    }
    Vector2 getPosition() {
        return position;
    }
    bool getIsFull() {
        return occupied;
    }
    Vector2 getGlobalPosition(int size) {
        return Vector2(position.x * size, position.y * size);
    }
    string getTextureName() {
        return textureName;
    }
    string getBuildingTextureName() {
        return buildingTextureName;
    }
    void setBeingDestroyed() {
        beingDestroyed = true;
    }
    bool isBeingDestroyed() {
        return beingDestroyed;
    }
    void deconstruct() {
        beingDestroyed = false;
        occupied = false;
        collidable = false;
        hasTree = false;
        hasBoulder = false;
    }
};
class Grid {
public:
    GridItem** spaces;
    Grid() {
        spaces = new GridItem * [256];
        for (int i = 0; i < 256; i++) {
            spaces[i] = new GridItem[256];
        }
    }
    Grid(int gridSize) {
        spaces = new GridItem * [gridSize];
        for (int i = 0; i < gridSize; i++) {
            spaces[i] = new GridItem[gridSize];
        }
    }
    ~Grid() {}
    GridItem& getSpace(int x, int y) {
        return spaces[x][y];
    }
    void setSpace(int x, int y, GridItem* gridItem) {
        spaces[x][y] = *gridItem;
    }
};
#endif