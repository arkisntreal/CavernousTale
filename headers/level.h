#ifndef CATA_LEVEL_H_
#define CATA_LEVEL_H_

#include <string>
#include <vector>

#include "globals.h"
#include "graphics.h"
#include "rectangle.h"
#include "tile.h"
#include "slope.h"

struct Tileset;

class Level {
    std::string _mapName;

    Vector2i _spawnPoint;
    Vector2i _size;
    Vector2i _tileSize;

    SDL_Texture* _backgroundTexture;

    std::vector<Tile> _tileList;
    std::vector<Tileset> _tilesets;
    std::vector<Rectangle> _collisionRects;
    std::vector<Slope> _collisionSlopes;

    void loadMap(std::string mapName, Graphics& graphics);

public:
    Level();
    Level(std::string mapName, Graphics& graphics);

    void draw(Graphics& graphics);
    void update(float elapsedTime);

    std::vector<Rectangle> checkTileCollisions(const Rectangle& collider);
    std::vector<Slope> checkSlopeCollisions(const Rectangle& collider);
    const Vector2i getSpawnPoint() const;
};

struct Tileset {
    SDL_Texture* texture;
    int width;
    int height;
    int firstGid;

    Tileset() {
        this->firstGid = -1;
    }
    Tileset(SDL_Texture* texture, int firstGid) {
        this->texture = texture;
        if (SDL_QueryTexture(texture, NULL, NULL, &this->width, &this->height) != 0)
            printf("[ERROR](Tileset Construction) SDL_Error: %s\n", SDL_GetError());
        this->firstGid = firstGid;
    }
};

#endif // CATA_LEVEL_H_
