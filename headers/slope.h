#ifndef CATA_SLOPE_H_
#define CATA_SLOPE_H_

#include "globals.h"
#include "rectangle.h"

class Slope {
    Vector2i _p1, _p2;

    float _slope;
public:
    Slope() {}
    Slope(Vector2i p1, Vector2i p2) 
        :   _p1(p1),
            _p2(p2)
    {
        if (this->_p2.x - this->_p1.x != 0) {
            this->_slope = (fabs(this->_p2.y) - fabs(this->_p1.y)) / (fabs(this->_p2.x) - fabs(this->_p1.x));
        }
    }

    const bool collidesWith(const Rectangle& other) const {
        return				
            (other.getRightSide() >= this->_p2.x &&
			other.getLeftSide() <= this->_p1.x &&
			other.getTopSide() <= this->_p2.y &&
			other.getBottomSide() >= this->_p1.y) ||
            (other.getRightSide() >= this->_p1.x &&
            other.getLeftSide() <= this->_p2.x &&
			other.getTopSide() <= this->_p1.y &&
			other.getBottomSide() >= this->_p2.y) ||
			(other.getLeftSide() <= this->_p1.x &&
			other.getRightSide() >= this->_p2.x &&
			other.getTopSide() <= this->_p1.y &&
			other.getBottomSide() >= this->_p2.y) ||
			(other.getLeftSide() <= this->_p2.x &&
			other.getRightSide() >= this->_p1.x &&
			other.getTopSide() <= this->_p2.y &&
			other.getBottomSide() >= this->_p1.y);
    }

    inline const Vector2i getP1() const { return this->_p1; }
    inline const Vector2i getP2() const { return this->_p2; }
    inline const float getSlope() const { return this->_slope; }
};

#endif // CATA_SLOPE_H_
