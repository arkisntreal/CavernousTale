#ifndef CATA_ANIMATED_SPRITE_H_
#define CATA_ANIMATED_SPRITE_H_

#include <vector>
#include <string>
#include <map>

#include "graphics.h"
#include "globals.h"
#include "sprite.h"

class AnimatedSprite : public Sprite {
    std::map<std::string, std::vector<SDL_Rect> > _animations;
    std::map<std::string, Vector2i> _offsets;

    double _timeElapsed;
    int _frameIndex;
    bool _visible;
protected:
    float _timeToUpdate;
    bool _currentAnimationOnce;
    std::string _currentAnimation;

    void addAnimation(int frames, int x, int y, std::string name, int width, int height, Vector2i offset);

    void resetAnimations();

    void stopAnimation();

    void setVisible(bool visible);

    virtual void animationDone(std::string currentAnimation) = 0;

    virtual void setupAnimations() = 0;
public:
    AnimatedSprite();
    AnimatedSprite(Graphics& graphics, const std::string& filePath, int sourceX, int sourceY,
            int width, int height, float posX, float posY, float timeToUpdate);
    
    void playAnimation(std::string animation, bool once = false);

    void update(float elapsedTime);

    void draw(Graphics& graphics, int x, int y);
};

#endif // CATA_ANIMATED_SPRITE_H_
