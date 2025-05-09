#include "light.h"

namespace Primitives {

    Light::Light(Point _position, Color _intensity) :
        position(_position),
        intensity(_intensity)
    {}

    void Light::Transform(const algebra::Matrix4f& matrix){
        position = matrix * position;
    }
}