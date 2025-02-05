#ifndef RAY_H
#define RAY_H

#include "primitives.h"

namespace Primitives {
    class Ray {
        private:
            Point position;
            Direction direction;
        public:
            Ray(const Point& _position, const Direction& _direction);

            Point GetPosition() const {return position;}
            Point GetDirection() const {return direction;}
    };
}

#endif