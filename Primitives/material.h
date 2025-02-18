#ifndef MATERIAL_H
#define MATERIAL_H

#include "primitives.h"

namespace Primitives {

    struct IntersectionInfo;
    
    class Material {
        public:
            Color diffuse_color;
            Color specular_color;
            float phong_bg;
            float phong_diffuse;
            float phong_specular;
            float phong_exponent;

        public:
            Material(){};
            Material(Color _diffuse_color, Color _specular_color, float _phong_bg, float _phong_diffuse, float _phong_specular, float _phong_exponent);

            Color GetColor(Primitives::Ray ray, Primitives::IntersectionInfo intersection) const;
    };

}

#endif