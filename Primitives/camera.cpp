#include "camera.h"

namespace Primitives {

    Camera::Camera(Point _position, Point _lookat, float _filmplane_width, float _filmplane_height, float _filmplane_dist)
    {
        position = _position;
        lookat = _lookat.normalize();
        filmplane_width = _filmplane_width;
        filmplane_height = _filmplane_height;
        filmplane_dist = _filmplane_dist;
    }

    algebra::Matrix4f Camera::GetViewMatrix() {
        algebra::Vector3f rot = algebra::Vector3f({
            std::atan2f(lookat.y(), lookat.z()),
            -std::atan2f(lookat.x(), lookat.z()),
            0.0
        });

        algebra::Matrix4f rot_matrix = algebra::Matrix4f({
            std::cosf(rot.y()) * std::cosf(rot.z()), std::sinf(rot.x()) * std::sinf(rot.y()) * std::cosf(rot.z()) - std::cosf(rot.x()) * std::sinf(rot.z()), std::cosf(rot.x()) * std::sinf(rot.y()) * std::cosf(rot.z()) + std::sinf(rot.x()) * std::sinf(rot.z()), 0.0f,
            std::cosf(rot.y()) * std::sinf(rot.z()), std::sinf(rot.x()) * std::sinf(rot.y()) * std::sinf(rot.z()) + std::cosf(rot.x()) * std::cosf(rot.z()), std::cosf(rot.x()) * std::sinf(rot.y()) * std::sinf(rot.z()) - std::sinf(rot.x()) * std::cosf(rot.z()), 0.0f,
            -std::sinf(rot.y()),                     std::sinf(rot.x()) * std::cos(rot.y()),                                                                 std::cosf(rot.x()) * std::cosf(rot.y()),                                                                0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        });

        algebra::Matrix4f translation_matrix = algebra::Matrix4f({
            1.0f, 0.0f, 0.0f, -position.x(),
            0.0f, 1.0f, 0.0f, -position.y(),
            0.0f, 0.0f, 1.0f, -position.z(),
            0.0f, 0.0f, 0.0f, 1.0f
        });

        return rot_matrix * translation_matrix;
    }

}