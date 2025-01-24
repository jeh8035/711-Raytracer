#ifndef SHAPE_H
#define SHAPE_H

#include "ray.h"
#include "primitives.h"

namespace Primitives {

    // Information returned from a ray hit
    struct IntersectionInfo {

    };

    class Shape {
        public:
            virtual IntersectionInfo Intersect(const Ray& ray) const = 0;
    };

    class Sphere : public Shape {
        private:
            float radius;
            Point position;

        public:
            Sphere(float _radius, Point _position);
            virtual IntersectionInfo Intersect(const Ray& ray) const;
    };

}

#endif