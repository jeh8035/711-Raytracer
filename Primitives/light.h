#ifndef LIGHT_H
#define LIGHT_H

#include "object.h"

namespace Primitives {

    class Light : Transformable {
        private:
            Point position;
            Color intensity;

        public:
            Light(Point _position, Color _intensity);
            Light() {};

            virtual void Transform(const algebra::Matrix4f& matrix);

            const Point& GetPosition() const {return position;}

            const Color& GetIntensity() const {return intensity;}
    };

}

#endif