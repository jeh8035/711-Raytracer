#include "shape.h"

namespace Primitives {

    void Shape::Translate(const Point& point) {
        position += point;
    }

    Sphere::Sphere(const float& _radius, const Point& _position) {
        radius = _radius;
        position = _position;
    }

    // Sphere-ray intersection
    IntersectionInfo Sphere::Intersect(const Ray& ray) const {
        IntersectionInfo result = IntersectionInfo();

        float B = 2 * 
            (ray.GetDirection().x() * (ray.GetPosition().x()-position.x()) + 
            ray.GetDirection().y() * (ray.GetPosition().y()-position.y()) +
            ray.GetDirection().z() * (ray.GetPosition().z()-position.z()));

        float C = pow(ray.GetPosition().x()-position.x(), 2) + 
            pow(ray.GetPosition().y()-position.y(), 2) +
            pow(ray.GetPosition().z()-position.z(), 2) - pow(radius, 2);

        float innerQuadratic = pow(B, 2) - 4 * C;

        if (innerQuadratic >= 0) {
            result.hit = true;
        }

        //float solution1 = (-B + innerQuadratic) / 2;
        //float solution2 = (-B - innerQuadratic) / 2;

        return result;
    }
}