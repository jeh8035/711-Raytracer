#include "primitives.h"

namespace Primitives {
    Point operator*(const algebra::Matrix<float, 4, 4>& m, const Point& a) {
        algebra::Vector4f b = algebra::Vector4f({a.x(), a.y(), a.z(), 1.0}); 

        algebra::Vector4f result = m * b;
        return Point({result.x(), result.y(), result.z()});
    }

    Ray::Ray(const Point& _position, const Direction& _direction) :
        position{_position},
        direction{_direction}
    {}

    Direction ReflectRay(Direction ray, Direction normal) {
        return ray - 2 * ((ray * normal) / normal.norm_squared()) * normal;
    }
}