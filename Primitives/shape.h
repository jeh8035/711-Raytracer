#ifndef SHAPE_H
#define SHAPE_H

#include "ray.h"
#include "primitives.h"
#include "../Libraries/tinyga/tinyga.h"

namespace Primitives {

    // Information returned from a ray hit
    struct IntersectionInfo {
        bool hit = false;
        TGAColor color = TGACOLOR(0, 0, 0);
        float rayDist = 0.0f;
    };

    class Shape {
        public:
            virtual IntersectionInfo Intersect(const Ray& ray) const = 0;
            virtual void Transform(const algebra::Matrix4f& matrix) = 0;
        protected:
            TGAColor material;
    };

    class Sphere : public Shape {
        private:
            Point position;
            float radius;

        public:
            Sphere(const TGAColor& _material, const float& _radius, const Point& _position);

            virtual IntersectionInfo Intersect(const Ray& ray) const;
            virtual void Transform(const algebra::Matrix4f& matrix);
    };


    class Triangle : public Shape {
        private:
            Point vert1;
            Point vert2;
            Point vert3;

        public:
            Triangle(const TGAColor& _material, const Point& _vert1, const Point& _vert2, const Point& _vert3);
            virtual IntersectionInfo Intersect(const Ray& ray) const;
            virtual void Transform(const algebra::Matrix4f& matrix);
    };

}

#endif