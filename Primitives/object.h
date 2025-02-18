#ifndef SHAPE_H
#define SHAPE_H

#include "primitives.h"
#include "material.h"

namespace Primitives {

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