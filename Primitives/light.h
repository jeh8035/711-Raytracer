#ifndef LIGHT_H
#define LIGHT_H

#include "primitives.h"

namespace Primitives {

    class Light : Transformable {
        private:
            Point position;

        public:
            Light(Point _position);
            Light() {};

            virtual void Transform(const algebra::Matrix4f& matrix);

            const Point& GetPosition() {return position;}
    };

}

#endif