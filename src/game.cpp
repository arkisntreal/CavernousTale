#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <algorithm>

#include "game.h"
#include "graphics.h"
#include "input.h"
#include "player.h"

namespace {
    const int Fps = 60;
    const float MaxFrameTime = 1000.0 / Fps;
}

Game::Game() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        printf("[ERROR](Initializing) SDL_Error: %s\n", SDL_GetError());

    if (!(IMG_Init(IMG_INIT_PNG)))
        printf("[ERROR](Initializing) SDL_Error: %s\n", SDL_GetError());

    this->gameLoop();
}

void Game::gameLoop() {
    Graphics graphics;
    Input input;
    SDL_Event event;

    this->_level = Level("first_cave", graphics);

    this->_player = Player(graphics, this->_level.getSpawnPoint().x, this->_level.getSpawnPoint().y);

    const double TimeStep = (1.0 / Fps) * 1000;
    double accumulator = 0.0;
    double currentTime = SDL_GetTicks();

    bool running = true;
    while (running) {
        float newTime = SDL_GetTicks();
        float frameTime = newTime - currentTime;
        currentTime = newTime; 

        accumulator += frameTime;
        while (accumulator >= TimeStep) {
            input.beginNewFrame();

            while (SDL_PollEvent(&event)) {
                switch (event.type) {
                case SDL_KEYDOWN: {
                    if (event.key.repeat == 0)
                        input.keyDownEvent(event);
                    break;
                }

                case SDL_KEYUP: {
                    input.keyUpEvent(event);
                    break;
                }

                case SDL_QUIT: {
                    running = false;
                    break;
                }
                }
            }

            if (input.wasKeyPressed(SDL_SCANCODE_ESCAPE) == true) {
                running = false;
            }

            //  Handling input for sprinting
            if (input.isKeyHeld(SDL_SCANCODE_LSHIFT) == true) {
                this->_player.sprinting = true;
            } else {
                this->_player.sprinting = false;
            }

            //  Handling input for left-right movement
            if (input.isKeyHeld(SDL_SCANCODE_LEFT) == true) {
                this->_player.moveLeft();
            } else if (input.isKeyHeld(SDL_SCANCODE_RIGHT) == true) {
                this->_player.moveRight();
            }

            if (!input.isKeyHeld(SDL_SCANCODE_LEFT) && !input.isKeyHeld(SDL_SCANCODE_RIGHT)) {
                this->_player.stopMoving();
            }

            //  Debug key, logs info to console when pressing L
            if (input.wasKeyPressed(SDL_SCANCODE_L) == true) {
                printf("x: %f\ny: %f\n", this->_player.getX(), this->_player.getY());
            }

            this->update(std::min<float>(frameTime, MaxFrameTime));
            this->draw(graphics);
            
            accumulator -= TimeStep;
        }
    }
}

void Game::draw(Graphics& graphics) {
    graphics.clear();
    
    this->_level.draw(graphics);
    this->_player.draw(graphics);

    graphics.flip();
}

void Game::update(float elapsedTime) {
    this->_player.update(elapsedTime);
    this->_level.update(elapsedTime);

    //  Checking collisions
    std::vector<Rectangle> tiles;
    tiles = this->_level.checkTileCollisions(this->_player.getBoundingBox());
    if (tiles.size() > 0) {
        this->_player.handleTileCollisions(tiles);
    }
    std::vector<Slope> slopes;
    slopes = this->_level.checkSlopeCollisions(this->_player.getBoundingBox());
    if (slopes.size() > 0) {
        this->_player.handleSlopeCollisions(slopes);
    }
}
