#include "camera.h"

namespace Primitives {

    Camera::Camera(Point _position, Point _lookat, float _filmplane_width, float _filmplane_height, float _filmplane_dist) :
        position(_position),
        lookat(_lookat.normalize()),
        filmplane_width(_filmplane_width),
        filmplane_height(_filmplane_height),
        filmplane_dist(_filmplane_dist)
    {
        algebra::Vector3f rot = algebra::Vector3f({
            std::atan2(lookat.y(), lookat.z()),
            -std::atan2(lookat.x(), lookat.z()),
            0.0
        });

        algebra::Matrix4f rot_matrix = algebra::Matrix4f({
            std::cos(rot.y()) * std::cos(rot.z()), std::sin(rot.x()) * std::sin(rot.y()) * std::cos(rot.z()) - std::cos(rot.x()) * std::sin(rot.z()), std::cos(rot.x()) * std::sin(rot.y()) * std::cos(rot.z()) + std::sin(rot.x()) * std::sin(rot.z()), 0.0f,
            std::cos(rot.y()) * std::sin(rot.z()), std::sin(rot.x()) * std::sin(rot.y()) * std::sin(rot.z()) + std::cos(rot.x()) * std::cos(rot.z()), std::cos(rot.x()) * std::sin(rot.y()) * std::sin(rot.z()) - std::sin(rot.x()) * std::cos(rot.z()), 0.0f,
            -std::sin(rot.y()),                     std::sin(rot.x()) * std::cos(rot.y()),                                                                 std::cos(rot.x()) * std::cos(rot.y()),                                                                0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        });

        algebra::Matrix4f translation_matrix = algebra::Matrix4f({
            1.0f, 0.0f, 0.0f, -position.x(),
            0.0f, 1.0f, 0.0f, -position.y(),
            0.0f, 0.0f, 1.0f, -position.z(),
            0.0f, 0.0f, 0.0f, 1.0f
        });

        transform_matrix = rot_matrix * translation_matrix;
    }

}