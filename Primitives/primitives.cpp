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

    Direction RefractRay(Direction ray, Direction normal, float iof1, float iof2) {
        const float index_of_refraction = iof1/iof2;

        const float cos_theta = -ray * normal;
        const float sin_2_theta = powf(index_of_refraction, 2.0f) * (1.0f - powf(cos_theta, 2.0f));

        const float inner_part = 1.0f - sin_2_theta;

        if (inner_part <= 0.0f) {
            // Total internal reflection
            return ReflectRay(ray, normal);
        }

        return index_of_refraction * ray + (index_of_refraction * cos_theta - sqrtf(inner_part)) * normal;
    }


    Color::Color(const float& _red, const float& _green, const float& _blue) :
        red(_red),
        green(_green),
        blue(_blue)
    {}

    const Color Color::operator*(float b) const{
        return Color(red * b, green * b, blue * b);
    }

    const Color Color::operator*(const Color& color) const{
        return Color(red * color.red, green * color.green, blue * color.blue);
    }

    const Color Color::operator+(const Color& color) const{
        return Color(red + color.red, green + color.green, blue + color.blue);
    }

    void Color::operator+=(const Color& color){
        red += color.red;
        green += color.green;
        blue += color.blue;
    }
}