#include "shape.h"

namespace Primitives {

    void Shape::Translate(const Point& point) {
        position += point;
    }

    Sphere::Sphere(const float& _radius, const Point& _position) {
        radius = _radius;
        position = _position;
    }

    IntersectionInfo Sphere::Intersect(const Ray& ray) const {
        return IntersectionInfo();
    }
}