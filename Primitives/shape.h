#ifndef SHAPE_H
#define SHAPE_H

#include "ray.h"
#include "primitives.h"

namespace Primitives {

    // Information returned from a ray hit
    struct IntersectionInfo {
        bool hit = false;
    };

    class Shape {
        public:
            virtual IntersectionInfo Intersect(const Ray& ray) const = 0;

            void Translate(const Point& point);

        protected:
            Point position;
    };

    class Sphere : public Shape {
        private:
            float radius;

        public:
            Sphere(const float& _radius, const Point& _position);
            virtual IntersectionInfo Intersect(const Ray& ray) const;
    };

}

#endif