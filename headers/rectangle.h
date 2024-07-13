#ifndef CATA_RECTANGLE_H_
#define CATA_RECTANGLE_H_

#include "globals.h"

class Rectangle {
    int _x, _y;
    int _width, _height;
public:
    Rectangle() {}

    Rectangle(int x, int y, int width, int height) :
        _x(x), 
        _y(y), 
        _width(width), 
        _height(height)
    {}

    const int getCenterX() const { return this->_x + this->_width / 2; }
    const int getCenterY() const { return this->_y + this->_height / 2; }

    const int getLeftSide() const { return this->_x; }
    const int getRightSide() const { return this->_x + this->_width; }
    const int getTopSide() const { return this->_y; }
    const int getBottomSide() const { return this->_y + this->_height; }

    const int getWidth() const { return this->_width; }
    const int getHeight() const { return this->_height; }

    const int getSide(const sides::Side side) const {
        return 
            side == sides::LEFT ? this->getLeftSide() :
            side == sides::RIGHT ? this->getRightSide() :
            side == sides::TOP ? this->getTopSide() :
            side == sides::BOTTOM ? this->getBottomSide() :
            sides::NONE;
    }

    const bool collidesWith(const Rectangle& otherRectangle) const {
        return 
            this->getRightSide() >= otherRectangle.getLeftSide() &&
            this->getLeftSide() <= otherRectangle.getRightSide() &&
            this->getTopSide() <= otherRectangle.getBottomSide() &&
            this->getBottomSide() >= otherRectangle.getTopSide();
    }

    const bool isValidRectangle() const {
        return (this->_x >= 0 && this->_y >= 0 && this->_width >= 0 && this->_height >= 0);
    }
};

#endif // CATA_RECTANGLE_H_
