#include "shape.h"

namespace Primitives {

#pragma region Sphere
    Sphere::Sphere(const TGAColor& _material, const float& _radius, const Point& _position) {
        material = _material;
        radius = _radius;
        position = _position;
    }

    void Sphere::Transform(const algebra::Matrix4f& matrix) {
        position = matrix * position;
    }

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
                (-B + sqrt(innerQuadratic)) / 2,
                (-B - sqrt(innerQuadratic)) / 2
            );
        }

        return result;
    }
#pragma endregion

#pragma region Triangle
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
        algebra::Vector3f p = ray.GetDirection().cross(e2); // cross product

        algebra::Vector3f q = t.cross(e1); //cross product

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
        vert1 = matrix * vert1;
        vert2 = matrix * vert2;
        vert3 = matrix * vert3;
    }
#pragma endregion

#pragma region Cylinder
    Cylinder::Cylinder(const TGAColor& _material, const Point& _endpoint1, const Point& _endpoint2, const float& _radius) {
        material = _material;
        endpoint1 = _endpoint1;
        endpoint2 = _endpoint2;
        radius = _radius;
    }

    IntersectionInfo Cylinder::Intersect(const Ray& ray) const {
        IntersectionInfo result = IntersectionInfo();

        algebra::Vector3f axis = (endpoint2 - endpoint1).normalize();

        algebra::Vector3f n_cross_a = ray.GetDirection().cross(axis);

        float innerQuadratic = n_cross_a.dot(n_cross_a) * powf(radius, 2.0f) - 
            powf(endpoint1 * n_cross_a, 2.0f);


        if (innerQuadratic >= 0.0f) {
            float d = std::min(
                ((n_cross_a * endpoint1.cross(axis)) + std::sqrt(innerQuadratic)) / (n_cross_a * n_cross_a),
                ((n_cross_a * endpoint1.cross(axis)) - std::sqrt(innerQuadratic)) / (n_cross_a * n_cross_a)
            );

            float length = (endpoint2 - endpoint1).norm();
            float t = axis * (ray.GetDirection() * d - endpoint1);

            if (t >= 0.0f && t <= length) {
                result.hit = true;
                result.rayDist = d;
                result.color = material;
            }

            // End caps
            {
                float d1 = (axis * endpoint1) / (axis * ray.GetDirection());
                bool case1 = (ray.GetDirection() * d1 - endpoint1).norm_squared() < pow(radius, 2.0f);
                case1 = case1 && (!result.hit || d1 < d);

                float d2 = (axis * endpoint2) / (axis * ray.GetDirection());
                bool case2 = (ray.GetDirection() * d2 - endpoint2).norm_squared() < pow(radius, 2.0f);
                case2 = case2 && (!result.hit || d2 < d);

                if (case1 || case2){
                    result.hit = true;
                    if (case1 && case2) result.rayDist = std::min(d1, d2);
                    else result.rayDist = case1 ? d1 : d2;
                    result.color = material;
                }
            }

        }

        return result;
    }

    void Cylinder::Transform(const algebra::Matrix4f& matrix) {
        endpoint1 = matrix * endpoint1;
        endpoint2 = matrix * endpoint2;
    }
#pragma endregion

}