#include <SDL2/SDL.h>

#include "sprite.h"
#include "graphics.h"
#include "globals.h"

Sprite::Sprite() {}

Sprite::Sprite(Graphics& graphics, const std::string& filePath, int sourceX, int sourceY,
        int width, int height, float posX, float posY) : _x(posX), _y(posY) {
    this->_sourceRect.x = sourceX;
    this->_sourceRect.y = sourceY;
    this->_sourceRect.w = width;
    this->_sourceRect.h = height;

    this->_spriteSheet = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadImage(filePath));
    if (this->_spriteSheet == NULL)
        printf("[ERROR](Sprite Creation) SDL_Error: %s\n", SDL_GetError());
}

Sprite::~Sprite() {}

void Sprite::draw(Graphics& graphics, int x, int y) {
    SDL_Rect destinationRectangle = { x, y, this->_sourceRect.w * globals::SPRITE_SCALE, this->_sourceRect.h * globals::SPRITE_SCALE};
    graphics.blitSurface(this->_spriteSheet, &this->_sourceRect, &destinationRectangle);
}

void Sprite::update() {
    this->_boundingBox = Rectangle(this->_x + 2, this->_y + 2, 
            this->_sourceRect.w * globals::SPRITE_SCALE - 4, this->_sourceRect.h * globals::SPRITE_SCALE - 2);
}

const Rectangle Sprite::getBoundingBox() const {
    return this->_boundingBox;
}

const sides::Side Sprite::getCollisionSide(Rectangle& other) const {
    int amtRight, amtLeft, amtTop, amtBottom;
    amtRight = this->getBoundingBox().getRightSide() - other.getLeftSide();
    amtLeft = other.getRightSide() - this->getBoundingBox().getLeftSide();
    amtTop = other.getBottomSide() - this->getBoundingBox().getTopSide();
    amtBottom = this->getBoundingBox().getBottomSide() - other.getTopSide();
    
    int vals[4] = { abs(amtRight), abs(amtLeft), abs(amtTop), abs(amtBottom) };
    int min = vals[0];
    for (int i = 0; i < 4; i++) {
        if (vals[i] < min) {
            min = vals[i];
        }
    }

    //  If there is a tie, the first one from between the tied numbers will be picked, so TOP and BOTTOM is the first one we check, 
    //  so the player has more leniency - when they land on the corner of a tile, they are placed on the top or bottom of it, not to the side
    return 
        abs(amtTop) == min ? sides::TOP :
        abs(amtBottom) == min ? sides::BOTTOM :
        abs(amtRight) == min ? sides::RIGHT :
        abs(amtLeft) == min ? sides::LEFT :
        sides::NONE;
}
