#include "ray.h"

namespace Primitives {
    Ray::Ray(const Point& _position, const Direction& _direction){
        position = _position; 
        direction = _direction;
    }
}