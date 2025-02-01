#include "camera.h"

namespace Primitives {

    Camera::Camera(Point _position, Point _lookat) {
        position = _position;
        lookat = _lookat.normalize();
    }

    algebra::Vector3f Camera::LookAtToAngles() {
        return algebra::Vector3f({
            std::atan2f(lookat.y(), lookat.z()),
            -std::atan2f(lookat.x(), lookat.z()),
            0.0
        });
    }

}