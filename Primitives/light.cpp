#include "light.h"

namespace Primitives {

    Light::Light(Point _position) :
        position(_position)
    {}

    void Light::Transform(const algebra::Matrix4f& matrix){
        position = matrix * position;
    }
}