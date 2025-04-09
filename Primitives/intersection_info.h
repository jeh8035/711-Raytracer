#ifndef INERSECTION_INFO_H
#define INERSECTION_INFO_H

#include <memory>

#include "primitives.h"
#include "material.h"

namespace Primitives {
    // Information returned from a ray hit
    struct IntersectionInfo {
        bool hit = 0;
        bool is_inside = false;
        float rayDist = 0.0f;
        float u;
        float v;
        Direction normal;
        std::shared_ptr<Material> material;
    };
}

#endif