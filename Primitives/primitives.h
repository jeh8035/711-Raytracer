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
    class Color {
        public:
            float red = 0.0f;
            float green = 0.0f;
            float blue = 0.0f;

            Color(){};
            Color(const float& _red, const float& _green, const float& _blue);

            const Color operator*(float b) const;
            const Color operator*(const Color& color) const;
            const Color operator+(const Color& color) const;
            void operator+=(const Color& color);
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
    Direction RefractRay(Direction ray, Direction normal, float iof1, float iof2);
}

#endif