#ifndef CATA_SPRITE_H_
#define CATA_SPRITE_H_

#include <SDL2/SDL.h>
#include <string>

#include "rectangle.h"
#include "graphics.h"

class Sprite {
protected:
    SDL_Rect _sourceRect;
    SDL_Texture* _spriteSheet;

    Rectangle _boundingBox;

    float _x, _y;
public:
    Sprite();
    Sprite(Graphics& graphics, const std::string& filePath, int sourceX, int sourceY,
            int width, int height, float posX, float posY);
    virtual ~Sprite();

    virtual void update();
    void draw(Graphics& graphics, int x, int y);

    const Rectangle getBoundingBox() const;
    const sides::Side getCollisionSide(Rectangle& other) const;
};

#endif // CATA_SPRITE_H_
