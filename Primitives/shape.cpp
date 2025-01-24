#include "shape.h"

namespace Primitives {

    Sphere::Sphere(float _radius, Point _position) {
        radius = _radius;
        position = _position;
    }

    IntersectionInfo Sphere::Intersect(const Ray& ray) const {
        return IntersectionInfo();
    }
}