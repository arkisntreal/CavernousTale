#ifndef CATA_GAME_H_
#define CATA_GAME_H_

#include "graphics.h"
#include "player.h"
#include "level.h"

class Game {
    void gameLoop();
    void draw(Graphics& graphics);
    void update(float elapsedTime);

    Player _player;

    Level _level;
public:
    Game();
};

#endif // CATA_GAME_H_
