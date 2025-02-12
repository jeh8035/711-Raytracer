#ifndef LIGHT_H
#define LIGHT_H

#include "primitives.h"

namespace Primitives {

    class Light : Transformable {
        private:
            Point position;
            float intensity;

        public:
            Light(Point _position, float _intensity);
            Light() {};

            virtual void Transform(const algebra::Matrix4f& matrix);

            const Point& GetPosition() {return position;}

            const float& GetIntensity() {return intensity;}
    };

}

#endif