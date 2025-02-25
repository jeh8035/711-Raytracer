#ifndef INERSECTION_INFO_H
#define INERSECTION_INFO_H

#include <memory>

#include "primitives.h"
#include "material.h"

namespace Primitives {
    // Information returned from a ray hit
    struct IntersectionInfo {
        bool hit = false;
        float rayDist = 0.0f;
        Direction normal;
        std::shared_ptr<Material> material;
    };
}

#endif