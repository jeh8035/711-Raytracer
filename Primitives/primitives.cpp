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