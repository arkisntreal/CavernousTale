#include "animated_sprite.h"
#include "globals.h"

AnimatedSprite::AnimatedSprite() {}

AnimatedSprite::AnimatedSprite(Graphics& graphics, const std::string& filePath, int sourceX, int sourceY,
    int width, int height, float posX, float posY, float timeToUpdate)
    :   Sprite(graphics, filePath, sourceX, sourceY, width, height, posX, posY),
        _frameIndex(0),
        _timeToUpdate(timeToUpdate),
        _visible(true),
        _currentAnimationOnce(false),
        _currentAnimation("") 
{}

void AnimatedSprite::addAnimation(int frames, int x, int y, std::string name, int width, int height, Vector2i offset) {
    std::vector<SDL_Rect> rectangles;
    for (int i = 0; i < frames; i++) {
        SDL_Rect newRect = { (i + x) * width, y, width, height };
        rectangles.push_back(newRect);
    }
    this->_animations.insert(std::pair<std::string, std::vector<SDL_Rect> >(name, rectangles));
    this->_offsets.insert(std::pair<std::string, Vector2i>(name, offset));
}

void AnimatedSprite::resetAnimations() {
    this->_animations.clear();
    this->_offsets.clear();
}

void AnimatedSprite::stopAnimation() {
    this->_frameIndex = 0;
    this->animationDone(this->_currentAnimation);
}

void AnimatedSprite::setVisible(bool visible) {
    this->_visible = visible;
}

void AnimatedSprite::playAnimation(std::string animation, bool once) {
    this->_currentAnimationOnce = once;
    if (this->_currentAnimation != animation) {
        this->_currentAnimation = animation;
        this->_frameIndex = 0;
    }
}

void AnimatedSprite::update(float elapsedTime) {
    Sprite::update();

    this->_timeElapsed += elapsedTime;
    if (this->_timeElapsed > this->_timeToUpdate) {
        this->_timeElapsed -= this->_timeToUpdate;
        if (this->_frameIndex < this->_animations[this->_currentAnimation].size() - 1) {
            this->_frameIndex++;
        } else {
            if (this->_currentAnimationOnce == true) {
                this->setVisible(false);
            }
            this->_frameIndex = 0;
            this->animationDone(this->_currentAnimation);
        }
    }
}

void AnimatedSprite::draw(Graphics& graphics, int x, int y) {
    if (this->_visible == true) {
        SDL_Rect destinationRectangle;
        destinationRectangle.x = x + this->_offsets[this->_currentAnimation].x;
        destinationRectangle.y = y + this->_offsets[this->_currentAnimation].y;
        destinationRectangle.w = this->_sourceRect.w * globals::SPRITE_SCALE;
        destinationRectangle.h = this->_sourceRect.h * globals::SPRITE_SCALE;

        SDL_Rect sourceRectangle = this->_animations[this->_currentAnimation][this->_frameIndex];

        graphics.blitSurface(this->_spriteSheet, &sourceRectangle, &destinationRectangle);
    }
}

