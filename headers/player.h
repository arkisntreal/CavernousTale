#ifndef CATA_PLAYER_H_
#define CATA_PLAYER_H_

#include "animated_sprite.h"
#include "globals.h"
#include "slope.h"

class Player : public AnimatedSprite {
    float _dx, _dy;

    Direction _facing;
public:
    bool sprinting;
    bool grounded;

    Player();
    Player(Graphics& graphics, float x, float y);

    void draw(Graphics& graphics);
    void update(float elapsedTime);

    void handleTileCollisions(std::vector<Rectangle>& rectangles);
    void handleSlopeCollisions(std::vector<Slope>& slopes);

    void moveLeft();
    void moveRight();
    void stopMoving();

    const float getX() const;
    const float getY() const;

    void animationDone(std::string currentAnimation);
    void setupAnimations();
};

#endif // CATA_PLAYER_H_
