#include "primitives.h"

namespace Primitives {
    Point operator*(const algebra::Matrix<float, 4, 4>& m, const Point& a) {
        algebra::Vector4f b = algebra::Vector4f({a.x(), a.y(), a.z(), 1.0}); 

        algebra::Vector4f result = m * b;
        return Point({result.x(), result.y(), result.z()});
    }
}