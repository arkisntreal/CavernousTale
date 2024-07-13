#ifndef CATA_TILE_H_
#define CATA_TILE_H_

#include "globals.h"
#include "graphics.h"

class Tile {
    SDL_Texture* _tileset;
    Vector2i _size;
    Vector2i _tilesetPosition;
    Vector2i _position;
public:
    Tile();
    Tile(SDL_Texture* tileset, Vector2i size, Vector2i tilesetPosition, Vector2i position);

    void draw(Graphics& graphics);
    void update(float elapsedTime);
};

#endif // CATA_TILE_H_
