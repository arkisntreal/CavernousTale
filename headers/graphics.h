#ifndef CATA_GRAPHICS_H_
#define CATA_GRAPHICS_H_

#include <SDL2/SDL.h>
#include <string>
#include <map>

class Graphics {
    SDL_Window* _window;
    SDL_Renderer* _renderer;

    std::map<std::string, SDL_Surface*> _spriteSheets;
public:
    Graphics();
    ~Graphics();

    SDL_Surface* loadImage(const std::string& filePath);

    //  Draws a texture to a certain part of the screen
    void blitSurface(SDL_Texture* source, SDL_Rect* sourceRectangle, SDL_Rect* destinationRectangle);

    //  Renders everything to the screen
    void flip();

    //  Clears the screen
    void clear();

    SDL_Renderer* getRenderer() const;
};

#endif // CATA_GRAPHICS_H_
