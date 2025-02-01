#include "shape.h"

namespace Primitives {

    Sphere::Sphere(const TGAColor& _material, const float& _radius, const Point& _position) {
        material = _material;
        radius = _radius;
        position = _position;
    }

    void Sphere::Transform(const algebra::Matrix4f& matrix) {
        algebra::Vector4f new_position = matrix * algebra::Vector4f({position.x(), position.y(), position.z(), 1.0f});
        position = algebra::Vector3f({
            new_position.x(),
            new_position.y(),
            new_position.z()
        });
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
            result.color = material;

            result.rayDist = std::min(
                -B + sqrt(innerQuadratic) / 2,
                -B - sqrt(innerQuadratic) / 2
            );
        }

        return result;
    }

    Triangle::Triangle(const TGAColor& _material, const Point& _vert1, const Point& _vert2, const Point& _vert3) {
        material = _material;
        vert1 = _vert1;
        vert2 = _vert2;
        vert3 = _vert3;
    }

    IntersectionInfo Triangle::Intersect(const Ray& ray) const {
        IntersectionInfo result = IntersectionInfo();

        // Vectors representing triangle
        algebra::Vector3f e1 = vert2 - vert1;
        algebra::Vector3f e2 = vert3 - vert1;

        algebra::Vector3f t = ray.GetPosition() - vert1;
        algebra::Vector3f p;
        ray.GetDirection().cross(e2, p); // cross product

        algebra::Vector3f q;
        t.cross(e1, q); //cross product

        // Barycentric coordinates
        algebra::Vector3f vec = algebra::Vector3f({
            q * e2,
            p * t,
            q * ray.GetDirection()
        });

        algebra::Vector3f wuv = vec * (1.0 / (p * e1));

        if (wuv.x() < 0 || wuv.y() < 0 || wuv.z() < 0 || wuv.y() + wuv.z() > 1.0 ) {
            result.hit = false;
        } else {
            result.hit = true;
            result.color = material;
            result.rayDist = wuv.x();
        }
        return result;
    }
        
    void Triangle::Transform(const algebra::Matrix4f& matrix) {
        algebra::Vector4f new_position1 = matrix * algebra::Vector4f({
            vert1.x(), vert1.y(), vert1.z(), 1.0f
        });
        algebra::Vector4f new_position2 = matrix * algebra::Vector4f({
            vert2.x(), vert2.y(), vert2.z(), 1.0f
        });
        algebra::Vector4f new_position3 = matrix * algebra::Vector4f({
            vert3.x(), vert3.y(), vert3.z(), 1.0f
        });


        vert1 = algebra::Vector3f({new_position1.x(), new_position1.y(), new_position1.z()});
        vert2 = algebra::Vector3f({new_position2.x(), new_position2.y(), new_position2.z()});;
        vert3 = algebra::Vector3f({new_position3.x(), new_position3.y(), new_position3.z()});;
    }

}