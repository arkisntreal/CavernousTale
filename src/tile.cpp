#include "tile.h"
#include "globals.h"

Tile::Tile() {};

Tile::Tile(SDL_Texture* tileset, Vector2i size, Vector2i tilesetPosition, Vector2i position)
    :   _tileset(tileset),
        _size(size),
        _tilesetPosition(tilesetPosition),
        _position(Vector2i(position.x * globals::SPRITE_SCALE, position.y * globals::SPRITE_SCALE))
{};

void Tile::draw(Graphics& graphics) {
    SDL_Rect sourceRectangle = { this->_tilesetPosition.x, this->_tilesetPosition.y, this->_size.x, this->_size.y };

    SDL_Rect destinationRectangle = { this->_position.x, this->_position.y,
        this->_size.x * globals::SPRITE_SCALE, this->_size.y * globals::SPRITE_SCALE};

    graphics.blitSurface(this->_tileset, &sourceRectangle, &destinationRectangle);
}

void Tile::update(float elapsedTime) {}
