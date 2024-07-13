#ifndef CATA_GLOBALS_H_
#define CATA_GLOBALS_H_

#include <SDL2/SDL.h>
#include <string>

namespace globals {
    const int SCREEN_WIDTH = 640;
    const int SCREEN_HEIGHT = 480;

    const int SPRITE_SCALE = 2;

    const std::string PROGRAM_PATH = "/home/arc/Projects/CavernousTale/"; 
}

namespace sides {
    enum Side {
        TOP,
        BOTTOM,
        LEFT,
        RIGHT,
        NONE
    };

    inline Side reverseSide(Side side) {
        return 
            side == TOP ? BOTTOM : 
            side == BOTTOM ? TOP :
            side == LEFT ? RIGHT :
            side == RIGHT ? LEFT :
            NONE;
    }
}

enum Direction {
    RIGHT,
    LEFT,
    UP,
    DOWN
};

struct Vector2f {
    float x, y;

    Vector2f() : x(0.0), y(0.0) {}
    Vector2f(float x, float y) : x(x), y(y) {}
};

struct Vector2i {
    int x, y;

    Vector2i() : x(0), y(0) {}
    Vector2i(int x, int y) : x(x), y(y) {}
};

#endif // CATA_GLOBALS_H_
