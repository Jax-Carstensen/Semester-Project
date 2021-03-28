#ifndef VECTOR2_H
#define VECTOR2_H

class Vector2 {
public:
    int x = 0;
    int y = 0;
    Vector2() { x = 0; y = 0; };
    Vector2(int new_x, int new_y) {
        x = new_x;
        y = new_y;
    };
    ~Vector2() {}
};
class Vector2Float {
public:
    float x = 0;
    float y = 0;
    Vector2Float() { x = 0; y = 0; };
    Vector2Float(float new_x, float new_y) {
        x = new_x;
        y = new_y;
    };
    ~Vector2Float() {}
};
#endif