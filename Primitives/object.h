#ifndef SHAPE_H
#define SHAPE_H

#include "ray.h"
#include "primitives.h"

namespace Primitives {
    
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

    // Information returned from a ray hit
    struct IntersectionInfo {
        bool hit = false;
        float rayDist = 0.0f;
        Direction normal;
        Color irradiance = Color(0.0f, 0.0f, 0.0f);
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

    class Object {
        public:
            virtual IntersectionInfo Intersect(const Ray& ray) const = 0;
            virtual void Transform(const algebra::Matrix4f& matrix) = 0;
        protected:
            Material material;
    };

    class Sphere : public Object {
        private:
            Point position;
            float radius;

        public:
            Sphere(const Material& _material, const float& _radius, const Point& _position);

            virtual IntersectionInfo Intersect(const Ray& ray) const;
            virtual void Transform(const algebra::Matrix4f& matrix);
    };

    class Triangle : public Object {
        private:
            Point vert1;
            Point vert2;
            Point vert3;

        public:
            Triangle(const Material& _material, const Point& _vert1, const Point& _vert2, const Point& _vert3);
            virtual IntersectionInfo Intersect(const Ray& ray) const;
            virtual void Transform(const algebra::Matrix4f& matrix);
    };

    class Cylinder : public Object {
        private:
            Point endpoint1;
            Point endpoint2;
            float radius;

        public:
            Cylinder(const Material& _material, const Point& _endpoint1, const Point& _endpoint2, const float& _radius);

            virtual IntersectionInfo Intersect(const Ray& ray) const;
            virtual void Transform(const algebra::Matrix4f& matrix);
    };
}

#endif