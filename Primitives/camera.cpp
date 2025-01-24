#include "camera.h"

namespace Primitives {

    Camera::Camera(Point _position, Point _lookat) {
        position = _position;
        lookat = _lookat;
    }

}