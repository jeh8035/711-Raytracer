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
    };

    struct Material {
        Color diffuse;
        Color specular;
        float phong_bg;
        float phong_diffuse;
        float phong_specular;
        float phong_exponent; 
        Material(){};
        Material(Color _diffuse, Color _specular, float _phong_bg, float _phong_diffuse, float _phong_specular, float _phong_exponent) :
            diffuse(_diffuse),
            specular(_specular),
            phong_bg(_phong_bg),
            phong_diffuse(_phong_diffuse),
            phong_specular(_phong_specular),
            phong_exponent(_phong_exponent)
        {}
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

    class Transformable {
        public:
            virtual void Transform(const algebra::Matrix4f& matrix) = 0;
    };


    struct IntersectionInfo;
    class Object : public Transformable {
        public:
            virtual IntersectionInfo Intersect(const Ray& ray) const = 0;

            const Material& GetMaterial() {return material;}
        protected:
            Material material;
    };

    // Information returned from a ray hit
    struct IntersectionInfo {
        bool hit = false;
        float rayDist = 0.0f;
        Direction normal;
        Color irradiance = Color(0.0f, 0.0f, 0.0f);
        std::shared_ptr<Object> object;
    };


    Direction ReflectRay(Direction ray, Direction normal);
}

#endif