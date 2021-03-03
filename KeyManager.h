#ifndef KEYMANAGER_H
#define KEYMANAGER_H

using namespace std;

class KeyManager {
public:
    bool a = false;
    bool w = false;
    bool s = false;
    bool d = false;
    bool escape = false;
    void manageKey(int keyCode, bool value) {
        switch (keyCode) {
            case 0:  a = value; break;
            case 3:  d = value; break;
            case 22: w = value; break;
            case 18: s = value; break;
            case 36: escape = value; break;
        }
    }
};
#endif