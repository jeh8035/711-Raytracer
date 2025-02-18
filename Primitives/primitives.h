#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include <memory>

#include "../Libraries/matrix/matrix.h"

namespace Primitives {
    typedef algebra::Vector3f Point;
    typedef algebra::Vector3f Direction;

    // Multiply vec3 by mat4x4 (automatically add 1.0 to end of vec3)
    Point operator*(const algebra::Matrix<float, 4, 4>& m, const Point& a);

    // Float color
    struct Color {
        float red = 0.0f;
        float green = 0.0f;
        float blue = 0.0f;

        Color(){};
        Color(float _red, float _green, float _blue) :
            red(_red),
            green(_green),
            blue(_blue)
        {}

        inline const Color operator*(float b) const{
            return Color(red * b, green * b, blue * b);
        }

        inline const Color operator*(const Color& color) const{
            return Color(red * color.red, green * color.green, blue * color.blue);
        }

        inline const Color operator+(const Color& color) const{
            return Color(red + color.red, green + color.green, blue + color.blue);
        }

        inline const void operator+=(const Color& color){
            red += color.red;
            green += color.green;
            blue += color.blue;
        }
    };

    class Ray {
        private:
            Point position;
            Direction direction;
        public:
            Ray(const Point& _position, const Direction& _direction);

            Point GetPosition() const {return position;}
            Point GetDirection() const {return direction;}
    };


    Direction ReflectRay(Direction ray, Direction normal);
}

#endif