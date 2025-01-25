#ifndef CAMERA_H
#define CAMERA_H

#include "primitives.h"

namespace Primitives {

    class Camera {
        private:
            Point position;
            Direction lookat;

        public:
            Camera(Point _position, Point _lookat);

            Point GetPosition() {return position;};
    };

}

#endif