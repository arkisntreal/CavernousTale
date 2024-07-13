#include "player.h"
#include "animated_sprite.h"
#include "globals.h"

namespace player_constants {
    const float WALK_SPEED = 2.4f;

    const float GRAVITY = 0.8f;
    const float GRAVITY_CAP = 8.f;
}

Player::Player() {}

Player::Player(Graphics& graphics, float x, float y) 
    :   AnimatedSprite(graphics, globals::PROGRAM_PATH + "content/sprites/char_sheet.png",
            0, 0, 16, 16, x, y, 10),
        _dx(0),
        _dy(0),
        _facing(LEFT),
        sprinting(false),
        grounded(true)
{
    this->setupAnimations();
}

void Player::draw(Graphics& graphics) {
    AnimatedSprite::draw(graphics, this->_x, this->_y);
}

void Player::update(float elapsedTime) {
    if (this->_dy <= player_constants::GRAVITY_CAP) {
        this->_dy += player_constants::GRAVITY * elapsedTime;
    }

    this->_x += this->_dx * elapsedTime;
    this->_y += this->_dy * elapsedTime;

    AnimatedSprite::update(elapsedTime);
}

void Player::handleTileCollisions(std::vector<Rectangle>& rectangles) {
    for (int i = 0; i < rectangles.size(); i++) {
        sides::Side collisionSide = Sprite::getCollisionSide(rectangles[i]);
        if (collisionSide != sides::NONE) {
            switch (collisionSide) {
            case sides::LEFT: 
                printf("Collision on the left side of bounding box detected\n");
                this->_x = rectangles[i].getRightSide() + 1;
                break;

            case sides::RIGHT:
                printf("Collision on the right side of bounding box detected\n");
                this->_x = rectangles[i].getLeftSide() - this->getBoundingBox().getWidth() - 1;
                break;

            case sides::TOP:
                printf("Collision on the top side of bounding box detected\n");
                this->_y = rectangles[i].getBottomSide() + 1;
                this->_dy = 0;
                break;

            case sides::BOTTOM:
                // printf("Collision on the bottom side of bounding box detected\n");
                this->_y = rectangles[i].getTopSide() - this->getBoundingBox().getHeight() - 1;
                this->_dy = 0;
                break;

            default:
                break;
            }
        }
    }
}

void Player::handleSlopeCollisions(std::vector<Slope>& slopes) {
    for (int i = 0; i < slopes.size(); i++) {
        int b = (slopes[i].getP1().y - (slopes[i].getSlope() * fabs(slopes[i].getP1().x)));

        int centerX = this->_boundingBox.getCenterX();

        int newY = (slopes[i].getSlope() * centerX) + b - 8;

        if (this->grounded) {
            this->_y = newY - this->_boundingBox.getHeight();
            this->grounded = true;
        }
    }
}

void Player::moveLeft() {
    this->_dx = (sprinting == true) ? -player_constants::WALK_SPEED * 2 : -player_constants::WALK_SPEED;
    this->_facing = LEFT;
    this->playAnimation("RunLeft");
}

void Player::moveRight() {
    this->_dx = (sprinting == true) ? player_constants::WALK_SPEED * 2 : player_constants::WALK_SPEED;
    this->_facing = RIGHT;
    this->playAnimation("RunRight");
}

void Player::stopMoving() {
    this->_dx = 0.0f;
    this->playAnimation(this->_facing == LEFT ? "IdleLeft" : "IdleRight");
}

const float Player::getX() const {
    return this->_x;
}

const float Player::getY() const {
    return this->_y;
}

void Player::animationDone(std::string currentAnimation) {}

void Player::setupAnimations() {
    this->addAnimation(1, 0, 0, "IdleLeft", 16, 16, Vector2i(0, 0));
    this->addAnimation(3, 0, 0, "RunLeft", 16, 16, Vector2i(0, 0));
    // this->addAnimation(1, 0, 0, "RunLeft", 16, 16, Vector2i(0, 0));
    this->addAnimation(1, 0, 16, "IdleRight", 16, 16, Vector2i(0, 0));
    // this->addAnimation(1, 0, 0, "IdleRight", 16, 16, Vector2i(0, 0));
    this->addAnimation(3, 0, 16, "RunRight", 16, 16, Vector2i(0, 0));
    // this->addAnimation(1, 0, 0, "RunRight", 16, 16, Vector2i(0, 0));
}
