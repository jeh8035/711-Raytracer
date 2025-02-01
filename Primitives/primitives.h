#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include "../Libraries/matrix/matrix.h"

namespace Primitives {
    typedef algebra::Vector3f Point;
    typedef algebra::Vector3f Direction;

    // Multiply vec3 by mat4x4 (automatically add 1.0 to end of vec3)
    Point operator*(const algebra::Matrix<float, 4, 4>& m, const Point& a);
}

#endif