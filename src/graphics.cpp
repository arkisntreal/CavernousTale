#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_image.h>
#include <cstring>
#include <sstream>

#include "graphics.h"
#include "globals.h"
#include "tinyxml2.h"

Graphics::Graphics() {
    this->_window = SDL_CreateWindow("Cavernous Tale", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            globals::SCREEN_WIDTH, globals::SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    if (this->_window == NULL) 
        printf("[ERROR](Window Creation) SDL_Error: %s\n", SDL_GetError());
    else
        this->_renderer = SDL_CreateRenderer(this->_window, -1, SDL_RENDERER_ACCELERATED);
}

Graphics::~Graphics() {
    SDL_DestroyWindow(this->_window);
    SDL_DestroyRenderer(this->_renderer);
}

SDL_Surface* Graphics::loadImage(const std::string& filePath) {
    //  Check file type
    if (strstr(filePath.c_str(), ".tsx") != NULL) {
        //  Parsing tileset file to get image source
        tinyxml2::XMLDocument doc;
        doc.LoadFile(filePath.c_str());

        tinyxml2::XMLElement* tilesetNode = doc.FirstChildElement("tileset");
        tinyxml2::XMLElement* pImage = tilesetNode->FirstChildElement("image");
        if (pImage != NULL) {
            while (pImage) {
                std::stringstream ss;
                ss << globals::PROGRAM_PATH << "content/tilesets/";
                
                const char* source;
                pImage->QueryStringAttribute("source", &source);

                ss << source;
                this->_spriteSheets[filePath] = IMG_Load(ss.str().c_str());;

                pImage = pImage->NextSiblingElement("image");
            }
        }
    } else if (strstr(filePath.c_str(), ".png") != NULL) {
        this->_spriteSheets[filePath] = IMG_Load(filePath.c_str());
    }

    return this->_spriteSheets[filePath];
}

void Graphics::blitSurface(SDL_Texture* texture, SDL_Rect* sourceRectangle, SDL_Rect* destinationRectangle) {
    SDL_RenderCopy(this->_renderer, texture, sourceRectangle, destinationRectangle);
}

void Graphics::flip() {
    SDL_RenderPresent(this->_renderer);
}

void Graphics::clear() {
    SDL_RenderClear(this->_renderer);
}

SDL_Renderer* Graphics::getRenderer() const {
    return this->_renderer;
}
